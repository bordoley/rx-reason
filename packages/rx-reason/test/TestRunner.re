open ReUnit.Test;

ReUnit.run(
  describe(
    "Reactive.re",
    [
      DisposableTest.test,
      SubscriberTest.test,
      ObservableTest.test,
      ObservablesTest.test,
      OperatorsTest.test,
      SubjectTest.test,
    ],
  ),
);