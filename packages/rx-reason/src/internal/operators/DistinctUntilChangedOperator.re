let operator = {
  let defaultShouldUpdate = (a, b) => a !== b;

  (~equals=Functions.referenceEquality) => {
    let shouldUpdate =
      equals === Functions.referenceEquality ?
        defaultShouldUpdate : ((a, b) => ! equals(a, b));

    subscriber => {
      let state = MutableOption.create();
      let predicate = next => MutableOption.setIf(shouldUpdate, next, state);
      KeepOperator.operator(predicate, subscriber);
    };
  };
};