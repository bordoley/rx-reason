open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxSerialDisposable",
    [
      describe(
        "create",
        [
          describe(
            "getInnerDisposable",
            [
              it("returns the current inner Disposable", () => {
                let disposable = RxDisposable.empty();
                let serialDisposable = RxSerialDisposable.create();

                serialDisposable
                |> RxSerialDisposable.getInnerDisposable
                |> Expect.toBeReferenceEqualTo(RxDisposable.disposed);

                serialDisposable
                |> RxSerialDisposable.setInnerDisposable(disposable);

                serialDisposable
                |> RxSerialDisposable.getInnerDisposable
                |> Expect.toBeReferenceEqualTo(disposable);
              }),
            ],
          ),
          describe(
            "dispose",
            [
              it("disposes the inner Disposable", () => {
                let disposable = RxDisposable.empty();
                let serialDisposable = RxSerialDisposable.create();
                serialDisposable
                |> RxSerialDisposable.setInnerDisposable(disposable);
                serialDisposable |> RxSerialDisposable.dispose;
                disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
              }),
            ],
          ),
        ],
      ),
      describe(
        "disposed",
        [
          describe(
            "getInnerDisposable",
            [
              it("returns RxDisposable.disposed", () =>
                RxSerialDisposable.disposed
                |> RxSerialDisposable.getInnerDisposable
                |> Expect.toBeReferenceEqualTo(RxDisposable.disposed)
              ),
            ],
          ),
          describe(
            "isDisposed",
            [
              it("is true", () =>
                RxSerialDisposable.disposed
                |> RxSerialDisposable.isDisposed
                |> Expect.toBeEqualToTrue
              ),
            ],
          ),
          describe(
            "setInnerDisposable",
            [
              it("disposes the provided disposable", () => {
                let disposable = RxDisposable.empty();

                RxSerialDisposable.disposed
                |> RxSerialDisposable.setInnerDisposable(disposable);

                disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
              }),
            ],
          ),
        ],
      ),
    ],
  );