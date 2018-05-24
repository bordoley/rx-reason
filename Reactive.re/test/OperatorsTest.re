open ReUnit;

open ReUnit.Test;

let test =
  describe(
    "Operators",
    [
      describe(
        "identity",
        [
          it("returns the observer provided as an argument", () => {
            let observer = Observer.create();
            let identityObserver = Operators.identity(observer);
            observer === identityObserver |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "map",
        [
          it("completes the observer when the mapper throws an exception", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            let mapper = (_) => raise(Division_by_zero);
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
              Observer.create(~onComplete=exn => observedExn := exn, ());
            let mappingObserver = Operators.map(a => a + 1, observer);
            mappingObserver |> Observer.completeWithResult(~exn=Some(Division_by_zero)) |> Expect.toBeEqualToTrue;
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
              Observer.create(~onNext=next => observedNext := next, ());
            let mappingObserver = Operators.mapTo("a", observer);
            mappingObserver |> Observer.next(1);
            observedNext^ |> Expect.toBeEqualToString("a");
          }),
        ],
      ),
      describe(
        "doOnNext",
        [
          it("calls the side effect function on next", () => {
            let sideEffectCalled = ref(false);
            let observedNext = ref(0);
            let observer =
              Observer.create(~onNext=next => observedNext := next, ());
            let doObserver =
              Operators.doOnNext((_) => sideEffectCalled := true, observer);
            doObserver |> Observer.next(1);
            sideEffectCalled^ |> Expect.toBeEqualToTrue;
            observedNext^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe("keep", []),
      describe("first", []),
      describe("firstOrNone", []),
      describe("maybeFirst", []),
      describe("last", []),
      describe("lastOrNone", []),
      describe("maybeLast", []),
      describe("scan", []),
      describe("distinctUntilChanged", []),
      describe("switch_", []),
      describe("switchMap", []),
      describe("debounceTime", []),
    ],
  );