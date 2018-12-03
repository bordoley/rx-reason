open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-imperative",
    [
      RxEventTest.test,
      RxSubjectTest.test,
      RxValueTest.test,
    ],
  ),
);