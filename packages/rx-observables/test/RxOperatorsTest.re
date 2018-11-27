open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "RxOperators",
    [
      describe(
        "debounce",
        [
          observableIt(
            "debounces",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
              RxObservables.ofRelativeTimeNotifications(
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
                |> RxObservable.lift(
                  RxOperators.debounce(~scheduler, 5.0),
                   ),
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
                |> RxObservable.lift(RxOperators.defaultIfEmpty(1)),
            ~expected=[Next(1), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through if not empty",
            ~nextToString=string_of_int,
            ~source=
              _ =>
              RxObservables.ofList([1, 2, 3])
                |> RxObservable.lift(RxOperators.defaultIfEmpty(1)),
            ~expected=[Next(1), Next(2), Next(3), RxNotification.complete(None)],
            (),
          ),
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
                |> RxObservable.lift(
                  RxOperators.distinctUntilChanged(~equals=(===)),
                   ),
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
      describe(
        "every",
        [
          observableIt(
            "returns true for an subscriber that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
              RxObservables.empty()
                |> RxObservable.lift(RxOperators.every(i => i > 10)),
            ~expected=[Next(true), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "completes with false on the first observed value that fails the predicate",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
              RxObservables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, Next(12)),
                    (2.0, Next(8)),
                    (3.0, Next(14)),
                    (5.0, Next(6)),
                    (6.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.pipe2(
                  RxOperators.every(i => i > 10),
                  RxOperators.map(_ =>
                       scheduler |> RxScheduler.now |> int_of_float
                     ),
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
                |> RxObservable.lift(RxOperators.every(i => i > 10)),
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
                RxObservables.ofRelativeTimeNotifications(
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
                RxObservables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, RxNotification.complete(None)),
                    ],
                  );

                  RxObservables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservable.lift(RxOperators.exhaust);
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
                |> RxObservable.lift(RxOperators.find(x => x mod 2 === 0)),
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
                |> RxObservable.lift(RxOperators.first),
            ~expected=[Next(2), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
              RxObservables.raise(Division_by_zero)
                |> RxObservable.lift(RxOperators.first),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=
              _ => RxObservables.empty() |> RxObservable.lift(RxOperators.first),
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
                |> RxObservable.lift(RxOperators.firstOrNone),
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
                |> RxObservable.lift(RxOperators.firstOrNone),
            ~expected=[Complete(Some((Division_by_zero)))],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
              RxObservables.empty()
                |> RxObservables.pipe2(RxOperators.first, RxOperators.firstOrNone),
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
                |> RxObservable.lift(RxOperators.ignoreElements),
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
              RxObservables.ofValue(1) |> RxObservable.lift(RxOperators.isEmpty),
            ~expected=[Next(false), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "return true if empty",
            ~nextToString=string_of_bool,
            ~source=
              _ => RxObservables.empty() |> RxObservable.lift(RxOperators.isEmpty),
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
                |> RxObservable.lift(
                     RxOperators.keep(_ => raise(Division_by_zero)),
                   ),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes the subscriber when the keep subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofValue(1)
                |> RxObservable.lift(RxOperators.keep(_ => true)),
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
                |> RxObservable.lift(RxOperators.last),
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
                |> RxObservable.lift(RxOperators.last),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=
              _ => RxObservables.empty() |> RxObservable.lift(RxOperators.last),
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
              _ =>
                RxObservables.ofList([2, 3])
                |> RxObservable.lift(RxOperators.lastOrNone),
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
                |> RxObservable.lift(RxOperators.lastOrNone),
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
                |> RxObservables.pipe2(RxOperators.first, RxOperators.lastOrNone),
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
                |> RxObservable.lift(
                     RxOperators.map(_ => raise(Division_by_zero)),
                   ),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes the subscriber when the mapping subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservable.lift(RxOperators.map(i => i + 1)),
            ~expected=[Next(2), Next(3), Next(4), RxNotification.complete(None)],
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
                RxObservables.ofList([1, 2, 3])
                |> RxObservable.lift(RxOperators.mapTo("a")),
            ~expected=[Next("a"), Next("a"), Next("a"), RxNotification.complete(None)],
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
                RxObservables.ofList([1, 2, 3])
                |> RxObservable.lift(RxOperators.maybeFirst),
            ~expected=[Next(1), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.raise(Division_by_zero)
                |> RxObservable.lift(RxOperators.maybeFirst),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.pipe2(RxOperators.first, RxOperators.maybeFirst),
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
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservable.lift(RxOperators.maybeLast),
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
                |> RxObservable.lift(RxOperators.maybeLast),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.pipe2(RxOperators.first, RxOperators.maybeLast),
            ~expected=[RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe("none", []),
      describe("observeOn", []),
      describe(
        "onComplete",
        [
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2])
                |> RxObservable.lift(
                     RxOperators.onComplete(RxFunctions.alwaysUnit1),
                   ),
            ~expected=[Next(1), Next(2), RxNotification.complete(None)],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            RxObservables.ofList([1])
            |> RxObservable.lift(
                 RxOperators.onComplete(_ => incr(sideEffectCount)),
               )
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
                |> RxObservable.lift(RxOperators.onNext(RxFunctions.alwaysUnit1)),
            ~expected=[Next(1), Next(2), RxNotification.complete(None)],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            RxObservables.ofList([1])
            |> RxObservable.lift(RxOperators.onNext(_ => incr(sideEffectCount)))
            |> RxObservable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
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
                |> RxObservable.lift(
                     RxOperators.scan((acc, next) => acc + next, 0),
                   ),
            ~expected=[Next(0), Next(2), Next(5), Next(9), RxNotification.complete(None)],
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
              _ =>
                RxObservables.empty()
                |> RxObservable.lift(RxOperators.some(i => i > 10)),
            ~expected=[Next(false), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "returns false for an subscriber for which no value passes the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                RxObservables.ofList([5, 6, 7])
                |> RxObservable.lift(RxOperators.some(i => i > 10)),
            ~expected=[Next(false), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "completes with true on the first observed value that passed the predicate",
            ~nextToString=string_of_int,
            ~source=
              ({now} as scheduler) =>
                RxObservables.ofAbsoluteTimeNotifications(
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
                |> RxObservables.pipe2(
                     RxOperators.some(i => i > 10),
                     RxOperators.map(_ => now() |> int_of_float),
                   ),
            ~expected=[Next(2), RxNotification.complete(None)],
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
                  RxObservables.ofRelativeTimeNotifications(
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
                  RxObservables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, RxNotification.complete(None)),
                    ],
                  );

                RxObservables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservable.lift(RxOperators.switch_);
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
      describe("synchronize", []),
      describe(
        "timeout",
        [
          observableIt(
            "when timeout does not expire",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (10.0, Next(4)),
                    (14.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservable.lift(RxOperators.timeout(~scheduler, 5.0)),
            ~expected=[Next(1), Next(2), Next(3), Next(4), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "when timeout expires",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (15.0, Next(4)),
                    (20.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservable.lift(RxOperators.timeout(~scheduler, 5.0)),
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
                RxObservables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (200.0, Next(2)),
                    (400.0, Next(3)),
                    (600.0, Next(4)),
                    (700.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservable.lift(
                     RxOperators.withLatestFrom(
                       ~selector=(a, b) => a + b,
                       RxObservables.ofAbsoluteTimeNotifications(
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
                   ),
            ~expected=[Next(3), Next(6), Next(8), RxNotification.complete(None)],
            (),
          ),
        ],
      ),
    ]
  );