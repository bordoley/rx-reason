open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "Observer",
    [
      describe(
        "complete",
        [
          it("will complete and stop the observer if not stopped", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );

            observer |> Observer.isStopped |> Expect.toBeEqualToFalse;

            observer |> Observer.complete(~exn=Division_by_zero);

            observedExn^ === None |> Expect.toBeEqualToFalse;
            observer |> Observer.isStopped |> Expect.toBeEqualToTrue;
          }),
          it("ignores complete request if stopped", () => {
            let observedExn = ref(None);
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=exn => observedExn := exn,
                ~onDispose=Functions.alwaysUnit,
              );

            observer |> Observer.isStopped |> Expect.toBeEqualToFalse;
            observer |> Observer.complete;
            observer |> Observer.complete(~exn=Division_by_zero);

            observedExn^ === None |> Expect.toBeEqualToTrue;
            observer |> Observer.isStopped |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "completeWithResult",
        [
          it("returns true if not stopped", () => {
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );

            observer |> Observer.isStopped |> Expect.toBeEqualToFalse;

            observer
            |> Observer.completeWithResult
            |> Expect.toBeEqualToTrue;
          }),
          it("returns false if stopped", () => {
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );

            observer |> Observer.isStopped |> Expect.toBeEqualToFalse;
            observer |> Observer.complete;

            observer
            |> Observer.completeWithResult
            |> Expect.toBeEqualToFalse;
          }),
        ],
      ),
      describe(
        "createAutoDisposing",
        [
          it(
            "disposes when an error occurs in onNext and rethrows the error",
            () => {
            let observer =
              Observer.createAutoDisposing(
                ~onNext=_ => raise(Division_by_zero),
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            (() => observer |> Observer.next(6)) |> Expect.shouldRaise;
            observer |> Observer.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "disposes when an error occurs in onComplete and rethrows the error",
            () => {
            let observer =
              Observer.createAutoDisposing(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=_ => raise(Division_by_zero),
                ~onDispose=Functions.alwaysUnit,
              );
            (() => observer |> Observer.complete) |> Expect.shouldRaise;
            observer |> Observer.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("disposes when the observer completes", () => {
            let observer =
              Observer.createAutoDisposing(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            observer |> Observer.complete;
            observer |> Observer.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "next",
        [
          it("call onNext if not stopped", () => {
            let observedNext = ref(0);
            let observer =
              Observer.create(
                ~onNext=next => observedNext := next,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            observer |> Observer.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            observer |> Observer.next(6);
            observedNext^ |> Expect.toBeEqualToInt(6);
          }),
          it("will not call onNext if stopped", () => {
            let observedNext = ref(0);
            let observer =
              Observer.create(
                ~onNext=next => observedNext := next,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            observer |> Observer.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            observer |> Observer.complete;
            observer |> Observer.next(6);
            observedNext^ |> Expect.toBeEqualToInt(5);
          }),
        ],
      ),
      describe(
        "dispose",
        [
          it("stops and disposes the observer", () => {
            let observer =
              Observer.create(
                ~onNext=Functions.alwaysUnit,
                ~onComplete=Functions.alwaysUnit,
                ~onDispose=Functions.alwaysUnit,
              );
            observer |> Observer.isDisposed |> Expect.toBeEqualToFalse;
            observer |> Observer.isStopped |> Expect.toBeEqualToFalse;
            observer |> Observer.asDisposable |> Disposable.dispose;
            observer |> Observer.isDisposed |> Expect.toBeEqualToTrue;
            observer |> Observer.isStopped |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
    ],
  );