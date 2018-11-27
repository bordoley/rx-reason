open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-core",
    [
      RxSubscriberTest.test,
    ],
  ),
);