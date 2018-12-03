open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-observables",
    [RxObservablesTest.test, RxOperatorsTest.test],
  ),
);