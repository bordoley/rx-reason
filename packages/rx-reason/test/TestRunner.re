open ReUnit.Test;

ReUnit.run(describe("Reactive.re", [
  DisposableTest.test, 
  MulticastObservableTest.test,
  ObserverTest.test, 
  ObservableTest.test,
  OperatorsTest.test,
  SingleTest.test,
  SubjectTest.test,
]));