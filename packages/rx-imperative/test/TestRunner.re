open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-imperative",
    [
      RxSubjectTest.test,
    ],
  ),
);