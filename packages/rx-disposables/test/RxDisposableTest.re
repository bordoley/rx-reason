open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxDisposable",
    [
      describe(
        "compose",
        [
          it("individual disposables disposed in order", () => {
            let count = ref(0);
            let disposable0 = RxDisposable.create(() => {
              count^ |> Expect.toBeEqualToInt(0);
              incr(count);
            });
            let disposable1 = RxDisposable.create(() => {
              count^ |> Expect.toBeEqualToInt(1);
              incr(count);
            });
            let disposable2 = RxDisposable.create(() => {
              count^ |> Expect.toBeEqualToInt(2);
            });

            RxDisposable.compose([disposable0, disposable1, disposable2])
            |> RxDisposable.dispose;
          }),
        ],
      ),
      describe(
        "create",
        [
          it("is not disposed when created", () =>
            RxDisposable.create(() => ())
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToFalse
          ),
          it("is disposed after being disposed", () => {
            let disposable = RxDisposable.create(() => ());
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("dispose calls teardown logic", () => {
            let called = ref(false);
            let disposable = RxDisposable.create(() => called := true);
            disposable |> RxDisposable.dispose;
            called^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "create1",
        [
          it("is not disposed when created", () =>
            RxDisposable.create1(_ => (), ())
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToFalse
          ),
          it("is disposed after being disposed", () => {
            let disposable = RxDisposable.create1(_ => (), ());
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("dispose calls teardown logic", () => {
            let called = ref(false);
            let disposable = RxDisposable.create1(_ => called := true, ());
            disposable |> RxDisposable.dispose;
            called^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "create2",
        [
          it("is not disposed when created", () =>
            RxDisposable.create2((_, _) => (), (), ())
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToFalse
          ),
          it("is disposed after being disposed", () => {
            let disposable = RxDisposable.create2((_, _) => (), (), ());
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("dispose calls teardown logic", () => {
            let called = ref(false);
            let disposable =
              RxDisposable.create2((_, _) => called := true, (), ());
            disposable |> RxDisposable.dispose;
            called^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "create3",
        [
          it("is not disposed when created", () =>
            RxDisposable.create3((_, _, _) => (), (), (), ())
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToFalse
          ),
          it("is disposed after being disposed", () => {
            let disposable =
              RxDisposable.create3((_, _, _) => (), (), (), ());
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("dispose calls teardown logic", () => {
            let called = ref(false);
            let disposable =
              RxDisposable.create3((_, _, _) => called := true, (), (), ());
            disposable |> RxDisposable.dispose;
            called^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "create4",
        [
          it("is not disposed when created", () =>
            RxDisposable.create4((_, _, _, _) => (), (), (), (), ())
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToFalse
          ),
          it("is disposed after being disposed", () => {
            let disposable =
              RxDisposable.create4((_, _, _, _) => (), (), (), (), ());
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("dispose calls teardown logic", () => {
            let called = ref(false);
            let disposable =
              RxDisposable.create4(
                (_, _, _, _) => called := true,
                (),
                (),
                (),
                (),
              );
            disposable |> RxDisposable.dispose;
            called^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "create5",
        [
          it("is not disposed when created", () =>
            RxDisposable.create5((_, _, _, _, _) => (), (), (), (), (), ())
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToFalse
          ),
          it("is disposed after being disposed", () => {
            let disposable =
              RxDisposable.create5(
                (_, _, _, _, _) => (),
                (),
                (),
                (),
                (),
                (),
              );
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("dispose calls teardown logic", () => {
            let called = ref(false);
            let disposable =
              RxDisposable.create5(
                (_, _, _, _, _) => called := true,
                (),
                (),
                (),
                (),
                (),
              );
            disposable |> RxDisposable.dispose;
            called^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "disposed",
        [
          describe(
            "dispose",
            [
              it("does nothing", () =>
                RxDisposable.disposed |> RxDisposable.dispose
              ),
            ],
          ),
          describe(
            "isDisposed",
            [
              it("always returns true", () =>
                RxDisposable.disposed
                |> RxDisposable.isDisposed
                |> Expect.toBeEqualToTrue
              ),
            ],
          ),
        ],
      ),
      describe(
        "empty",
        [
          it("is not disposed when created", () =>
            RxDisposable.empty()
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToFalse
          ),
          it("is disposed after being disposed", () => {
            let disposable = RxDisposable.empty();
            disposable |> RxDisposable.dispose;
            disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "raiseIfDisposed",
        [
          it("does nothing if not disposed", () =>
            RxDisposable.empty() |> RxDisposable.raiseIfDisposed
          ),
          it("raises RxDisposedException when disposed", () =>
            Expect.shouldRaise(() =>
              RxDisposable.disposed |> RxDisposable.raiseIfDisposed
            )
          ),
        ],
      ),
    ],
  );