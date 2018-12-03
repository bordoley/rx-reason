let defaultShouldUpdate = (a, b) => a !== b;

let create = (~equals=RxFunctions.referenceEquality) => {
  let shouldUpdate =
    equals === RxFunctions.referenceEquality ?
      defaultShouldUpdate : ((a, b) => ! equals(a, b));

  subscriber => {
    let state = RxMutableOption.create();
    let predicate = next => RxMutableOption.setIf(shouldUpdate, next, state);
    KeepOperator.create(predicate, subscriber);
  };
};