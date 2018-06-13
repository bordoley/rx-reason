open ReUnit;
open ReUnit.Test;

let buffer = observer =>
  observer
  |> Operators.(
       scan((acc, next) => [next, ...acc], []) >> last >> map(List.rev)
     );

let expectToBeEqualToListOfNotifications = (~equals=(===), ~toString) =>
  Expect.toBeEqualToListWith(
    ~equals=
      (a, b) =>
        switch (a, b) {
        | (Notification.Next(a), Notification.Next(b)) => equals(a, b)
        | (Notification.Complete(Some(a)), Notification.Complete(Some(b))) =>
          a === b
        | (Notification.Complete(None), Notification.Complete(None)) => true
        | _ => false
        },
    ~toString=
      v =>
        switch (v) {
        | Notification.Next(v) => "Next(" ++ toString(v) ++ ")"
        | Notification.Complete(Some(_)) => "Complete(Some(exn))"
        | Notification.Complete(None) => "Complete(None)"
        },
  );

let operatorIt =
    (
      name,
      ~equals=(===),
      ~toString,
      ~source: VirtualTimeScheduler.t => Observable.t('a),
      ~operator: VirtualTimeScheduler.t => Operator.t('a, 'b),
      ~expected: list(Notification.t('b)),
      (),
    ) =>
  it(
    name,
    () => {
      let vts = VirtualTimeScheduler.create();

      let subscription = source(vts)
      |> Observable.lift(Operators.(operator(vts) >> materialize >> buffer))
      |> Observable.lift(
           Operators.onNext(
             expectToBeEqualToListOfNotifications(
               ~equals,
               ~toString,
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
            ~toString=string_of_int,
            ~source=
              vts =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=VirtualTimeScheduler.toDelayScheduler(vts),
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 15.0),
                    (Next(5), 16.0),
                    (Next(6), 17.0),
                    (Complete(None), 18.0),
                  ],
                ),
            ~operator=
              vts => {
                let scheduler = vts |> VirtualTimeScheduler.toDelayScheduler;
                Operators.debounce(scheduler(~delay=5.0));
              },
            ~expected=[Next(3), Next(6), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "defaultIfEmpty",
        [
          operatorIt(
            "returns the default if empty",
            ~toString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.defaultIfEmpty(1),
            ~expected=[Next(1), Complete(None)],
            (),
          ),
          operatorIt(
            "passes through if not empty",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.defaultIfEmpty(1),
            ~expected=[Next(1), Next(2), Next(3), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "distinctUntilChanged",
        [
          operatorIt(
            "removes duplicates",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 1, 1, 3, 5, 3, 3, 1]),
            ~operator=_ => Operators.distinctUntilChanged(~equals=(===)),
            ~expected=[
              Next(1),
              Next(3),
              Next(5),
              Next(3),
              Next(1),
              Complete(None),
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
            ~toString=string_of_bool,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.every(i => i > 10),
            ~expected=[Next(true), Complete(None)],
            (),
          ),
          it(
            "it completes with false on the first observed value that fails the predicate",
            () => {
            let observedValue = ref(true);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let everyObserver = observer |> Operators.every(i => i > 10);
            everyObserver |> Observer.next(12);
            observedValue^ |> Expect.toBeEqualToTrue;
            completed^ |> Expect.toBeEqualToFalse;

            everyObserver |> Observer.next(10);
            observedValue^ |> Expect.toBeEqualToFalse;
            completed^ |> Expect.toBeEqualToTrue;

            everyObserver |> Observer.complete(None);
          }),
          operatorIt(
            "it completes with true if all values pass the predicate",
            ~toString=string_of_bool,
            ~source=_ => Observable.ofList([12, 13]),
            ~operator=_ => Operators.every(i => i > 10),
            ~expected=[Next(true), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "exhaust",
        [
          operatorIt(
            "exhausts",
            ~toString=string_of_int,
            ~source=
              vts => {
                let scheduler = vts |> VirtualTimeScheduler.toDelayScheduler;
                let childObservableA =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (Next(1), 0.0),
                      (Next(2), 10.0),
                      (Next(3), 20.0),
                      (Next(4), 30.0),
                      (Complete(None), 40.0),
                    ],
                  );

                let childObservableB =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (Next(5), 0.0),
                      (Next(6), 10.0),
                      (Next(7), 19.0),
                      (Next(8), 30.0),
                      (Complete(None), 40.0),
                    ],
                  );

                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler=
                    ClockScheduler.create(
                      ~getCurrentTime=
                        () => VirtualTimeScheduler.getCurrentTime(vts),
                      vts |> VirtualTimeScheduler.toDelayScheduler,
                    ),
                  [
                    (Next(childObservableA), 0.0),
                    (Next(childObservableB), 15.0),
                    (Next(childObservableA), 35.0),
                    (Next(childObservableB), 60.0),
                    (Complete(None), 75.0),
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
              Complete(None),
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
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 3, 10, 6, 8]),
            ~operator=_ => Operators.find(x => x mod 2 === 0),
            ~expected=[Next(10), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "first",
        [
          operatorIt(
            "publishes the first observed value",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([2, 3]),
            ~operator=_ => Operators.first,
            ~expected=[Next(2), Complete(None)],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=_ => Observable.raise(Division_by_zero),
            ~operator=_ => Operators.first,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          it("completes with exception if no values are produced", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let firstObserver = Operators.first(observer);
            firstObserver |> Observer.complete(None);
            switch (observedExn^) {
            | None => failwith("expected observedExn to be not be None")
            | Some(_) => ()
            };
          }),
        ],
      ),
      describe(
        "firstOrNone",
        [
          operatorIt(
            "publishes Some of the first observed value",
            ~equals=Option.equals,
            ~toString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.ofList([2, 3]),
            ~operator=_ => Operators.firstOrNone,
            ~expected=[Next(Some(2)), Complete(None)],
            (),
          ),
          operatorIt(
            "publishes Some of the first observed value",
            ~equals=Option.equals,
            ~toString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.raise(Division_by_zero),
            ~operator=_ => Operators.firstOrNone,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~equals=Option.equals,
            ~toString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> firstOrNone),
            ~expected=[Next(None), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "ignoreElements",
        [
          operatorIt(
            "ignores all elements and publishes an exception",
            ~toString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  Complete(Some(Division_by_zero)),
                ]),
            ~operator=_ => Operators.ignoreElements,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
        ],
      ),
      describe(
        "isEmpty",
        [
          operatorIt(
            "return false if not empty",
            ~toString=string_of_bool,
            ~source=_ => Observable.ofValue(1),
            ~operator=_ => Operators.isEmpty,
            ~expected=[Next(false), Complete(None)],
            (),
          ),
          operatorIt(
            "return true if empty",
            ~toString=string_of_bool,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.isEmpty,
            ~expected=[Next(true), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "keep",
        [
          operatorIt(
            "completes the observer when the predicate throws an exception",
            ~toString=string_of_int,
            ~source=_ => Observable.ofValue(1),
            ~operator=_ => Operators.keep(_ => raise(Division_by_zero)),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            "completes the observer when the keep observer is completed",
            ~toString=string_of_int,
            ~source=_ => Observable.ofValue(1),
            ~operator=_ => Operators.keep(_ => true),
            ~expected=[Next(1), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "last",
        [
          operatorIt(
            "publishes the last observed value and disposes",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.last,
            ~expected=[Next(3), Complete(None)],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  Complete(Some(Division_by_zero)),
                ]),
            ~operator=_ => Operators.last,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          it("completes with exception if no values are produced", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let lastObserver = Operators.last(observer);
            lastObserver |> Observer.complete(None);
            switch (observedExn^) {
            | None => failwith("expected observedExn to be not be None")
            | Some(_) => ()
            };
          }),
        ],
      ),
      describe(
        "lastOrNone",
        [
          operatorIt(
            "publishes the Some of the last observed value and completes",
            ~equals=Option.equals,
            ~toString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.ofList([2, 3]),
            ~operator=_ => Operators.lastOrNone,
            ~expected=[Next(Some(3)), Complete(None)],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~equals=Option.equals,
            ~toString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Complete(Some(Division_by_zero)),
                ]),
            ~operator=_ => Operators.lastOrNone,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~equals=Option.equals,
            ~toString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> lastOrNone),
            ~expected=[Next(None), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "map",
        [
          operatorIt(
            "completes the observer when the mapper throws an exception",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.map(_ => raise(Division_by_zero)),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            "completes the observer when the mapping observer is completed",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.map(i => i + 1),
            ~expected=[Next(2), Next(3), Next(4), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "mapTo",
        [
          operatorIt(
            "maps any input to value",
            ~toString=Functions.identity,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.mapTo("a"),
            ~expected=[Next("a"), Next("a"), Next("a"), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "maybeFirst",
        [
          operatorIt(
            "publishes the first observed value",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.maybeFirst,
            ~expected=[Next(1), Complete(None)],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=_ => Observable.raise(Division_by_zero),
            ~operator=_ => Operators.maybeFirst,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            "ignores EmptyException and completes normally",
            ~toString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> maybeFirst),
            ~expected=[Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "maybeLast",
        [
          operatorIt(
            "publishes the last observed value",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]),
            ~operator=_ => Operators.maybeLast,
            ~expected=[Next(3), Complete(None)],
            (),
          ),
          operatorIt(
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Complete(Some(Division_by_zero)),
                ]),
            ~operator=_ => Operators.maybeLast,
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            "ignores EmptyException and completes normally",
            ~toString=string_of_int,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.(first >> maybeLast),
            ~expected=[Complete(None)],
            (),
          ),
        ],
      ),
      describe("none", []),
      describe("observe", []),
      describe("observeOn", []),
      describe(
        "onComplete",
        [
          it("calls the side effect function on complete", () => {
            let sideEffectCount = ref(0);

            Observable.ofList([1])
            |> Observable.lift(
                 Operators.(
                   onComplete(_ => sideEffectCount := sideEffectCount^ + 1)
                   >> materialize
                   >> buffer
                 ),
               )
            |> Observable.lift(
                 Operators.onNext(
                   expectToBeEqualToListOfNotifications(
                     ~toString=string_of_int,
                     [Next(1), Complete(None)],
                   ),
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
          it("calls the side effect function on next", () => {
            let sideEffectCount = ref(0);

            Observable.ofList([1, 2])
            |> Observable.lift(
                 Operators.(
                   onNext(_ => sideEffectCount := sideEffectCount^ + 1)
                   >> materialize
                   >> buffer
                 ),
               )
            |> Observable.lift(
                 Operators.onNext(
                   expectToBeEqualToListOfNotifications(
                     ~toString=string_of_int,
                     [Next(1), Complete(None)],
                   ),
                 ),
               )
            |> Observable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(2);
          }),
        ],
      ),
      describe(
        "scan",
        [
          operatorIt(
            "publishes all intermediate values, including the initial accumulator value",
            ~toString=string_of_int,
            ~source=_ => Observable.ofList([2, 3, 4]),
            ~operator=_ => Operators.scan((acc, next) => acc + next, 0),
            ~expected=[
              Next(0),
              Next(2),
              Next(5),
              Next(9),
              Complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "some",
        [
          operatorIt(
            "returns false for an observer that completes without producing values",
            ~toString=string_of_bool,
            ~source=_ => Observable.empty(),
            ~operator=_ => Operators.some(i => i > 10),
            ~expected=[Next(false), Complete(None)],
            (),
          ),
          operatorIt(
            "returns false for an observer for which no value passes the predicate",
            ~toString=string_of_bool,
            ~source=_ => Observable.ofList([5, 6, 7]),
            ~operator=_ => Operators.some(i => i > 10),
            ~expected=[Next(false), Complete(None)],
            (),
          ),
          it(
            "returns true for the first observed value that passed the predicate",
            () => {
            let observedValue = ref(false);
            let completed = ref(false);

            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let someObserver = observer |> Operators.some(i => i > 10);

            someObserver |> Observer.next(5);
            observedValue^ |> Expect.toBeEqualToFalse;
            completed^ |> Expect.toBeEqualToFalse;

            someObserver |> Observer.next(11);
            observedValue^ |> Expect.toBeEqualToTrue;
            completed^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "switch_",
        [
          operatorIt(
            "switches",
            ~toString=string_of_int,
            ~source=
              vts => {
                let scheduler = vts |> VirtualTimeScheduler.toDelayScheduler;
                let childObservableA =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (Next(1), 0.0),
                      (Next(2), 10.0),
                      (Next(3), 20.0),
                      (Next(4), 30.0),
                      (Complete(None), 40.0),
                    ],
                  );

                let childObservableB =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (Next(5), 0.0),
                      (Next(6), 10.0),
                      (Next(7), 19.0),
                      (Next(8), 30.0),
                      (Complete(None), 40.0),
                    ],
                  );

                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler=
                    ClockScheduler.create(
                      ~getCurrentTime=
                        () => VirtualTimeScheduler.getCurrentTime(vts),
                      vts |> VirtualTimeScheduler.toDelayScheduler,
                    ),
                  [
                    (Next(childObservableA), 0.0),
                    (Next(childObservableB), 15.0),
                    (Next(childObservableA), 35.0),
                    (Next(childObservableB), 60.0),
                    (Complete(None), 75.0),
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
              Complete(None),
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
            ~toString=string_of_int,
            ~source=
              vts =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=VirtualTimeScheduler.toDelayScheduler(vts),
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 10.0),
                    (Complete(None), 14.0),
                  ],
                ),
            ~operator=
              vts => {
                let scheduler = VirtualTimeScheduler.toDelayScheduler(vts);
                Operators.timeout(scheduler(~delay=5.0));
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Complete(None),
            ],
            (),
          ),
          operatorIt(
            "when timeout expires",
            ~toString=string_of_int,
            ~source=
              vts =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=VirtualTimeScheduler.toDelayScheduler(vts),
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 15.0),
                    (Complete(None), 20.0),
                  ],
                ),
            ~operator=
              vts => {
                let scheduler = VirtualTimeScheduler.toDelayScheduler(vts);
                Operators.timeout(scheduler(~delay=5.0));
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Complete(Some(Operators.TimeoutException)),
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
            ~toString=string_of_int,
            ~source=
              vts =>
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler=
                    ClockScheduler.create(
                      ~getCurrentTime=
                        () => VirtualTimeScheduler.getCurrentTime(vts),
                      vts |> VirtualTimeScheduler.toDelayScheduler,
                    ),
                  [
                    (Next(1), 0.0),
                    (Next(2), 200.0),
                    (Next(3), 400.0),
                    (Next(4), 600.0),
                    (Complete(None), 700.0),
                  ],
                ),
            ~operator=
              vts => {
                let other =
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler=
                      ClockScheduler.create(
                        ~getCurrentTime=
                          () => VirtualTimeScheduler.getCurrentTime(vts),
                        vts |> VirtualTimeScheduler.toDelayScheduler,
                      ),
                    [
                      (Next(1), 100.0),
                      (Next(2), 250.0),
                      (Next(3), 300.0),
                      (Next(4), 450.0),
                      (Complete(None), 500.0),
                    ],
                  );
                Operators.withLatestFrom(~selector=(a, b) => a + b, other);
              },
            ~expected=[Next(3), Next(6), Next(8), Complete(None)],
            (),
          ),
        ],
      ),
    ],
  );