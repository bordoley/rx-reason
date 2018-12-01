open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-core",
    [
      RxObservableTest.test,
      RxSubscriberTest.test,
    ],
  ),
);