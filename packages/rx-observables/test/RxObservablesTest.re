open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "RxObservables",
    [
      describe(
        "concat",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.concat([
                  RxObservables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(7)),
                      (3.0, Next(9)),
                      (4.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([1, 2, 3]),
                  RxObservables.ofRelativeTimeNotifications(
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
      describe("empty", []),
      describe(
        "merge",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.merge([
                  RxObservables.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(7)),
                      (3.0, Next(9)),
                      (4.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([1, 2, 3]),
                  RxObservables.ofAbsoluteTimeNotifications(
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
      describe("ofList", []),
      describe("ofValue", []),
      describe("publish", []),
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
            let subject = ref(RxSubject.create());

            let subscription =
              RxObservable.create(subscriber => {
                subject := RxSubject.create();
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
                |> RxSubscriber.addTeardown1(
                     RxDisposable.dispose,
                     subscription,
                   )
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
              RxObservables.ofAbsoluteTimeNotifications(
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
                  RxObservables.ofAbsoluteTimeNotifications(
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

                RxObservables.merge([
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
        "startWithlist",
        [
          observableIt(
            "prepends the values",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.startWithList(
                  [1, 2, 3],
                  RxObservables.ofRelativeTimeNotifications(
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
      describe("subscribeOn", []),
    ],
  );