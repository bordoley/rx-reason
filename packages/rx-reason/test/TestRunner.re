open ReUnit.Test;

ReUnit.run(
  describe(
    "Reactive.re",
    [
      DisposableTest.test,
      SubscriberTest.test,
      MulticastObservableTest.test,
      ObservableTest.test,
      SingleTest.test,
      SubjectTest.test,
    ],
  ),
);