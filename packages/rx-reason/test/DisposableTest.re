open ReUnit;

open ReUnit.Test;

let test =
  describe(
    "Disposable",
    [
      describe(
        "compose",
        [
          it("should dispose all children disposables once", () => {
            let count = ref(0);
            let onDispose = () => {count := count ^ + 1};
            let composed = Disposable.compose([
              Disposable.create(onDispose),
              Disposable.create(onDispose),
              Disposable.create(onDispose),
            ]);
            composed |> Disposable.dispose;
            count^ |> Expect.toBeEqualToInt(3);
            composed |> Disposable.dispose;
            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe(
        "isDisposed",
        [
          it("is false if not disposed", () => {
            let disposable = Disposable.empty();
            disposable |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
          }),
          it("is true if disposed", () => {
            let disposable = Disposable.empty();
            disposable |> Disposable.dispose;
            disposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "raiseIfDisposed",
        [
          it("should raise if disposable disposed", () => {
            let disposable = Disposable.disposed;
            Expect.shouldRaise(() => Disposable.raiseIfDisposed(disposable));
          }),
          it("should not raise if disposable not disposed", () => {
            let disposable = Disposable.empty();
            Disposable.raiseIfDisposed(disposable);
          }),
        ],
      ),
    ],
  );