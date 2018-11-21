open ReUnit;
open ReUnit.Test;
open ReUnitHelpers;

let test =
  describe(
    "Operators",
    [
      describe(
        "debounce",
        [
          observableIt(
            "debounces",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observables.ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (15.0, Next(4)),
                    (16.0, Next(5)),
                    (17.0, Next(6)),
                    (18.0, Complete),
                  ],
                )
                |> Observable.lift(
                     Operators.debounce(~scheduler, ~dueTime=5.0),
                   ),
            ~expected=[Next(3), Next(6), Complete],
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
                Observables.empty()
                |> Observable.lift(Operators.defaultIfEmpty(1)),
            ~expected=[Next(1), Complete],
            (),
          ),
          observableIt(
            "passes through if not empty",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.ofList([1, 2, 3])
                |> Observable.lift(Operators.defaultIfEmpty(1)),
            ~expected=[Next(1), Next(2), Next(3), Complete],
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
                Observables.ofList([1, 1, 1, 3, 5, 3, 3, 1])
                |> Observable.lift(
                     Operators.distinctUntilChanged(~equals=(===)),
                   ),
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
          observableIt(
            "returns true for an subscriber that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                Observables.empty()
                |> Observable.lift(Operators.every(i => i > 10)),
            ~expected=[Next(true), Complete],
            (),
          ),
          observableIt(
            "completes with false on the first observed value that fails the predicate",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, Next(12)),
                    (2.0, Next(8)),
                    (3.0, Next(14)),
                    (5.0, Next(6)),
                    (6.0, Complete),
                  ],
                )
                |> Observable.pipe2(
                     Operators.every(i => i > 10),
                     Operators.map(_ =>
                       scheduler |> Scheduler.now |> int_of_float
                     ),
                   ),
            ~expected=[Next(2), Complete],
            (),
          ),
          observableIt(
            "completes with true if all values pass the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                Observables.ofList([12, 13])
                |> Observable.lift(Operators.every(i => i > 10)),
            ~expected=[Next(true), Complete],
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
                  Observables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(1)),
                      (10.0, Next(2)),
                      (20.0, Next(3)),
                      (30.0, Next(4)),
                      (40.0, Complete),
                    ],
                  );

                let childObservableB =
                  Observables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, Complete),
                    ],
                  );

                Observables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, Complete),
                  ],
                )
                |> Observable.lift(Operators.exhaust);
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
              Complete,
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
                Observables.ofList([1, 3, 10, 6, 8])
                |> Observable.lift(Operators.find(x => x mod 2 === 0)),
            ~expected=[Next(10), Complete],
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
                Observables.ofList([2, 3])
                |> Observable.lift(Operators.first),
            ~expected=[Next(2), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.raise(Division_by_zero)
                |> Observable.lift(Operators.first),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=
              _ => Observables.empty() |> Observable.lift(Operators.first),
            ~expected=[CompleteWithException(EmptyException.Exn)],
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
                Observables.ofList([2, 3])
                |> Observable.lift(Operators.firstOrNone),
            ~expected=[Next(Some(2)), Complete],
            (),
          ),
          observableIt(
            "publishes Some of the first observed value",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observables.raise(Division_by_zero)
                |> Observable.lift(Operators.firstOrNone),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observables.empty()
                |> Observable.pipe2(Operators.first, Operators.firstOrNone),
            ~expected=[Next(None), Complete],
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
                Observables.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.lift(Operators.ignoreElements),
            ~expected=[CompleteWithException(Division_by_zero)],
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
                Observables.ofValue(1) |> Observable.lift(Operators.isEmpty),
            ~expected=[Next(false), Complete],
            (),
          ),
          observableIt(
            "return true if empty",
            ~nextToString=string_of_bool,
            ~source=
              _ => Observables.empty() |> Observable.lift(Operators.isEmpty),
            ~expected=[Next(true), Complete],
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
                Observables.ofValue(1)
                |> Observable.lift(
                     Operators.keep(_ => raise(Division_by_zero)),
                   ),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes the subscriber when the keep subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.ofValue(1)
                |> Observable.lift(Operators.keep(_ => true)),
            ~expected=[Next(1), Complete],
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
                Observables.ofList([1, 2, 3])
                |> Observable.lift(Operators.last),
            ~expected=[Next(3), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.lift(Operators.last),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=
              _ => Observables.empty() |> Observable.lift(Operators.last),
            ~expected=[CompleteWithException(EmptyException.Exn)],
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
                Observables.ofList([2, 3])
                |> Observable.lift(Operators.lastOrNone),
            ~expected=[Next(Some(3)), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observables.ofNotifications([
                  Next(1),
                  Next(2),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.lift(Operators.lastOrNone),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observables.empty()
                |> Observable.pipe2(Operators.first, Operators.lastOrNone),
            ~expected=[Next(None), Complete],
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
                Observables.ofList([1, 2, 3])
                |> Observable.lift(
                     Operators.map(_ => raise(Division_by_zero)),
                   ),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes the subscriber when the mapping subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.ofList([1, 2, 3])
                |> Observable.lift(Operators.map(i => i + 1)),
            ~expected=[Next(2), Next(3), Next(4), Complete],
            (),
          ),
        ],
      ),
      describe(
        "mapTo",
        [
          observableIt(
            "maps any input to value",
            ~nextToString=Functions.identity,
            ~source=
              _ =>
                Observables.ofList([1, 2, 3])
                |> Observable.lift(Operators.mapTo("a")),
            ~expected=[Next("a"), Next("a"), Next("a"), Complete],
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
                Observables.ofList([1, 2, 3])
                |> Observable.lift(Operators.maybeFirst),
            ~expected=[Next(1), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.raise(Division_by_zero)
                |> Observable.lift(Operators.maybeFirst),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.empty()
                |> Observable.pipe2(Operators.first, Operators.maybeFirst),
            ~expected=[Complete],
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
                Observables.ofList([1, 2, 3])
                |> Observable.lift(Operators.maybeLast),
            ~expected=[Next(3), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.ofNotifications([
                  Next(1),
                  Next(2),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.lift(Operators.maybeLast),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.empty()
                |> Observable.pipe2(Operators.first, Operators.maybeLast),
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
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.ofList([1, 2])
                |> Observable.lift(
                     Operators.onComplete(Functions.alwaysUnit1),
                   ),
            ~expected=[Next(1), Next(2), Complete],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            Observables.ofList([1])
            |> Observable.lift(
                 Operators.onComplete(_ => incr(sideEffectCount)),
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
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observables.ofList([1, 2])
                |> Observable.lift(Operators.onNext(Functions.alwaysUnit1)),
            ~expected=[Next(1), Next(2), Complete],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            Observables.ofList([1])
            |> Observable.lift(Operators.onNext(_ => incr(sideEffectCount)))
            |> Observable.subscribe
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
                Observables.ofList([2, 3, 4])
                |> Observable.lift(
                     Operators.scan((acc, next) => acc + next, 0),
                   ),
            ~expected=[Next(0), Next(2), Next(5), Next(9), Complete],
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
                Observables.empty()
                |> Observable.lift(Operators.some(i => i > 10)),
            ~expected=[Next(false), Complete],
            (),
          ),
          observableIt(
            "returns false for an subscriber for which no value passes the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                Observables.ofList([5, 6, 7])
                |> Observable.lift(Operators.some(i => i > 10)),
            ~expected=[Next(false), Complete],
            (),
          ),
          observableIt(
            "completes with true on the first observed value that passed the predicate",
            ~nextToString=string_of_int,
            ~source=
              ({now} as scheduler) =>
                Observables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, Next(8)),
                    (2.0, Next(11)),
                    (3.0, Next(14)),
                    (4.0, Next(6)),
                    (5.0, Next(5)),
                    (6.0, Complete),
                  ],
                )
                |> Observable.pipe2(
                     Operators.some(i => i > 10),
                     Operators.map(_ => now() |> int_of_float),
                   ),
            ~expected=[Next(2), Complete],
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
                  Observables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(1)),
                      (10.0, Next(2)),
                      (20.0, Next(3)),
                      (30.0, Next(4)),
                      (40.0, Complete),
                    ],
                  );

                let childObservableB =
                  Observables.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, Complete),
                    ],
                  );

                Observables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, Complete),
                  ],
                )
                |> Observable.lift(Operators.switch_);
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
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe("synchronize", []),
      describe(
        "withLatestFrom",
        [
          observableIt(
            "drops values from the source, if there is no latest value",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observables.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (200.0, Next(2)),
                    (400.0, Next(3)),
                    (600.0, Next(4)),
                    (700.0, Complete),
                  ],
                )
                |> Observable.lift(
                     Operators.withLatestFrom(
                       ~selector=(a, b) => a + b,
                       Observables.ofAbsoluteTimeNotifications(
                         ~scheduler,
                         [
                           (100.0, Next(1)),
                           (250.0, Next(2)),
                           (300.0, Next(3)),
                           (450.0, Next(4)),
                           (500.0, Complete),
                         ],
                       ),
                     ),
                   ),
            ~expected=[Next(3), Next(6), Next(8), Complete],
            (),
          ),
        ],
      ),
    ]
  );