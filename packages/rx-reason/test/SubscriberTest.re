open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "Subscriber",
    [
      describe(
        "complete",
        [
          it("will complete and stop the subscriber if not stopped", () => {
            let observedExn = ref(None);
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=Functions.alwaysUnit1,
                ~onComplete=exn => observedExn := exn,
              );

            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToFalse;

            subscriber |> Subscriber.complete(~exn=Division_by_zero);

            observedExn^ === None |> Expect.toBeEqualToFalse;
            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToTrue;
          }),
          it("ignores complete request if stopped", () => {
            let observedExn = ref(None);
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=Functions.alwaysUnit1,
                ~onComplete=exn => observedExn := exn,
              );

            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToFalse;
            subscriber |> Subscriber.complete;
            subscriber |> Subscriber.complete(~exn=Division_by_zero);

            observedExn^ === None |> Expect.toBeEqualToTrue;
            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "completeWithResult",
        [
          it("returns true if not stopped", () => {
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=Functions.alwaysUnit1,
                ~onComplete=Functions.alwaysUnit1,
              );

            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToFalse;

            subscriber
            |> Subscriber.completeWithResult
            |> Expect.toBeEqualToTrue;
          }),
          it("returns false if stopped", () => {
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=Functions.alwaysUnit1,
                ~onComplete=Functions.alwaysUnit1,
              );

            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToFalse;
            subscriber |> Subscriber.complete;

            subscriber
            |> Subscriber.completeWithResult
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
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=_ => raise(Division_by_zero),
                ~onComplete=Functions.alwaysUnit1,
              );
            (() => subscriber |> Subscriber.next(6)) |> Expect.shouldRaise;
            subscriber |> Subscriber.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "disposes when an error occurs in onComplete and rethrows the error",
            () => {
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=Functions.alwaysUnit1,
                ~onComplete=_ => raise(Division_by_zero),
              );
            (() => subscriber |> Subscriber.complete) |> Expect.shouldRaise;
            subscriber |> Subscriber.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("disposes when the subscriber completes", () => {
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=Functions.alwaysUnit1,
                ~onComplete=Functions.alwaysUnit1,
              );
            subscriber |> Subscriber.complete;
            subscriber |> Subscriber.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "next",
        [
          it("call onNext if not stopped", () => {
            let observedNext = ref(0);
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=next => observedNext := next,
                ~onComplete=Functions.alwaysUnit1,
              );
            subscriber |> Subscriber.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            subscriber |> Subscriber.next(6);
            observedNext^ |> Expect.toBeEqualToInt(6);
          }),
          it("will not call onNext if stopped", () => {
            let observedNext = ref(0);
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=next => observedNext := next,
                ~onComplete=Functions.alwaysUnit1,
              );
            subscriber |> Subscriber.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            subscriber |> Subscriber.complete;
            subscriber |> Subscriber.next(6);
            observedNext^ |> Expect.toBeEqualToInt(5);
          }),
        ],
      ),
      describe(
        "dispose",
        [
          it("stops and disposes the subscriber", () => {
            let subscriber =
              Subscriber.createAutoDisposing(
                ~onNext=Functions.alwaysUnit1,
                ~onComplete=Functions.alwaysUnit1,
              );
            subscriber |> Subscriber.isDisposed |> Expect.toBeEqualToFalse;
            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToFalse;
            subscriber |> Subscriber.asDisposable |> Disposable.dispose;
            subscriber |> Subscriber.isDisposed |> Expect.toBeEqualToTrue;
            subscriber |> Subscriber.isStopped |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
    ],
  );