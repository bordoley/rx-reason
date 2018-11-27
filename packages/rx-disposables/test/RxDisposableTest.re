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
            let onDispose = () => incr(count);
            let composed =
              RxDisposable.compose([
                RxDisposable.create(onDispose),
                RxDisposable.create1(incr, count),
                RxDisposable.create(onDispose),
              ]);
            composed |> RxDisposable.dispose;
            count^ |> Expect.toBeEqualToInt(3);
            composed |> RxDisposable.dispose;
            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe(
        "isDisposed",
        [
          it("is false if not disposed", () => {
            let disposable = RxDisposable.empty();
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToFalse;
          }),
          it("is true if disposed", () => {
            let disposable = RxDisposable.empty();
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
    ],
  );