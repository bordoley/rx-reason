open ReUnit;
open ReUnit.Test;
open ReUnitHelpers;

let test =
  describe(
    "Observables",
    [
      describe(
        "concat",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observables.concat([
                  Observables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [(1.0, Next(7)), (3.0, Next(9)), (4.0, Complete)],
                  ),
                  Observables.ofList([1, 2, 3]),
                  Observables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [(2.0, Next(8)), (4.0, Next(10)), (5.0, Complete)],
                  ),
                  Observables.ofList([4, 5, 6]),
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
              Complete,
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
              Observables.defer(() => {
                incr(count);
                Observables.empty();
              });

            observable |> Observable.subscribe |> ignore;
            observable |> Observable.subscribe |> ignore;
            observable |> Observable.subscribe |> ignore;
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
                Observables.merge([
                  Observables.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(1.0, Next(7)), (3.0, Next(9)), (4.0, Complete)],
                  ),
                  Observables.ofList([1, 2, 3]),
                  Observables.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(2.0, Next(8)), (4.0, Next(10)), (5.0, Complete)],
                  ),
                  Observables.ofList([4, 5, 6]),
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
              Complete,
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

                Observable.create(subscriber => {
                  subscriber |> Subscriber.next(1);
                  subscriber |> Subscriber.next(2);
                  subscriber |> Subscriber.complete(~exn=Division_by_zero);
                })
                |> Observables.retry(~predicate);
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(1),
              Next(2),
              Next(1),
              Next(2),
              CompleteWithException(Division_by_zero),
            ],
            (),
          ),
          it("with hot observable", () => {
            let result = ref([]);
            let subject = ref(Subject.create());

            let subscription =
              Observable.create(subscriber => {
                subject := Subject.create();
                let observable = subject^ |> Subject.asObservable;
                let subscription =
                  observable
                  |> Observable.lift(
                       Operators.observe(
                         ~onNext=next => subscriber |> Subscriber.next(next),
                         ~onComplete=
                           exn => subscriber |> Subscriber.complete(~exn?),
                       ),
                     )
                  |> Observable.subscribe;

                subscriber
                |> Subscriber.addTeardown1(Disposable.dispose, subscription)
                |> ignore;
              })
              |> Observables.retry
              |> Observable.lift(Operators.onNext(x => result := [x, ...result^]))
              |> Observable.subscribe;

            let subscriber = subject^;
            subscriber |> Subject.next(1);
            subscriber |> Subject.next(2);
            subscriber |> Subject.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            let subscriber = subject^;
            subscriber |> Subject.next(3);
            subscriber |> Subject.next(4);
            subscriber |> Subject.complete;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;

            result^ |> Expect.toBeEqualToListOfInt([4, 3, 2, 1]);
          }),
          it("doesn't retry if unsubscribed in shouldRetry callback", () => {
            let vts = VirtualTimeScheduler.create();
            let scheduler = vts |> VirtualTimeScheduler.asScheduler;

            let subscription = ref(Disposable.disposed);
            subscription :=
              Observables.ofAbsoluteTimeNotifications(
                ~scheduler,
                [
                  (1.0, Next(5)),
                  (2.0, CompleteWithException(Division_by_zero)),
                ],
              )
              |> Observables.retry(~predicate=_ => {
                   subscription^ |> Disposable.dispose;
                   true;
                 })
              |> expectObservableToProduce(
                   ~nextToString=string_of_int,
                   [Next(5), Complete],
                 );

            vts |> VirtualTimeScheduler.run;
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
                  Observables.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(1)),
                      (2.0, Next(2)),
                      (4.0, Next(3)),
                      (6.0, Next(4)),
                      (9.0, Complete),
                    ],
                  )
                  |> Observables.shareWithReplayBuffer(2)

                Observables.merge([
                  source,
                  source |> Observables.subscribeOn(~delay=5.0, scheduler),
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
              Complete,
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
                Observables.startWithList(
                  [1, 2, 3],
                  Observables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [(1.0, Next(4)), (2.0, Next(5)), (3.0, Complete)],
                  ),
                ),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              Complete,
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
              _ => Observables.startWithValue(1, Observables.ofList([2, 3])),
            ~expected=[Next(1), Next(2), Next(3), RxReason.Notification.complete(None)],
            (),
          ),
        ],
      ),
      describe("subscribeOn", []),
    ],
  );