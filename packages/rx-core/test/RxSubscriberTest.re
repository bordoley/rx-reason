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
              RxSubscriber.create(
                ~onNext=RxFunctions.alwaysUnit1, ~onComplete=exn =>
                observedExn := exn
              );

            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToFalse;

            subscriber |> RxSubscriber.complete(~exn=Division_by_zero);

            observedExn^ === None |> Expect.toBeEqualToFalse;
            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToTrue;
          }),
          it("ignores complete request if stopped", () => {
            let observedExn = ref(None);
            let subscriber =
              RxSubscriber.create(
                ~onNext=RxFunctions.alwaysUnit1, ~onComplete=exn =>
                observedExn := exn
              );

            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToFalse;
            subscriber |> RxSubscriber.complete;
            subscriber |> RxSubscriber.complete(~exn=Division_by_zero);

            observedExn^ === None |> Expect.toBeEqualToTrue;
            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "completeWithResult",
        [
          it("returns true if not stopped", () => {
            let subscriber =
              RxSubscriber.create(
                ~onNext=RxFunctions.alwaysUnit1,
                ~onComplete=RxFunctions.alwaysUnit1,
              );

            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToFalse;

            subscriber
            |> RxSubscriber.completeWithResult
            |> Expect.toBeEqualToTrue;
          }),
          it("returns false if stopped", () => {
            let subscriber =
              RxSubscriber.create(
                ~onNext=RxFunctions.alwaysUnit1,
                ~onComplete=RxFunctions.alwaysUnit1,
              );

            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToFalse;
            subscriber |> RxSubscriber.complete;

            subscriber
            |> RxSubscriber.completeWithResult
            |> Expect.toBeEqualToFalse;
          }),
        ],
      ),
      describe(
        "create",
        [
          it(
            "disposes when an error occurs in onNext and rethrows the error",
            () => {
            let subscriber =
              RxSubscriber.create(
                ~onNext=_ => raise(Division_by_zero),
                ~onComplete=RxFunctions.alwaysUnit1,
              );
            (() => subscriber |> RxSubscriber.next(6)) |> Expect.shouldRaise;
            subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "disposes when an error occurs in onComplete and rethrows the error",
            () => {
            let subscriber =
              RxSubscriber.create(
                ~onNext=RxFunctions.alwaysUnit1, ~onComplete=_ =>
                raise(Division_by_zero)
              );
            (() => subscriber |> RxSubscriber.complete) |> Expect.shouldRaise;
            subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("disposes when the subscriber completes", () => {
            let subscriber =
              RxSubscriber.create(
                ~onNext=RxFunctions.alwaysUnit1,
                ~onComplete=RxFunctions.alwaysUnit1,
              );
            subscriber |> RxSubscriber.complete;
            subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "next",
        [
          it("call onNext if not stopped", () => {
            let observedNext = ref(0);
            let subscriber =
              RxSubscriber.create(
                ~onNext=next => observedNext := next,
                ~onComplete=RxFunctions.alwaysUnit1,
              );
            subscriber |> RxSubscriber.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            subscriber |> RxSubscriber.next(6);
            observedNext^ |> Expect.toBeEqualToInt(6);
          }),
          it("will not call onNext if stopped", () => {
            let observedNext = ref(0);
            let subscriber =
              RxSubscriber.create(
                ~onNext=next => observedNext := next,
                ~onComplete=RxFunctions.alwaysUnit1,
              );
            subscriber |> RxSubscriber.next(5);
            observedNext^ |> Expect.toBeEqualToInt(5);
            subscriber |> RxSubscriber.complete;
            subscriber |> RxSubscriber.next(6);
            observedNext^ |> Expect.toBeEqualToInt(5);
          }),
        ],
      ),
      describe(
        "dispose",
        [
          it("stops and disposes the subscriber", () => {
            let subscriber =
              RxSubscriber.create(
                ~onNext=RxFunctions.alwaysUnit1,
                ~onComplete=RxFunctions.alwaysUnit1,
              );
            subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToFalse;
            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToFalse;
            subscriber |> RxSubscriber.asDisposable |> RxDisposable.dispose;
            subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToTrue;
            subscriber |> RxSubscriber.isStopped |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
    ],
  );