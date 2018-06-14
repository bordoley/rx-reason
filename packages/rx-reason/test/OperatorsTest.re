open ReUnit;
open ReUnit.Test;

let expectToBeEqualToListOfNotifications = (~nextEquals=(===), ~nextToString) =>
  Expect.toBeEqualToListWith(
    ~equals=Notification.equals(~nextEquals),
    ~toString=Notification.toString(~nextToString),
  );

let operatorIt =
    (
      name,
      ~nextEquals=(===),
      ~nextToString,
      ~source: ClockScheduler.t => Observable.t('a),
      ~operator: ClockScheduler.t => Operator.t('a, 'b),
      ~expected: list(Notification.t('b)),
      (),
    ) =>
  it(
    name,
    () => {
      let vts = VirtualTimeScheduler.create();
      let scheduler = vts |> VirtualTimeScheduler.toClockScheduler;

      let subscription =
        source(scheduler)
        |> Observable.lift(
             Operators.(operator(scheduler) >> materialize >> toList),
           )
        |> Observable.lift(
             Operators.onNext(
               expectToBeEqualToListOfNotifications(
                 ~nextEquals,
                 ~nextToString,
                 expected,
               ),
             ),
           )
        |> Observable.subscribe;

      vts |> VirtualTimeScheduler.run;
      subscription |> Disposable.dispose;
    },
  );

let test =
  describe(
    "Operators",
    [
      describe(
        "debounce",
        [
          operatorIt(
            "debounces",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay}) =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=scheduleWithDelay,
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 15.0),
                    (Next(5), 16.0),
                    (Next(6), 17.0),
                    (Complete, 18.0),
                  ],
                ),
            ~operator=
              ({scheduleWithDelay}) =>
                Operators.debounce(scheduleWithDelay(~delay=5.0)),
            ~expected=[Next(3), Next(6), Complete],
            (),
          ),
        ],
      ),
      describe(
        "defaultIfEmpty",
        [
          operatorIt(
            "returns the default if empty",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.defaultIfEmpty(1),
            ~expected=[Next(1), Complete],
            (),
          ),
          operatorIt(
            "passes through if not empty",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.defaultIfEmpty(1),
            ~expected=[Next(1), Next(2), Next(3), Complete],
            (),
          ),
        ],
      ),
      describe(
        "distinctUntilChanged",
        [
          operatorIt(
            "removes duplicates",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 1, 1, 3, 5, 3, 3, 1]),
            ~operator=_ => Operators.distinctUntilChanged(~equals=(===)),
            ~expected=[
              Next(1),
              Next(3),
              Next(5),
              Next(3),
              Next(1),
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe(
        "every",
        [
          operatorIt(
            "returns true for an observer that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.every(i => i > 10),
            ~expected=[Next(true), Complete],
            (),
          ),
          operatorIt(
            "completes with false on the first observed value that fails the predicate",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (Next(12), 1.0),
                    (Next(8), 2.0),
                    (Next(14), 3.0),
                    (Next(13), 4.0),
                    (Next(6), 5.0),
                    (Complete, 6.0),
                  ],
                ),
            ~operator=
              ({getCurrentTime}) =>
                Operators.(
                  every(i => i > 10)
                  >> map(_ => getCurrentTime() |> int_of_float)
                ),
            ~expected=[Next(2), Complete],
            (),
          ),
          operatorIt(
            "completes with true if all values pass the predicate",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.ofList([12, 13]),
            ~operator=_ => Operators.every(i => i > 10),
            ~expected=[Next(true), Complete],
            (),
          ),
        ],
      ),
      describe(
        "exhaust",
        [
          operatorIt(
            "exhausts",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay} as scheduler) => {
                let childObservableA =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (Next(1), 0.0),
                      (Next(2), 10.0),
                      (Next(3), 20.0),
                      (Next(4), 30.0),
                      (Complete, 40.0),
                    ],
                  );

                let childObservableB =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (Next(5), 0.0),
                      (Next(6), 10.0),
                      (Next(7), 19.0),
                      (Next(8), 30.0),
                      (Complete, 40.0),
                    ],
                  );

                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (Next(childObservableA), 0.0),
                    (Next(childObservableB), 15.0),
                    (Next(childObservableA), 35.0),
                    (Next(childObservableB), 60.0),
                    (Complete, 75.0),
                  ],
                );
              },
            ~operator=_ => Operators.exhaust,
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              Next(6),
              Next(7),
              Next(8),
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe(
        "find",
        [
          operatorIt(
            "finds the first matching element and completes",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 3, 10, 6, 8]),
            ~operator=_ => Operators.find(x => x mod 2 === 0),
            ~expected=[Next(10), Complete],
            (),
          ),
        ],
      ),
      describe(
        "first",
        [
          operatorIt(
            "publishes the first observed value",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([2, 3]),
            ~operator=_ => Operators.first,
            ~expected=[Next(2), Complete],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=_ => Observable.raise(Division_by_zero),
            ~operator=_ => Operators.first,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.first,
            ~expected=[CompleteWithException(Operators.EmptyException)],
            (),
          ),
        ],
      ),
      describe(
        "firstOrNone",
        [
          operatorIt(
            "publishes Some of the first observed value",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.ofList([2, 3]),
            ~operator=_ => Operators.firstOrNone,
            ~expected=[Next(Some(2)), Complete],
            (),
          ),
          operatorIt(
            "publishes Some of the first observed value",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.raise(Division_by_zero),
            ~operator=_ => Operators.firstOrNone,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> firstOrNone),
            ~expected=[Next(None), Complete],
            (),
          ),
        ],
      ),
      describe(
        "ignoreElements",
        [
          operatorIt(
            "ignores all elements and publishes an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  CompleteWithException(Division_by_zero),
                ]),
            ~operator=_ => Operators.ignoreElements,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
        ],
      ),
      describe(
        "isEmpty",
        [
          operatorIt(
            "return false if not empty",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.ofValue(1),
            ~operator=_ => Operators.isEmpty,
            ~expected=[Next(false), Complete],
            (),
          ),
          operatorIt(
            "return true if empty",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.isEmpty,
            ~expected=[Next(true), Complete],
            (),
          ),
        ],
      ),
      describe(
        "keep",
        [
          operatorIt(
            "completes the observer when the predicate throws an exception",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofValue(1),
            ~operator=_ => Operators.keep(_ => raise(Division_by_zero)),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "completes the observer when the keep observer is completed",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofValue(1),
            ~operator=_ => Operators.keep(_ => true),
            ~expected=[Next(1), Complete],
            (),
          ),
        ],
      ),
      describe(
        "last",
        [
          operatorIt(
            "publishes the last observed value and disposes",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.last,
            ~expected=[Next(3), Complete],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  CompleteWithException(Division_by_zero),
                ]),
            ~operator=_ => Operators.last,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.last,
            ~expected=[CompleteWithException(Operators.EmptyException)],
            (),
          ),
        ],
      ),
      describe(
        "lastOrNone",
        [
          operatorIt(
            "publishes the Some of the last observed value and completes",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.ofList([2, 3]),
            ~operator=_ => Operators.lastOrNone,
            ~expected=[Next(Some(3)), Complete],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  CompleteWithException(Division_by_zero),
                ]),
            ~operator=_ => Operators.lastOrNone,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> lastOrNone),
            ~expected=[Next(None), Complete],
            (),
          ),
        ],
      ),
      describe(
        "map",
        [
          operatorIt(
            "completes the observer when the mapper throws an exception",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.map(_ => raise(Division_by_zero)),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "completes the observer when the mapping observer is completed",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.map(i => i + 1),
            ~expected=[Next(2), Next(3), Next(4), Complete],
            (),
          ),
        ],
      ),
      describe(
        "mapTo",
        [
          operatorIt(
            "maps any input to value",
            ~nextToString=Functions.identity,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.mapTo("a"),
            ~expected=[Next("a"), Next("a"), Next("a"), Complete],
            (),
          ),
        ],
      ),
      describe(
        "maybeFirst",
        [
          operatorIt(
            "publishes the first observed value",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.maybeFirst,
            ~expected=[Next(1), Complete],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=_ => Observable.raise(Division_by_zero),
            ~operator=_ => Operators.maybeFirst,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> maybeFirst),
            ~expected=[Complete],
            (),
          ),
        ],
      ),
      describe(
        "maybeLast",
        [
          operatorIt(
            "publishes the last observed value",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.maybeLast,
            ~expected=[Next(3), Complete],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  CompleteWithException(Division_by_zero),
                ]),
            ~operator=_ => Operators.maybeLast,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          operatorIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> maybeLast),
            ~expected=[Complete],
            (),
          ),
        ],
      ),
      describe("none", []),
      describe("observeOn", []),
      describe(
        "onComplete",
        [
          operatorIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2]),
            ~operator=_ => Operators.onComplete(Functions.alwaysUnit),
            ~expected=[Next(1), Complete],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            Observable.ofList([1])
            |> Observable.lift(
                 Operators.onComplete(_ =>
                   sideEffectCount := sideEffectCount^ + 1
                 ),
               )
            |> Observable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe(
        "onNext",
        [
          operatorIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2]),
            ~operator=_ => Operators.onNext(Functions.alwaysUnit),
            ~expected=[Next(1), Complete],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            Observable.ofList([1])
            |> Observable.lift(
                 Operators.onNext(_ =>
                   sideEffectCount := sideEffectCount^ + 1
                 ),
               )
            |> Observable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe(
        "scan",
        [
          operatorIt(
            "publishes all intermediate values, including the initial accumulator value",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([2, 3, 4]),
            ~operator=_ => Operators.scan((acc, next) => acc + next, 0),
            ~expected=[Next(0), Next(2), Next(5), Next(9), Complete],
            (),
          ),
        ],
      ),
      describe(
        "some",
        [
          operatorIt(
            "returns false for an observer that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.some(i => i > 10),
            ~expected=[Next(false), Complete],
            (),
          ),
          operatorIt(
            "returns false for an observer for which no value passes the predicate",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.ofList([5, 6, 7]),
            ~operator=_ => Operators.some(i => i > 10),
            ~expected=[Next(false), Complete],
            (),
          ),
          operatorIt(
            "completes with true on the first observed value that passed the predicate",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (Next(8), 1.0),
                    (Next(11), 2.0),
                    (Next(14), 3.0),
                    (Next(6), 4.0),
                    (Next(5), 5.0),
                    (Complete, 6.0),
                  ],
                ),
            ~operator=
              ({getCurrentTime}) =>
                Operators.(
                  some(i => i > 10)
                  >> map(_ => getCurrentTime() |> int_of_float)
                ),
            ~expected=[Next(2), Complete],
            (),
          ),
        ],
      ),
      describe(
        "switch_",
        [
          operatorIt(
            "switches",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay} as scheduler) => {
                let childObservableA =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (Next(1), 0.0),
                      (Next(2), 10.0),
                      (Next(3), 20.0),
                      (Next(4), 30.0),
                      (Complete, 40.0),
                    ],
                  );

                let childObservableB =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (Next(5), 0.0),
                      (Next(6), 10.0),
                      (Next(7), 19.0),
                      (Next(8), 30.0),
                      (Complete, 40.0),
                    ],
                  );

                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (Next(childObservableA), 0.0),
                    (Next(childObservableB), 15.0),
                    (Next(childObservableA), 35.0),
                    (Next(childObservableB), 60.0),
                    (Complete, 75.0),
                  ],
                );
              },
            ~operator=_ => Operators.switch_,
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
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe("synchronize", []),
      describe(
        "timeout",
        [
          operatorIt(
            "when timeout does not expire",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay}) =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=scheduleWithDelay,
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 10.0),
                    (Complete, 14.0),
                  ],
                ),
            ~operator=
              ({scheduleWithDelay}) =>
                Operators.timeout(scheduleWithDelay(~delay=5.0)),
            ~expected=[Next(1), Next(2), Next(3), Next(4), Complete],
            (),
          ),
          operatorIt(
            "when timeout expires",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay}) =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=scheduleWithDelay,
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 15.0),
                    (Complete, 20.0),
                  ],
                ),
            ~operator=
              ({scheduleWithDelay}) =>
                Operators.timeout(scheduleWithDelay(~delay=5.0)),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              CompleteWithException(Operators.TimeoutException),
            ],
            (),
          ),
        ],
      ),
      describe(
        "withLatestFrom",
        [
          operatorIt(
            "drops values from the source, if there is no latest value",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (Next(1), 0.0),
                    (Next(2), 200.0),
                    (Next(3), 400.0),
                    (Next(4), 600.0),
                    (Complete, 700.0),
                  ],
                ),
            ~operator=
              scheduler => {
                let other =
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (Next(1), 100.0),
                      (Next(2), 250.0),
                      (Next(3), 300.0),
                      (Next(4), 450.0),
                      (Complete, 500.0),
                    ],
                  );
                Operators.withLatestFrom(~selector=(a, b) => a + b, other);
              },
            ~expected=[Next(3), Next(6), Next(8), Complete],
            (),
          ),
        ],
      ),
    ],
  );