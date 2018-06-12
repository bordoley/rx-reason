open ReUnit;

open ReUnit.Test;

let optionEquals = (~equals=(===), a, b) =>
  switch (a, b) {
  | (Some(a), Some(b)) => equals(a, b)
  | (None, None) => true
  | _ => false
  };

let optionToString = (~toString, x) =>
  switch (x) {
  | Some(v) => "Some(" ++ toString(v) ++ ")"
  | _ => "None"
  };

let subscribeWithResult = (result, observable) =>
  observable
  |> Observable.subscribeWithCallbacks(
       ~onNext=next => result := [Notification.Next(next), ...result^],
       ~onComplete=exn => result := [Notification.Complete(exn), ...result^],
     );

let subscribeOnVTSWithResult =
    (f: VirtualTimeScheduler.t => Observable.t('a))
    : list(Notification.t('a)) => {
  let vts = VirtualTimeScheduler.create();
  let result = ref([]);

  f(vts) |> subscribeWithResult(result) |> ignore;

  vts |> VirtualTimeScheduler.run;
  result^ |> List.rev;
};

let subscribeSynchronousWithResult =
    (source: Observable.t('a))
    : list(Notification.t('a)) => {
  let result = ref([]);
  source |> subscribeWithResult(result) |> ignore;

  result^ |> List.rev;
};
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
      operator: Operator.t('a, 'b),
      name,
      ~equals=(===),
      ~toString,
      ~source: Observable.t('a),
      ~expected: list(Notification.t('b)),
      (),
    ) =>
  it(name, () =>
    source
    |> Observable.lift(operator)
    |> subscribeSynchronousWithResult
    |> expectToBeEqualToListOfNotifications(~equals, ~toString, expected)
  );

let test =
  describe(
    "Operators",
    [
      describe("bufferCount", []),
      describe("bufferTime", []),
      describe(
        "debounce",
        [
          it("debounces", () =>
            subscribeOnVTSWithResult(vts => {
              let scheduler = vts |> VirtualTimeScheduler.toDelayScheduler;
              let source =
                Observable.ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 15.0),
                    (Next(5), 16.0),
                    (Next(6), 17.0),
                    (Complete(None), 18.0),
                  ],
                );
              source
              |> Observable.lift(Operators.debounce(scheduler(~delay=5.0)));
            })
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(3), Next(6), Complete(None)],
               )
          ),
        ],
      ),
      describe(
        "defaultIfEmpty",
        [
          operatorIt(
            Operators.defaultIfEmpty(1),
            "returns the default if empty",
            ~toString=string_of_int,
            ~source=Observable.empty(),
            ~expected=[Next(1), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.defaultIfEmpty(1),
            "passes through if not empty",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 2, 3]),
            ~expected=[Next(1), Next(2), Next(3), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "distinctUntilChanged",
        [
          operatorIt(
            Operators.distinctUntilChanged,
            "removes duplicates",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 1, 1, 3, 5, 3, 3, 1]),
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
            Operators.every(i => i > 10),
            "returns true for an observer that completes without producing values",
            ~toString=string_of_bool,
            ~source=Observable.empty(),
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
            Operators.every(i => i > 10),
            "it completes with true if all values pass the predicate",
            ~toString=string_of_bool,
            ~source=Observable.ofList([12, 13]),
            ~expected=[Next(true), Complete(None)],
            (),
          ),
        ],
      ),
      describe("exhaust", []),
      describe(
        "find",
        [
          operatorIt(
            Operators.find(x => x mod 2 === 0),
            "finds the first matching element and completes",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 3, 10, 6, 8]),
            ~expected=[Next(10), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "first",
        [
          operatorIt(
            Operators.first,
            "publishes the first observed value",
            ~toString=string_of_int,
            ~source=Observable.ofList([2, 3]),
            ~expected=[Next(2), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.first,
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=Observable.raise(Division_by_zero),
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
            Operators.firstOrNone,
            "publishes Some of the first observed value",
            ~equals=optionEquals,
            ~toString=optionToString(~toString=string_of_int),
            ~source=Observable.ofList([2, 3]),
            ~expected=[Next(Some(2)), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.firstOrNone,
            "publishes Some of the first observed value",
            ~equals=optionEquals,
            ~toString=optionToString(~toString=string_of_int),
            ~source=Observable.raise(Division_by_zero),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            observer => Operators.first @@ Operators.firstOrNone @@ observer,
            "ignores EmptyException, publishes None, and completes normally",
            ~equals=optionEquals,
            ~toString=optionToString(~toString=string_of_int),
            ~source=Observable.empty(),
            ~expected=[Next(None), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "ignoreElements",
        [
          operatorIt(
            Operators.ignoreElements,
            "ignores all elements and publishes an exception",
            ~toString=string_of_int,
            ~source=
              Observable.ofNotifications([
                Next(1),
                Next(2),
                Next(3),
                Complete(Some(Division_by_zero)),
              ]),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
        ],
      ),
      describe("isEmpty", [
        operatorIt(
          Operators.isEmpty,
          "return false if not empty",
          ~toString=string_of_bool,
          ~source=Observable.ofValue(1),
          ~expected=[Next(false), Complete(None)],
          (),
        ),
        operatorIt(
          Operators.isEmpty,
          "return true if empty",
          ~toString=string_of_bool,
          ~source=Observable.empty(),
          ~expected=[Next(true), Complete(None)],
          (),
        ),
      ]),
      describe(
        "keep",
        [
          operatorIt(
            Operators.keep(_ => raise(Division_by_zero)),
            "completes the observer when the predicate throws an exception",
            ~toString=string_of_int,
            ~source=Observable.ofValue(1),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            Operators.keep(_ => true),
            "completes the observer when the keep observer is completed",
            ~toString=string_of_int,
            ~source=Observable.ofValue(1),
            ~expected=[Next(1), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "last",
        [
          operatorIt(
            Operators.last,
            "publishes the last observed value and disposes",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 2, 3]),
            ~expected=[Next(3), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.last,
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=
              Observable.ofNotifications([
                Next(1),
                Next(2),
                Next(3),
                Complete(Some(Division_by_zero)),
              ]),
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
            Operators.lastOrNone,
            "publishes the Some of the last observed value and completes",
            ~equals=optionEquals,
            ~toString=optionToString(~toString=string_of_int),
            ~source=Observable.ofList([2, 3]),
            ~expected=[Next(Some(3)), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.lastOrNone,
            "passes through completed exceptions",
            ~equals=optionEquals,
            ~toString=optionToString(~toString=string_of_int),
            ~source=
              Observable.ofNotifications([
                Next(1),
                Next(2),
                Complete(Some(Division_by_zero)),
              ]),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            observer => Operators.first @@ Operators.lastOrNone @@ observer,
            "ignores EmptyException, publishes None, and completes normally",
            ~equals=optionEquals,
            ~toString=optionToString(~toString=string_of_int),
            ~source=Observable.empty(),
            ~expected=[Next(None), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "map",
        [
          operatorIt(
            Operators.map(_ => raise(Division_by_zero)),
            "completes the observer when the mapper throws an exception",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 2, 3]),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            Operators.map(i => i + 1),
            "completes the observer when the mapping observer is completed",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 2, 3]),
            ~expected=[Next(2), Next(3), Next(4), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "mapTo",
        [
          operatorIt(
            Operators.mapTo("a"),
            "maps any input to value",
            ~toString=Functions.identity,
            ~source=Observable.ofList([1, 2, 3]),
            ~expected=[Next("a"), Next("a"), Next("a"), Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "maybeFirst",
        [
          operatorIt(
            Operators.maybeFirst,
            "publishes the first observed value",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 2, 3]),
            ~expected=[Next(1), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.maybeFirst,
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=Observable.raise(Division_by_zero),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            observer => Operators.first @@ Operators.maybeFirst @@ observer,
            "ignores EmptyException and completes normally",
            ~toString=string_of_int,
            ~source=Observable.empty(),
            ~expected=[Complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "maybeLast",
        [
          operatorIt(
            Operators.maybeLast,
            "publishes the last observed value",
            ~toString=string_of_int,
            ~source=Observable.ofList([1, 2, 3]),
            ~expected=[Next(3), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.maybeLast,
            "passes through completed exceptions",
            ~toString=string_of_int,
            ~source=
              Observable.ofNotifications([
                Next(1),
                Next(2),
                Complete(Some(Division_by_zero)),
              ]),
            ~expected=[Complete(Some(Division_by_zero))],
            (),
          ),
          operatorIt(
            observer => Operators.first @@ Operators.maybeLast @@ observer,
            "ignores EmptyException and completes normally",
            ~toString=string_of_int,
            ~source=Observable.empty(),
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
            let result = ref([]);
            let sideEffectCount = ref(0);

            Observable.ofList([1])
            |> Observable.lift(
                 Operators.onComplete(_ =>
                   sideEffectCount := sideEffectCount^ + 1
                 ),
               )
            |> subscribeWithResult(result)
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(1), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "onNext",
        [
          it("calls the side effect function on next", () => {
            let result = ref([]);
            let sideEffectCount = ref(0);

            Observable.ofList([1])
            |> Observable.lift(
                 Operators.onNext(_ =>
                   sideEffectCount := sideEffectCount^ + 1
                 ),
               )
            |> subscribeWithResult(result)
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(1), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "scan",
        [
          operatorIt(
            Operators.scan((acc, next) => acc + next, 0),
            "publishes all intermediate values, including the initial accumulator value",
            ~toString=string_of_int,
            ~source=Observable.ofList([2, 3, 4]),
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
            Operators.some(i => i > 10),
            "returns false for an observer that completes without producing values",
            ~toString=string_of_bool,
            ~source=Observable.empty(),
            ~expected=[Next(false), Complete(None)],
            (),
          ),
          operatorIt(
            Operators.some(i => i > 10),
            "returns false for an observer for which no value passes the predicate",
            ~toString=string_of_bool,
            ~source=Observable.ofList([5, 6, 7]),
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
          it("switches", () => {
            let result = ref([]);
            let observer =
              Observer.create(
                ~onNext=next => result := [next, ...result^],
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );

            let switchObserver = Operators.switch_(observer);

            let subject0 = Subject.create();
            let subject1 = Subject.create();
            subject0 |> Subject.toObserver |> Observer.next(1);
            subject1 |> Subject.toObserver |> Observer.next(2);
            switchObserver |> Observer.next(subject0 |> Subject.toObservable);
            subject0 |> Subject.toObserver |> Observer.next(3);
            subject1 |> Subject.toObserver |> Observer.next(4);
            switchObserver |> Observer.next(subject1 |> Subject.toObservable);
            subject0 |> Subject.toObserver |> Observer.next(5);
            subject1 |> Subject.toObserver |> Observer.next(6);
            result^ |> Expect.toBeEqualToListOfInt([6, 3]);
          }),
        ],
      ),
      describe("synchronize", []),
      describe(
        "timeout",
        [
          it("when timeout does not expire", () =>
            subscribeOnVTSWithResult(vts => {
              let scheduler = vts |> VirtualTimeScheduler.toDelayScheduler;
              let source =
                Observable.ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 10.0),
                    (Complete(None), 14.0),
                  ],
                );
              source
              |> Observable.lift(Operators.timeout(scheduler(~delay=5.0)));
            })
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(1), Next(2), Next(3), Next(4), Complete(None)],
               )
          ),
          it("when timeout expires", () =>
            subscribeOnVTSWithResult(vts => {
              let scheduler = vts |> VirtualTimeScheduler.toDelayScheduler;
              let source =
                Observable.ofRelativeTimeNotifications(
                  ~scheduler,
                  [
                    (Next(1), 0.0),
                    (Next(2), 4.0),
                    (Next(3), 6.0),
                    (Next(4), 15.0),
                    (Complete(None), 20.0),
                  ],
                );
              source
              |> Observable.lift(Operators.timeout(scheduler(~delay=5.0)));
            })
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [
                   Next(1),
                   Next(2),
                   Next(3),
                   Complete(Some(Operators.TimeoutException)),
                 ],
               )
          ),
        ],
      ),
      describe(
        "withLatestFrom",
        [
          it("drops values from the source, if there is no latest value", () =>
            subscribeOnVTSWithResult(vts => {
              let scheduler = vts |> VirtualTimeScheduler.toClockScheduler;
              let source =
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (Next(1), 0.0),
                    (Next(2), 200.0),
                    (Next(3), 400.0),
                    (Next(4), 600.0),
                    (Complete(None), 700.0),
                  ],
                );
              let other =
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (Next(1), 100.0),
                    (Next(2), 250.0),
                    (Next(3), 300.0),
                    (Next(4), 450.0),
                    (Complete(None), 500.0),
                  ],
                );
              source
              |> Observable.lift(
                   Operators.withLatestFrom(
                     ~selector=(a, b) => a + b,
                     other,
                   ),
                 );
            })
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(3), Next(6), Next(8), Complete(None)],
               )
          ),
        ],
      ),
    ],
  );