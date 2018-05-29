open ReUnit;

open ReUnit.Test;

let test =
  describe(
    "Operators",
    [
      describe("bufferCount", []),
      describe("bufferTime", []),
      describe("debounceTime", []),
      describe("defaultIfEmpty", []),
      describe(
        "dispose",
        [
          it("call the dispose function when the observer is disposes", () => {
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let disposedCalled = ref(false);
            let disposable = Disposable.create(() => disposedCalled := true);
            let disposingObserver = observer |> Operators.dispose(disposable);
            disposingObserver |> Observer.dispose;
            disposedCalled^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "distinctUntilChanged",
        [
          it("removes duplicates", () => {
            let result = ref([]);
            let observer =
              Observer.create(
                ~onNext=next => result := [next, ...result^],
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            let distinctObserver = Operators.distinctUntilChanged(observer);
            distinctObserver |> Observer.next(1);
            distinctObserver |> Observer.next(1);
            distinctObserver |> Observer.next(1);
            distinctObserver |> Observer.next(3);
            distinctObserver |> Observer.next(5);
            distinctObserver |> Observer.next(3);
            distinctObserver |> Observer.next(3);
            distinctObserver |> Observer.next(1);

            result^ |> Expect.toBeEqualToListOfInt([1, 3, 5, 3, 1]);
          }),
        ],
      ),
      describe("every", []),
      describe("exhaust", []),
      describe(
        "find",
        [
          it("finds the first matching element and completes", () => {
            let observedValue = ref(0);
            let completed = ref(false);
            let observer =
              Observer.create(
                ~onNext=next => observedValue := next,
                ~onComplete=_ => completed := true,
                ~onDispose=Functions.alwaysUnit,
              );

            let findObserver = observer |> Operators.find(x => x mod 2 === 0);
            findObserver |> Observer.next(1);
            observedValue^ |> Expect.toBeEqualToInt(0);
            completed^ |> Expect.toBeEqualToFalse;

            findObserver |> Observer.next(3);
            observedValue^ |> Expect.toBeEqualToInt(0);
            completed^ |> Expect.toBeEqualToFalse;
            findObserver |> Observer.next(10);
            observedValue^ |> Expect.toBeEqualToInt(10);
            completed^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "first",
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
            let firstObserver = Operators.first(observer);
            firstObserver |> Observer.next(2);
            firstObserver |> Observer.next(3);
            observedValue^ |> Expect.toBeEqualToInt(2);
            completed^ |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            firstObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it("passes through completed exceptions", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );
            let firstObserver = Operators.first(observer);
            firstObserver |> Observer.complete(Some(Division_by_zero));
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            firstObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            let firstObserver = Operators.first(observer);
            firstObserver |> Observer.complete(None);
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            firstObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
          it("publishes Some of the first observed value and disposes", () => {
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            firstOrNoneObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            firstOrNoneObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            firstOrNoneObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            keepObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            keepObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            lastObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            lastObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            lastObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            lastOrNoneObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            lastOrNoneObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            lastOrNoneObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            mappingObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            mappingObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
          it("publishes the first observed value and disposes", () => {
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            maybeFirstObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            maybeFirstObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            maybeFirstObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            maybeLastObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            maybeLastObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            maybeLastObserver
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
            observedExn^ === None |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe("none", []),
      describe("observe", []),
      describe("observeOn", []),
      describe("onComplete", []),
      describe("onDispose", []),
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
      describe("some", []),
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