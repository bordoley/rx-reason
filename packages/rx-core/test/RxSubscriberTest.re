open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxSubscriber",
    [
      describe(
        "complete",
        [
          it(
            "recursively call onComplete when an exception is thrown from the delegate handler",
            () => {
              let subscriber = RxSubscriber.create();
              let observedException = ref(None);
              let lastDecorator =
                RxSubscriber.decorate(
                  ~onNext=(_, _) => (),
                  ~onComplete=(_, exn) => observedException := exn,
                  subscriber,
                );
              let throwingDecorator =
                RxSubscriber.decorate(
                  ~onNext=(_, _) => (),
                  ~onComplete=
                    (_, exn) =>
                      switch (exn) {
                      | Some(exn) => raise(exn)
                      | _ => ()
                      },
                  lastDecorator,
                );
              ();
              throwingDecorator |> RxSubscriber.complete(~exn=RxDisposedException.Exn);

              observedException^ |> Expect.toBeEqualToSomeReference(RxDisposedException.Exn);
            },
          ),
        ],
      ),
      describe(
        "completeWithResult",
        [
          it("returns true if not stopped", () => {
            let subscriber = RxSubscriber.create();

            subscriber
            |> RxSubscriber.completeWithResult
            |> Expect.toBeEqualToTrue;
          }),
          it("returns false if stopped", () => {
            let subscriber = RxSubscriber.create();

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
          describe(
            "dispose",
            [
              it("disposes the subscriber", () => {
                let subscriber = RxSubscriber.create();
                subscriber
                |> RxSubscriber.isDisposed
                |> Expect.toBeEqualToFalse;
                subscriber |> RxSubscriber.asDisposable |> RxDisposable.dispose;
                subscriber |> RxSubscriber.isDisposed |> Expect.toBeEqualToTrue;
              }),
            ],
          ),
        ],
      ),
    ],
  );