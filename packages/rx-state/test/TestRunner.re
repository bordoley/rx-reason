open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-state",
    [
      RxSubjectTest.test,
    ],
  ),
);