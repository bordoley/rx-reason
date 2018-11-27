open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-disposables",
    [
      RxDisposableTest.test,
    ],
  ),
);