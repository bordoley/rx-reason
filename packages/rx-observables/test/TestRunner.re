open ReUnit.Test;

ReUnit.run(
  describe(
    "rx-observables",
    [ObservableTest.test, RxObservablesTest.test, RxOperatorsTest.test],
  ),
);