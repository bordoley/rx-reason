open ReUnit;
open ReUnit.Test;

let itIsNotDisposedWhenCreated = f =>
  it("is not disposed when created", () =>
    f() |> RxDisposable.isDisposed |> Expect.toBeEqualToFalse
  );

let itIsDisposedAfterDisposedCall = f =>
  it("is disposed after being disposed", () => {
    let disposable = f();
    disposable |> RxDisposable.dispose;
    disposable |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;
  });

let test =
  describe(
    "RxDisposable",
    [
      describe(
        "compose",
        [
          it("individual disposables disposed in order", () => {
            let count = ref(0);
            let disposable0 =
              RxDisposable.create(() => {
                count^ |> Expect.toBeEqualToInt(0);
                incr(count);
              });
            let disposable1 =
              RxDisposable.create(() => {
                count^ |> Expect.toBeEqualToInt(1);
                incr(count);
              });
            let disposable2 =
              RxDisposable.create(() => count^ |> Expect.toBeEqualToInt(2));

            RxDisposable.compose([disposable0, disposable1, disposable2])
            |> RxDisposable.dispose;
          }),
        ],
      ),
      describe(
        "create",
        [
          itIsNotDisposedWhenCreated(() => RxDisposable.create(() => ())),
          itIsDisposedAfterDisposedCall(() => RxDisposable.create(() => ())),
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
          itIsNotDisposedWhenCreated(() => RxDisposable.create1(_ => (), ())),
          itIsDisposedAfterDisposedCall(() =>
            RxDisposable.create1(_ => (), ())
          ),
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
          itIsNotDisposedWhenCreated(() =>
            RxDisposable.create2((_, _) => (), (), ())
          ),
          itIsDisposedAfterDisposedCall(() =>
            RxDisposable.create2((_, _) => (), (), ())
          ),
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
          itIsNotDisposedWhenCreated(() =>
            RxDisposable.create3((_, _, _) => (), (), (), ())
          ),
          itIsDisposedAfterDisposedCall(() =>
            RxDisposable.create3((_, _, _) => (), (), (), ())
          ),
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
          itIsNotDisposedWhenCreated(() =>
            RxDisposable.create4((_, _, _, _) => (), (), (), (), ())
          ),
          itIsDisposedAfterDisposedCall(() =>
            RxDisposable.create4((_, _, _, _) => (), (), (), (), ())
          ),
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
          itIsNotDisposedWhenCreated(() =>
            RxDisposable.create5((_, _, _, _, _) => (), (), (), (), (), ())
          ),
          itIsDisposedAfterDisposedCall(() =>
            RxDisposable.create5((_, _, _, _, _) => (), (), (), (), (), ())
          ),
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
          itIsNotDisposedWhenCreated(RxDisposable.empty),
          itIsDisposedAfterDisposedCall(RxDisposable.empty),
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