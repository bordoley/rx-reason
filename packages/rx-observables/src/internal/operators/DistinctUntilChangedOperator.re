let defaultShouldUpdate = (a, b) => a !== b;

let predicate = (shouldUpdate, state, next) =>
  RxMutableOption.setIf(shouldUpdate, next, state);

let create = (~equals=RxFunctions.referenceEquality) => {
  let shouldUpdate =
    equals === RxFunctions.referenceEquality ?
      defaultShouldUpdate : ((a, b) => !equals(a, b));

  subscriber => {
    let state = RxMutableOption.create();
    KeepOperator.create2(predicate, shouldUpdate, state, subscriber);
  };
};