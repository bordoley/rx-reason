open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-disposables",
    [
      RxCompositeDisposableTest.test,
      RxDisposableTest.test,
      RxSerialDisposableTest.test,
    ],
  ),
);