open ReUnit.Test;

ReUnit.run(describe("Reactive.re", [DisposableTest.test, ObserverTest.test, OperatorsTest.test]));