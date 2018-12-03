open ReUnit;
open ReUnit.Test;
open RxReUnit;

let ofRelativeTimeNotifications = {
  let ofRelativeTimeNotificationsScheduledSource = {
    let rec loop =
            (
              subscriber,
              notif,
              previousDelay,
              notifications,
              ~now as _,
              ~shouldYield as _,
            ) => {
              
      switch (notif) {
        | RxNotification.Next(v) => subscriber |> RxSubscriber.next(v)
        | RxNotification.Complete(exn) =>
          subscriber |> RxSubscriber.complete(~exn?)
        };        

      switch (notifications) {
      | [(requestedDelay, notif), ...tail] =>
        let computedDelay = max(0.0, requestedDelay -. previousDelay);

        RxScheduler.Result.continueAfter(
          ~delay=computedDelay,
          loop(subscriber, notif, requestedDelay, tail),
        );
      | [] => RxScheduler.Result.complete
      };
    };

    (scheduler, notifications, subscriber) =>
      switch (notifications) {
      | [(delay, notif), ...tail] =>
        let schedulerSubscription =
          scheduler
          |> RxScheduler.schedule(
               ~delay,
               loop(subscriber, notif, delay, tail),
             );
        subscriber
        |> RxSubscriber.addDisposable(schedulerSubscription)
        |> ignore;
      | [] => ()
      };
  };

  (~scheduler, notifications) =>
    RxObservable.create2(
      ofRelativeTimeNotificationsScheduledSource,
      scheduler,
      notifications,
    );
};

let ofAbsoluteTimeNotifications = (~scheduler, notifications) => {
  let currentTime = scheduler |> RxScheduler.now;

  let relativeTimeNotifications =
    notifications
    |> List.map(((time, notif)) =>
         (time -. currentTime, notif)
       );

  ofRelativeTimeNotifications(~scheduler, relativeTimeNotifications);
};

let test =
  describe(
    "RxObservables",
    [
      describe(
        "concatList",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.concatList([
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(7)),
                      (3.0, Next(9)),
                      (4.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([1, 2, 3]),
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (2.0, Next(8)),
                      (4.0, Next(10)),
                      (5.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([4, 5, 6]),
                ]),
            ~expected=[
              Next(7),
              Next(9),
              Next(1),
              Next(2),
              Next(3),
              Next(8),
              Next(10),
              Next(4),
              Next(5),
              Next(6),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "debounce",
        [
          observableIt(
            "debounces",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (15.0, Next(4)),
                    (16.0, Next(5)),
                    (17.0, Next(6)),
                    (18.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.debounce(~scheduler, 5.0),
            ~expected=[Next(3), Next(6), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "defaultIfEmpty",
        [
          observableIt(
            "returns the default if empty",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.defaultIfEmpty(1),
            ~expected=[Next(1), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through if not empty",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservables.defaultIfEmpty(1),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "defer",
        [
          it("calls the observable factory on subscribe", () => {
            let count = ref(0);

            let observable =
              RxObservables.defer(() => {
                incr(count);
                RxObservables.empty();
              });

            observable |> RxObservable.subscribe |> ignore;
            observable |> RxObservable.subscribe |> ignore;
            observable |> RxObservable.subscribe |> ignore;
            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe(
        "distinctUntilChanged",
        [
          observableIt(
            "removes duplicates",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 1, 1, 3, 5, 3, 3, 1])
                |> RxObservables.distinctUntilChanged(~equals=(===)),
            ~expected=[
              Next(1),
              Next(3),
              Next(5),
              Next(3),
              Next(1),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe("empty", []),
      describe(
        "every",
        [
          observableIt(
            "returns true for an subscriber that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.every(i => i > 10),
            ~expected=[Next(true), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "completes with false on the first observed value that fails the predicate",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, Next(12)),
                    (2.0, Next(8)),
                    (3.0, Next(14)),
                    (5.0, Next(6)),
                    (6.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.every(i => i > 10)
                |> RxObservables.map(_ =>
                     scheduler |> RxScheduler.now |> int_of_float
                   ),
            ~expected=[Next(2), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "completes with true if all values pass the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                RxObservables.ofList([12, 13])
                |> RxObservables.every(i => i > 10),
            ~expected=[Next(true), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "exhaust",
        [
          observableIt(
            "exhausts",
            ~nextToString=string_of_int,
            ~source=
              scheduler => {
                let childObservableA =
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(1)),
                      (10.0, Next(2)),
                      (20.0, Next(3)),
                      (30.0, Next(4)),
                      (40.0, RxNotification.complete(None)),
                    ],
                  );

                let childObservableB =
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, RxNotification.complete(None)),
                    ],
                  );

                ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.exhaust;
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              Next(6),
              Next(7),
              Next(8),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "find",
        [
          observableIt(
            "finds the first matching element and completes",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 3, 10, 6, 8])
                |> RxObservables.find(x => x mod 2 === 0),
            ~expected=[Next(10), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "first",
        [
          observableIt(
            "publishes the first observed value",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([2, 3])
                |> RxObservables.first,
            ~expected=[Next(2), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.raise(Division_by_zero)
                |> RxObservables.first,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty() |> RxObservables.first,
            ~expected=[Complete(Some(RxEmptyException.Exn))],
            (),
          ),
        ],
      ),
      describe(
        "firstOrNone",
        [
          observableIt(
            "publishes Some of the first observed value",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                RxObservables.ofList([2, 3])
                |> RxObservables.firstOrNone,
            ~expected=[Next(Some(2)), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "publishes Some of the first observed value",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                RxObservables.raise(Division_by_zero)
                |> RxObservables.firstOrNone,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.first
                |> RxObservables.firstOrNone,
            ~expected=[Next(None), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "ignoreElements",
        [
          observableIt(
            "ignores all elements and publishes an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  Complete(Some(Division_by_zero)),
                ])
                |> RxObservables.ignoreElements,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
        ],
      ),
      describe(
        "isEmpty",
        [
          observableIt(
            "return false if not empty",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                RxObservables.ofValue(1)
                |> RxObservables.isEmpty,
            ~expected=[Next(false), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "return true if empty",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.isEmpty,
            ~expected=[Next(true), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "keep",
        [
          observableIt(
            "completes the subscriber when the predicate throws an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofValue(1)
                |> RxObservables.keep(_ => raise(Division_by_zero)),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes the subscriber when the keep subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofValue(1)
                |> RxObservables.keep(_ => true),
            ~expected=[Next(1), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "last",
        [
          observableIt(
            "publishes the last observed value and disposes",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservables.last,
            ~expected=[Next(3), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  Complete(Some(Division_by_zero)),
                ])
                |> RxObservables.last,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty() |> RxObservables.last,
            ~expected=[Complete(Some(RxEmptyException.Exn))],
            (),
          ),
        ],
      ),
      describe(
        "lastOrNone",
        [
          observableIt(
            "publishes the Some of the last observed value and completes",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ => RxObservables.ofList([2, 3]) |> RxObservables.lastOrNone,
            ~expected=[Next(Some(3)), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  Next(1),
                  Next(2),
                  Complete(Some(Division_by_zero)),
                ])
                |> RxObservables.lastOrNone,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.first
                |> RxObservables.lastOrNone,
            ~expected=[Next(None), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "map",
        [
          observableIt(
            "completes the subscriber when the mapper throws an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservables.map(_ => raise(Division_by_zero)),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes the subscriber when the mapping subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservables.map(i => i + 1),
            ~expected=[
              Next(2),
              Next(3),
              Next(4),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "mapTo",
        [
          observableIt(
            "maps any input to value",
            ~nextToString=RxFunctions.identity,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3]) |> RxObservables.mapTo("a"),
            ~expected=[
              Next("a"),
              Next("a"),
              Next("a"),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "maybeFirst",
        [
          observableIt(
            "publishes the first observed value",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3]) |> RxObservables.maybeFirst,
            ~expected=[Next(1), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.raise(Division_by_zero)
                |> RxObservables.maybeFirst,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.first
                |> RxObservables.maybeFirst,
            ~expected=[RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "maybeLast",
        [
          observableIt(
            "publishes the last observed value",
            ~nextToString=string_of_int,
            ~source=
              _ => RxObservables.ofList([1, 2, 3]) |> RxObservables.maybeLast,
            ~expected=[Next(3), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  Next(1),
                  Next(2),
                  Complete(Some(Division_by_zero)),
                ])
                |> RxObservables.maybeLast,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.first
                |> RxObservables.maybeLast,
            ~expected=[RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "mergeList",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.mergeList([
                  ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(7)),
                      (3.0, Next(9)),
                      (4.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([1, 2, 3]),
                  ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (2.0, Next(8)),
                      (4.0, Next(10)),
                      (5.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([4, 5, 6]),
                ]),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              Next(6),
              Next(7),
              Next(8),
              Next(9),
              Next(10),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe("none", []),
      describe("observeOn", []),
      describe("ofList", []),
      describe("ofValue", []),
      describe(
        "onComplete",
        [
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2])
                |> RxObservables.onComplete(RxFunctions.alwaysUnit1),
            ~expected=[Next(1), Next(2), RxNotification.complete(None)],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            RxObservables.ofList([1])
            |> RxObservables.onComplete(_ => incr(sideEffectCount))
            |> RxObservable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe(
        "onNext",
        [
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2])
                |> RxObservables.onNext(RxFunctions.alwaysUnit1),
            ~expected=[Next(1), Next(2), RxNotification.complete(None)],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            RxObservables.ofList([1])
            |> RxObservables.onNext(_ => incr(sideEffectCount))
            |> RxObservable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe("raise", []),
      describe(
        "retry",
        [
          observableIt(
            "with cold observable",
            ~nextToString=string_of_int,
            ~source=
              _ => {
                let retryCount = ref(0);
                let predicate = _ => {
                  let retry = retryCount^ < 2;
                  if (retry) {
                    incr(retryCount);
                  };
                  retry;
                };

                RxObservable.create(subscriber => {
                  subscriber |> RxSubscriber.next(1);
                  subscriber |> RxSubscriber.next(2);
                  subscriber |> RxSubscriber.complete(~exn=Division_by_zero);
                })
                |> RxObservables.retry(~predicate);
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(1),
              Next(2),
              Next(1),
              Next(2),
              RxNotification.complete(Some(Division_by_zero)),
            ],
            (),
          ),
          it("with hot observable", () => {
            let result = ref([]);
            let subject = ref(RxSubject.createMulticast());

            let subscription =
              RxObservable.create(subscriber => {
                subject := RxSubject.createMulticast();
                let observable = subject^ |> RxSubject.asObservable;
                let subscription =
                  observable
                  |> RxObservables.observe(
                       ~onNext=next => subscriber |> RxSubscriber.next(next),
                       ~onComplete=
                         exn => subscriber |> RxSubscriber.complete(~exn?),
                     )
                  |> RxObservable.subscribe;

                subscriber
                |> RxSubscriber.addDisposable(subscription)
                |> ignore;
              })
              |> RxObservables.retry
              |> RxObservables.onNext(x => result := [x, ...result^])
              |> RxObservable.subscribe;

            let subscriber = subject^;
            subscriber |> RxSubject.next(1);
            subscriber |> RxSubject.next(2);
            subscriber |> RxSubject.complete(~exn=Division_by_zero);
            subscription |> RxDisposable.isDisposed |> Expect.toBeEqualToFalse;

            let subscriber = subject^;
            subscriber |> RxSubject.next(3);
            subscriber |> RxSubject.next(4);
            subscriber |> RxSubject.complete;
            subscription |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;

            result^ |> Expect.toBeEqualToListOfInt([4, 3, 2, 1]);
          }),
          it("doesn't retry if unsubscribed in shouldRetry callback", () => {
            let vts = RxVirtualTimeScheduler.create();
            let scheduler = vts |> RxVirtualTimeScheduler.asScheduler;

            let subscription = ref(RxDisposable.disposed);
            subscription :=
              ofAbsoluteTimeNotifications(
                ~scheduler,
                [
                  (1.0, Next(5)),
                  (2.0, RxNotification.complete(Some(Division_by_zero))),
                ],
              )
              |> RxObservables.retry(~predicate=_ => {
                   subscription^ |> RxDisposable.dispose;
                   true;
                 })
              |> expectObservableToProduce(
                   ~nextToString=string_of_int,
                   [Next(5), RxNotification.complete(None)],
                 );

            vts |> RxVirtualTimeScheduler.run;
          }),
        ],
      ),
      describe(
        "scan",
        [
          observableIt(
            "publishes all intermediate values, including the initial accumulator value",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([2, 3, 4])
                |> RxObservables.scan((acc, next) => acc + next, 0),
            ~expected=[
              Next(0),
              Next(2),
              Next(5),
              Next(9),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe("share", []),
      describe(
        "shareWithReplayBuffer",
        [
          observableIt(
            "replays event on subscribe",
            ~nextToString=string_of_int,
            ~source=
              scheduler => {
                let source =
                  ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(1)),
                      (2.0, Next(2)),
                      (4.0, Next(3)),
                      (6.0, Next(4)),
                      (9.0, RxNotification.complete(None)),
                    ],
                  )
                  |> RxObservables.shareWithReplayBuffer(2);

                RxObservables.mergeList([
                  source,
                  source |> RxObservables.subscribeOn(~delay=5.0, scheduler),
                ]);
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(2),
              Next(3),
              Next(4),
              Next(4),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "some",
        [
          observableIt(
            "returns false for an subscriber that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=
              _ => RxObservables.empty() |> RxObservables.some(i => i > 10),
            ~expected=[Next(false), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "returns false for an subscriber for which no value passes the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                RxObservables.ofList([5, 6, 7])
                |> RxObservables.some(i => i > 10),
            ~expected=[Next(false), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "completes with true on the first observed value that passed the predicate",
            ~nextToString=string_of_int,
            ~source=
              ({now} as scheduler) =>
                ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, Next(8)),
                    (2.0, Next(11)),
                    (3.0, Next(14)),
                    (4.0, Next(6)),
                    (5.0, Next(5)),
                    (6.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.some(i => i > 10)
                |> RxObservables.map(_ => now() |> int_of_float),
            ~expected=[Next(2), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "startWithlist",
        [
          observableIt(
            "prepends the values",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.startWithList(
                  [1, 2, 3],
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(4)),
                      (2.0, Next(5)),
                      (3.0, RxNotification.complete(None)),
                    ],
                  ),
                ),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "startWithValue",
        [
          observableIt(
            "prepends the values",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.startWithValue(
                  1,
                  RxObservables.ofList([2, 3]),
                ),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "switch_",
        [
          observableIt(
            "switches",
            ~nextToString=string_of_int,
            ~source=
              scheduler => {
                let childObservableA =
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(1)),
                      (10.0, Next(2)),
                      (20.0, Next(3)),
                      (30.0, Next(4)),
                      (40.0, RxNotification.complete(None)),
                    ],
                  );

                let childObservableB =
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, RxNotification.complete(None)),
                    ],
                  );

                ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.switch_;
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(5),
              Next(6),
              Next(7),
              Next(1),
              Next(2),
              Next(3),
              Next(5),
              Next(6),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe("subscribeOn", []),
      describe(
        "timeout",
        [
          observableIt(
            "when timeout does not expire",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (10.0, Next(4)),
                    (14.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.timeout(~scheduler, 5.0),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "when timeout expires",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (15.0, Next(4)),
                    (20.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.timeout(~scheduler, 5.0),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              RxNotification.complete(Some(RxTimeoutException.Exn)),
            ],
            (),
          ),
        ],
      ),
      describe(
        "withLatestFrom",
        [
          observableIt(
            "drops values from the source, if there is no latest value",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (200.0, Next(2)),
                    (400.0, Next(3)),
                    (600.0, Next(4)),
                    (700.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.withLatestFrom(
                     ~selector=(a, b) => a + b,
                     ofAbsoluteTimeNotifications(
                       ~scheduler,
                       [
                         (100.0, Next(1)),
                         (250.0, Next(2)),
                         (300.0, Next(3)),
                         (450.0, Next(4)),
                         (500.0, RxNotification.complete(None)),
                       ],
                     ),
                   ),
            ~expected=[
              Next(3),
              Next(6),
              Next(8),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
    ],
  );