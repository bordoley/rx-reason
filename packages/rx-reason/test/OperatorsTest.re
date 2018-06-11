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
                 ~equals=optionEquals,
                 ~toString=optionToString(~toString=string_of_int),
                 [Next(Some(2)), Complete(None)],
               );
          }),
          it("passes through completed exceptions", () => {
            let result = ref([]);
            Observable.raise(Division_by_zero)
            |> Observable.lift(Operators.firstOrNone)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~equals=optionEquals,
                 ~toString=optionToString(~toString=string_of_int),
                 [Complete(Some(Division_by_zero))],
               );
          }),
          it(
            "ignores EmptyException, publishes None, and completes normally",
            () => {
            let result = ref([]);
            Observable.empty()
            |> Observable.lift(observer =>
                 Operators.first @@ Operators.firstOrNone @@ observer
               )
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~equals=optionEquals,
                 ~toString=optionToString(~toString=string_of_int),
                 [Next(None), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "ignoreElements",
        [
          it("ignores all elements and publishes an exception", () => {
            let result = ref([]);
            Observable.ofNotifications([
              Next(1),
              Next(2),
              Next(3),
              Complete(Some(Division_by_zero)),
            ])
            |> Observable.lift(Operators.ignoreElements)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(Some(Division_by_zero))],
               );
          }),
        ],
      ),
      describe("isEmpty", []),
      describe(
        "keep",
        [
          it(
            "completes the observer when the predicate throws an exception", () => {
            let predicate = _ => raise(Division_by_zero);

            let result = ref([]);
            Observable.ofValue(1)
            |> Observable.lift(Operators.keep(predicate))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(Some(Division_by_zero))],
               );
          }),
          it("completes the observer when the keep observer is completed", () => {
            let predicate = _ => true;

            let result = ref([]);
            Observable.ofValue(1)
            |> Observable.lift(Operators.keep(predicate))
            |> subscribeWithResult(result)
            |> ignore;

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
        "last",
        [
          it("publishes the last observed value and disposes", () => {
            let result = ref([]);
            Observable.ofList([1, 2, 3])
            |> Observable.lift(Operators.last)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(3), Complete(None)],
               );
          }),
          it("passes through completed exceptions", () => {
            let result = ref([]);
            Observable.ofNotifications([
              Next(1),
              Next(2),
              Next(3),
              Complete(Some(Division_by_zero)),
            ])
            |> Observable.lift(Operators.last)
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
            let result = ref([]);
            Observable.ofList([2, 3])
            |> Observable.lift(Operators.lastOrNone)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~equals=optionEquals,
                 ~toString=optionToString(~toString=string_of_int),
                 [Next(Some(3)), Complete(None)],
               );
          }),
          it("passes through completed exceptions", () => {
            let result = ref([]);
            Observable.ofNotifications([
              Next(1),
              Next(2),
              Complete(Some(Division_by_zero)),
            ])
            |> Observable.lift(Operators.lastOrNone)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~equals=optionEquals,
                 ~toString=optionToString(~toString=string_of_int),
                 [Complete(Some(Division_by_zero))],
               );
          }),
          it(
            "ignores EmptyException, publishes None, and completes normally",
            () => {
            let result = ref([]);
            Observable.empty()
            |> Observable.lift(observer =>
                 Operators.first @@ Operators.lastOrNone @@ observer
               )
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~equals=optionEquals,
                 ~toString=optionToString(~toString=string_of_int),
                 [Next(None), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "map",
        [
          it("completes the observer when the mapper throws an exception", () => {
            let result = ref([]);
            let mapper = _ => raise(Division_by_zero);
            Observable.ofList([1, 2, 3])
            |> Observable.lift(Operators.map(mapper))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(Some(Division_by_zero))],
               );
          }),
          it(
            "completes the observer when the mapping observer is completed", () => {
            let result = ref([]);
            let mapper = i => i + 1;
            Observable.ofList([1, 2, 3])
            |> Observable.lift(Operators.map(mapper))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(2), Next(3), Next(4), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "mapTo",
        [
          it("maps any input to value", () => {
            let result = ref([]);
            Observable.ofList([1, 2, 3])
            |> Observable.lift(Operators.mapTo("a"))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=Functions.identity,
                 [Next("a"), Next("a"), Next("a"), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "maybeFirst",
        [
          it("publishes the first observed value", () => {
            let result = ref([]);
            Observable.ofList([1, 2, 3])
            |> Observable.lift(Operators.maybeFirst)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(1), Complete(None)],
               );
          }),
          it("passes through completed exceptions", () => {
            let result = ref([]);
            Observable.raise(Division_by_zero)
            |> Observable.lift(Operators.maybeFirst)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(Some(Division_by_zero))],
               );
          }),
          it("ignores EmptyException and completes normally", () => {
            let result = ref([]);
            Observable.empty()
            |> Observable.lift(observer =>
                 Operators.first @@ Operators.maybeFirst @@ observer
               )
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "maybeLast",
        [
          it("publishes the last observed value", () => {
            let result = ref([]);
            Observable.ofList([1, 2, 3])
            |> Observable.lift(Operators.maybeLast)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(3), Complete(None)],
               );
          }),
          it("passes through completed exceptions", () => {
            let result = ref([]);
            Observable.ofNotifications([
              Next(1),
              Next(2),
              Complete(Some(Division_by_zero)),
            ])
            |> Observable.lift(Operators.maybeLast)
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(Some(Division_by_zero))],
               );
          }),
          it("ignores EmptyException and completes normally", () => {
            let result = ref([]);
            Observable.empty()
            |> Observable.lift(observer =>
                 Operators.first @@ Operators.maybeLast @@ observer
               )
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Complete(None)],
               );
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
          it(
            "publishes all intermediate values, including the initial accumulator value",
            () => {
            let result = ref([]);
            Observable.ofList([2, 3, 4])
            |> Observable.lift(
                 Operators.scan((acc, next) => acc + next, 0),
               )
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_int,
                 [Next(0), Next(2), Next(5), Next(9), Complete(None)],
               );
          }),
        ],
      ),
      describe(
        "some",
        [
          it(
            "returns false for an observer that completes without producing values",
            () => {
            let result = ref([]);
            Observable.empty()
            |> Observable.lift(Operators.some(i => i > 10))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_bool,
                 [Next(false), Complete(None)],
               );
          }),
          it(
            "returns false for an observer for which no value passes the predicate",
            () => {
            let result = ref([]);
            Observable.ofList([5, 6, 7])
            |> Observable.lift(Operators.some(i => i > 10))
            |> subscribeWithResult(result)
            |> ignore;

            result^
            |> List.rev
            |> expectToBeEqualToListOfNotifications(
                 ~toString=string_of_bool,
                 [Next(false), Complete(None)],
               );
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