open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "Subscriber",
    [
      describe(
        "completeWithResult",
        [
          it("returns true if not stopped", () => {
            let subscriber =
              RxSubscriber.create();

            subscriber
            |> RxSubscriber.completeWithResult
            |> Expect.toBeEqualToTrue;
          }),
          it("returns false if stopped", () => {
            let subscriber =
              RxSubscriber.create();

            subscriber |> RxSubscriber.complete;

            subscriber
            |> RxSubscriber.completeWithResult
            |> Expect.toBeEqualToFalse;
          }),
        ],
      ),
      describe(
        "dispose",
        [
          it("stops and disposes the subscriber", () => {
            let subscriber =
              RxSubscriber.create();
            subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToFalse;
            subscriber |> RxSubscriber.asDisposable |> RxDisposable.dispose;
            subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
    ],
  );