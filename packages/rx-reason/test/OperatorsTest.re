open ReUnit;

open ReUnit.Test;

let subscribeWithResult = (result, observable) =>
  observable
  |> Observable.subscribeWithCallbacks(
       ~onNext=next => result := [Notification.Next(next), ...result^],
       ~onComplete=exn => result := [Notification.Complete(exn), ...result^],
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
        | Notification.Next(v) => "next: " ++ toString(v)
        | Notification.Complete(Some(_)) => "completed with exception"
        | Notification.Complete(None) => "completed"
        },
  );

let test =
  describe(
    "Operators",
    [
      describe("bufferCount", []),
      describe("bufferTime", []),
      describe("debounce", []),
      describe("defaultIfEmpty", []),
      describe(
        "distinctUntilChanged",
        [
          it("removes duplicates", () => {
            let result = ref([]);

            Observable.ofList([1, 1, 1, 3, 5, 3, 3, 1])
            |> Observable.lift(Operators.distinctUntilChanged)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [
                   Next(1),
                   Next(3),
                   Next(5),
                   Next(3),
                   Next(1),
                   Complete(None),
                 ],
               );
          }),
        ],
      ),
      describe(
        "every",
        [
          it(
            "returns true for an observer that completes without producing values",
            () => {
            let result = ref([]);
            Observable.empty()
            |> Observable.lift(Operators.every(i => i > 10))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_bool,
                 [Next(true), Complete(None)],
               );
          }),
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
          it("it completes with true if all values pass the predicate", () => {
            let result = ref([]);
            Observable.ofList([12, 13])
            |> Observable.lift(Operators.every(i => i > 10))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_bool,
                 [Next(true), Complete(None)],
               );
          }),
        ],
      ),
      describe("exhaust", []),
      describe(
        "find",
        [
          it("finds the first matching element and completes", () => {
            let result = ref([]);
            Observable.ofList([1, 3, 10, 6, 8])
            |> Observable.lift(Operators.find(x => x mod 2 === 0))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(10), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "first",
        [
          it("publishes the first observed value", () => {
            let result = ref([]);
            Observable.ofList([2, 3])
            |> Observable.lift(Operators.first)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(2), Complete(None)],
               );
          }),
          it("passes through completed exceptions", () => {
            let result = ref([]);
            Observable.raise(Division_by_zero)
            |> Observable.lift(Operators.first)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(Some(Division_by_zero))],
               );
          }),
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
          it("publishes Some of the first observed value", () => {
            let result = ref([]);
            Observable.ofList([2, 3])
            |> Observable.lift(Operators.firstOrNone)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~equals=
                   (a, b) =>
                     switch (a, b) {
                     | (Some(a), Some(b)) => a === b
                     | _ => false
                     },
                 ~toString=
                   x =>
                     switch (x) {
                     | Some(v) => "Some(" ++ string_of_int(v) ++ ")"
                     | _ => "None"
                     },
                 [Next(Some(2)), Complete(None)],
               );

            let observedValue = ref(None);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let firstOrNoneObserver = Operators.firstOrNone(observer);
            firstOrNoneObserver |> Observer.next(2);
            observedValue^ |> Expect.toBeEqualToSomeOfInt(2);
            completed^ |> Expect.toBeEqualToTrue;
          }),
          it("passes through completed exceptions", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let firstOrNoneObserver = Operators.firstOrNone(observer);
            firstOrNoneObserver |> Observer.complete(Some(Division_by_zero));
            switch (observedExn^) {
            | None => failwith("expected observedExn to be not be None")
            | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue
            };
          }),
          it(
            "ignores EmptyException, publishes None, and completes normally",
            () => {
            let observedValue = ref(Some(1));
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let firstOrNoneObserver =
              Operators.first @@ Operators.firstOrNone @@ observer;
            firstOrNoneObserver |> Observer.complete(None);
            observedValue^ |> Expect.toBeEqualToNoneOfInt;
          }),
        ],
      ),
      describe(
        "identity",
        [
          it("returns the observer provided as an argument", () => {
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let identityObserver = Operators.identity(observer);
            observer === identityObserver |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "ignoreElements",
        [
          it("ignores all elements and publishes an exception", () => {
            let observedValues = ref([]);
            let completed = ref(None);
            let observer =
              Observer.create(
                ~onNext=next => observedValues := [next, ...observedValues^],
                ~onComplete=exn => completed := exn,
                ~onDispose=Functions.alwaysUnit,
              );

            let ignoreObserver = observer |> Operators.ignoreElements;
            ignoreObserver |> Observer.next(1);
            ignoreObserver |> Observer.next(2);
            ignoreObserver |> Observer.next(3);
            ignoreObserver |> Observer.complete(Some(Division_by_zero));

            observedValues^ === [] |> Expect.toBeEqualToTrue;
            switch (completed^) {
            | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue
            | _ => failwith("expected a division by zero exception")
            };
          }),
        ],
      ),
      describe("isEmpty", []),
      describe(
        "keep",
        [
          it("completes the observer when the mapper throws an exception", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let mapper = _ => raise(Division_by_zero);
            let keepObserver = Operators.keep(mapper, observer);
            keepObserver |> Observer.next(1);
            observedExn^ === None |> Expect.toBeEqualToFalse;
          }),
          it(
            "completes the observer when the mapping observer is completed", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let keepObserver = Operators.keep(_ => true, observer);
            keepObserver
            |> Observer.completeWithResult(Some(Division_by_zero))
            |> Expect.toBeEqualToTrue;
            observedExn^ === None |> Expect.toBeEqualToFalse;
          }),
        ],
      ),
      describe(
        "last",
        [
          it("publishes the last observed value and disposes", () => {
            let observedValue = ref(0);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let lastObserver = Operators.last(observer);
            lastObserver |> Observer.next(2);
            observedValue^ |> Expect.toBeEqualToInt(0);
            completed^ |> Expect.toBeEqualToFalse;
            lastObserver |> Observer.next(3);
            observedValue^ |> Expect.toBeEqualToInt(0);
            completed^ |> Expect.toBeEqualToFalse;
            lastObserver |> Observer.complete(None);
            observedValue^ |> Expect.toBeEqualToInt(3);
            completed^ |> Expect.toBeEqualToTrue;
          }),
          it("passes through completed exceptions", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let lastObserver = Operators.last(observer);
            lastObserver |> Observer.next(2);
            lastObserver |> Observer.next(3);
            lastObserver |> Observer.complete(Some(Division_by_zero));
            switch (observedExn^) {
            | None => failwith("expected observedExn to be not be None")
            | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue
            };
          }),
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
          it("publishes the Some of the last observed value and disposes", () => {
            let observedValue = ref(None);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let lastOrNoneObserver = Operators.lastOrNone(observer);
            lastOrNoneObserver |> Observer.next(3);
            observedValue^ |> Expect.toBeEqualToNoneOfInt;
            lastOrNoneObserver |> Observer.next(2);
            observedValue^ |> Expect.toBeEqualToNoneOfInt;
            lastOrNoneObserver |> Observer.complete(None);
            observedValue^ |> Expect.toBeEqualToSomeOfInt(2);
            completed^ |> Expect.toBeEqualToTrue;
          }),
          it("passes through completed exceptions", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let lastOrNoneObserver = Operators.lastOrNone(observer);
            lastOrNoneObserver |> Observer.complete(Some(Division_by_zero));

            switch (observedExn^) {
            | None => failwith("expected observedExn to be not be None")
            | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue
            };
          }),
          it(
            "ignores EmptyException, publishes None, and completes normally",
            () => {
            let observedValue = ref(Some(1));
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let lastOrNoneObserver =
              Operators.first @@ Operators.lastOrNone @@ observer;
            lastOrNoneObserver |> Observer.complete(None);
            observedValue^ |> Expect.toBeEqualToNoneOfInt;
          }),
        ],
      ),
      describe(
        "map",
        [
          it("completes the observer when the mapper throws an exception", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let mapper = _ => raise(Division_by_zero);
            let mappingObserver = Operators.map(mapper, observer);
            mappingObserver |> Observer.next(1);
            observedExn^ === None |> Expect.toBeEqualToFalse;
          }),
          it(
            "completes the observer when the mapping observer is completed", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let mappingObserver = Operators.map(a => a + 1, observer);
            mappingObserver
            |> Observer.completeWithResult(Some(Division_by_zero))
            |> Expect.toBeEqualToTrue;
            observedExn^ === None |> Expect.toBeEqualToFalse;
          }),
        ],
      ),
      describe(
        "mapTo",
        [
          it("maps any input to value", () => {
            let observedNext = ref("");
            let observer =
              Observer.create(
                ~onNext=next => observedNext := next,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let mappingObserver = Operators.mapTo("a", observer);
            mappingObserver |> Observer.next(1);
            observedNext^ |> Expect.toBeEqualToString("a");
          }),
        ],
      ),
      describe("maybe", []),
      describe(
        "maybeFirst",
        [
          it("publishes the first observed value", () => {
            let observedValue = ref(0);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let maybeFirstObserver = Operators.maybeFirst(observer);
            maybeFirstObserver |> Observer.next(2);
            observedValue^ |> Expect.toBeEqualToInt(2);
            completed^ |> Expect.toBeEqualToTrue;
          }),
          it("passes through completed exceptions", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let maybeFirstObserver = Operators.maybeFirst(observer);
            maybeFirstObserver |> Observer.complete(Some(Division_by_zero));
            switch (observedExn^) {
            | None => failwith("expected observedExn to be not be None")
            | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue
            };
          }),
          it("ignores EmptyException and completes normally", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let maybeFirstObserver =
              Operators.first @@ Operators.maybeFirst @@ observer;
            maybeFirstObserver |> Observer.complete(None);
            observedExn^ === None |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "maybeLast",
        [
          it("publishes the first observed value and disposes", () => {
            let observedValue = ref(0);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let maybeLastObserver = Operators.maybeLast(observer);
            maybeLastObserver |> Observer.next(2);
            observedValue^ |> Expect.toBeEqualToInt(0);
            maybeLastObserver |> Observer.next(4);
            observedValue^ |> Expect.toBeEqualToInt(0);
            maybeLastObserver |> Observer.next(6);
            maybeLastObserver |> Observer.complete(None);
            observedValue^ |> Expect.toBeEqualToInt(6);
            completed^ |> Expect.toBeEqualToTrue;
          }),
          it("passes through completed exceptions", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let maybeLastObserver = Operators.maybeLast(observer);
            maybeLastObserver |> Observer.complete(Some(Division_by_zero));
            switch (observedExn^) {
            | None => failwith("expected observedExn to be not be None")
            | Some(x) => x === Division_by_zero |> Expect.toBeEqualToTrue
            };
          }),
          it("ignores EmptyException and completes normally", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let maybeLastObserver =
              Operators.first @@ Operators.maybeLast @@ observer;
            maybeLastObserver |> Observer.complete(None);
            observedExn^ === None |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe("none", []),
      describe("observe", []),
      describe("observeOn", []),
      describe("onComplete", []),
      describe(
        "onNext",
        [
          it("calls the side effect function on next", () => {
            let sideEffectCalled = ref(false);
            let observedNext = ref(0);
            let observer =
              Observer.create(
                ~onNext=next => observedNext := next,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let doObserver =
              Operators.onNext(_ => sideEffectCalled := true, observer);
            doObserver |> Observer.next(1);
            sideEffectCalled^ |> Expect.toBeEqualToTrue;
            observedNext^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe(
        "scan",
        [
          it(
            "publishes all intermediate values, including the initial accumulator value",
            () => {
            let result = ref([]);
            let observer =
              Observer.create(
                ~onNext=next => result := [next, ...result^],
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let scanObserver =
              Operators.scan((acc, next) => acc + next, 0, observer);
            scanObserver |> Observer.next(2);
            scanObserver |> Observer.next(3);
            scanObserver |> Observer.next(4);

            result^ |> Expect.toBeEqualToListOfInt([9, 5, 2, 0]);
          }),
        ],
      ),
      describe(
        "some",
        [
          it(
            "returns false for an observer that completes without producing values",
            () => {
            let observedValue = ref(true);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let someObserver = observer |> Operators.some(i => i > 10);
            someObserver |> Observer.complete(None);
            observedValue^ |> Expect.toBeEqualToFalse;
            completed^ |> Expect.toBeEqualToTrue;
          }),
          it(
            "returns false for an observer for which no value passes the predicate",
            () => {
            let observedValue = ref(true);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );
            let someObserver = observer |> Operators.some(i => i > 10);

            someObserver |> Observer.next(5);
            observedValue^ |> Expect.toBeEqualToTrue;
            completed^ |> Expect.toBeEqualToFalse;

            someObserver |> Observer.complete(None);
            observedValue^ |> Expect.toBeEqualToFalse;
            completed^ |> Expect.toBeEqualToTrue;
          }),
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
      describe("timeout", []),
      describe(
        "withLatestFrom",
        [
          it("drops values from the source, if there is no latest value", () => {
            let source = Subject.create();
            let latest = Subject.create();

            let result = ref([]);
            source
            |> Subject.toObservable
            |> Observable.lift(
                 Operators.withLatestFrom(
                   ~selector=(a, b) => [a, b],
                   latest |> Subject.toObservable,
                 ),
               )
            |> Observable.subscribeWithCallbacks(
                 ~onNext=next => result := next,
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            source |> Subject.toObserver |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);

            source |> Subject.toObserver |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);

            latest |> Subject.toObserver |> Observer.next(1);
            source |> Subject.toObserver |> Observer.next(3);
            result^ |> Expect.toBeEqualToListOfInt([3, 1]);

            source |> Subject.toObserver |> Observer.next(4);
            result^ |> Expect.toBeEqualToListOfInt([4, 1]);

            latest |> Subject.toObserver |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([4, 1]);

            source |> Subject.toObserver |> Observer.next(5);
            result^ |> Expect.toBeEqualToListOfInt([5, 2]);
          }),
        ],
      ),
    ],
  );