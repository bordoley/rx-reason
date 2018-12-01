open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxCompositeDisposable",
    [
      describe(
        "create",
        [
          describe(
            "addDisposable",
            [
              it("disposes the added RxDisposable if disposed", () => {
                let disposable = RxDisposable.empty();

                let compositeDisposable = RxCompositeDisposable.create();
                compositeDisposable |> RxCompositeDisposable.dispose;

                compositeDisposable
                |> RxCompositeDisposable.addDisposable(disposable)
                |> ignore;

                disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
              }),
            ],
          ),
          describe(
            "removeDisposable",
            [
              it(
                "removes the child Disposable, and does not dispose it when disposed",
                () => {
                let disposable0 = RxDisposable.empty();
                let disposable1 = RxDisposable.empty();
                let disposable2 = RxDisposable.empty();

                let compositeDisposable =
                  RxCompositeDisposable.create()
                  |> RxCompositeDisposable.addDisposable(disposable0)
                  |> RxCompositeDisposable.addDisposable(disposable1)
                  |> RxCompositeDisposable.addDisposable(disposable2);

                compositeDisposable
                |> RxCompositeDisposable.removeDisposable(disposable1)
                |> ignore;

                compositeDisposable |> RxCompositeDisposable.dispose;
                disposable0
                |> RxDisposable.isDisposed
                |> Expect.toBeEqualToTrue;
                disposable1
                |> RxDisposable.isDisposed
                |> Expect.toBeEqualToFalse;
                disposable2
                |> RxDisposable.isDisposed
                |> Expect.toBeEqualToTrue;
              }),
            ],
          ),
        ],
      ),
      describe(
        "disposed",
        [
          describe(
            "addDisposable",
            [
              it("disposes the added RxDisposable", () => {
                let disposable = RxDisposable.empty();

                RxCompositeDisposable.disposed
                |> RxCompositeDisposable.addDisposable(disposable)
                |> ignore;

                disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
              }),
            ],
          ),
          describe(
            "asDisposable",
            [
              it("is equal to RxDisposable.disposed", () =>
                RxCompositeDisposable.disposed
                |> RxCompositeDisposable.asDisposable
                |> Expect.toBeReferenceEqualTo(RxDisposable.disposed)
              ),
            ],
          ),
          describe(
            "isDisposed",
            [
              it("is true", () =>
                RxCompositeDisposable.disposed
                |> RxCompositeDisposable.isDisposed
                |> Expect.toBeEqualToTrue
              ),
            ],
          ),
          describe(
            "raisedIfDisposed",
            [
              it("is true", () =>
                Expect.shouldRaise(() =>
                  RxCompositeDisposable.disposed
                  |> RxCompositeDisposable.raiseIfDisposed
                )
              ),
            ],
          ),
          describe(
            "removeDisposable",
            [
              it("does nothing", () => {
                let disposable = RxDisposable.empty();
                RxCompositeDisposable.disposed
                |> RxCompositeDisposable.removeDisposable(disposable)
                |> ignore;
              }),
            ],
          ),
        ],
      ),
    ],
  );