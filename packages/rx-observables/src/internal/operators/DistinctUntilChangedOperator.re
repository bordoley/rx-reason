let operator = {
  let defaultShouldUpdate = (a, b) => a !== b;

  (~equals=RxFunctions.referenceEquality) => {
    let shouldUpdate =
      equals === RxFunctions.referenceEquality ?
        defaultShouldUpdate : ((a, b) => ! equals(a, b));

    subscriber => {
      let state = RxMutableOption.create();
      let predicate = next => RxMutableOption.setIf(shouldUpdate, next, state);
      KeepOperator.operator(predicate, subscriber);
    };
  };
};