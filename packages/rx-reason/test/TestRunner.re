open ReUnit.Test;

ReUnit.run(describe("Reactive.re", [
  DisposableTest.test, 
  ObserverTest.test, 
  ObservableTest.test,/*
  OperatorsTest.test,*/
]));