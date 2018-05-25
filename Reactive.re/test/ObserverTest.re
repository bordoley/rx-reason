open ReUnit;

open ReUnit.Test;

let test =
  describe(
    "Observer",
    [
      describe(
        "next",
        [
          it("call onNext if not disposed", () => {
            let observedNext = ref(0);
            let observer =
              Observer.create(~onNext=next => observedNext := next, ());
            observer |> Observer.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            observer |> Observer.next(6);
            observedNext^ |> Expect.toBeEqualToInt(6);
          }),
          it("will not call onNext if disposed", () => {
            let observedNext = ref(0);
            let observer =
              Observer.create(~onNext=next => observedNext := next, ());
            observer |> Observer.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            observer |> Observer.toDisposable |> Disposable.dispose;
            observer |> Observer.next(6);
            observedNext^ |> Expect.toBeEqualToInt(5);
          }),
          it("disposes the observer if onNext throws", () => {
            let observer =
              Observer.create(~onNext=_ => raise(Division_by_zero), ());
            Expect.shouldRaise(() => observer |> Observer.next(4));
            observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "complete",
        [
          it("will complete and dispose the observer if not dispose", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToFalse;
            observer |> Observer.complete(~exn=Some(Division_by_zero));
            observedExn^ === None |> Expect.toBeEqualToFalse;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it("disposes the observer if onComplete throws", () => {
            let observer =
              Observer.create(~onComplete=_ => raise(Division_by_zero), ());
            Expect.shouldRaise(() => observer |> Observer.complete);
            observer |> Observer.toDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "completeWithResult",
        [
          it("will complete and dispose the observer if not dispose", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToFalse;
            let result =
              observer
              |> Observer.completeWithResult(~exn=Some(Division_by_zero));
            result |> Expect.toBeEqualToTrue;
            observedExn^ === None |> Expect.toBeEqualToFalse;
            observer
            |> Observer.toDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it("will not call onComplete if disposed", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(~onComplete=exn => observedExn := exn, ());
            observer |> Observer.toDisposable |> Disposable.dispose;
            let result =
              observer
              |> Observer.completeWithResult(~exn=Some(Division_by_zero));
            result |> Expect.toBeEqualToFalse;
            observedExn^
            |> Expect.toBeEqualToNoneWith(~toString=(_) => "exception");
          }),
        ],
      ),
    ],
  );