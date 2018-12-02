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

              throwingDecorator
              |> RxSubscriber.complete(~exn=Division_by_zero);

              observedException^
              |> Expect.toBeEqualToSomeReference(Division_by_zero);
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
      describe(
        "next",
        [
          it("does nothing if the subscriber has been completed", () => {
            let onNextCalledCount = ref(0);

            let subscriber = RxSubscriber.create();
            let decorator =
              RxSubscriber.decorate(
                ~onNext=(_, _) => incr(onNextCalledCount),
                ~onComplete=(_, _) => (),
                subscriber,
              );

            decorator |> RxSubscriber.next();
            decorator |> RxSubscriber.complete;
            decorator |> RxSubscriber.next();

            onNextCalledCount^ |> Expect.toBeEqualToInt(1);
          }),
          it("does nothing if the subscriber has been disposed", () => {
            let onNextCalledCount = ref(0);

            let subscriber = RxSubscriber.create();
            let decorator =
              RxSubscriber.decorate(
                ~onNext=(_, _) => incr(onNextCalledCount),
                ~onComplete=(_, _) => (),
                subscriber,
              );

            decorator |> RxSubscriber.next();
            decorator |> RxSubscriber.dispose;
            decorator |> RxSubscriber.next();

            onNextCalledCount^ |> Expect.toBeEqualToInt(1);
          }),
          it("completes if onNext throws an exception", () => {
            let completed = ref(false);

            let subscriber = RxSubscriber.create();
            let decorator =
              RxSubscriber.decorate(
                ~onNext=(_, _) => raise(Division_by_zero),
                ~onComplete=(_, _) => completed := true,
                subscriber,
              );

            decorator |> RxSubscriber.next();
            completed^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
    ],
  );