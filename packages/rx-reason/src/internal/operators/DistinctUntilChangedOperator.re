let operator =
    (~equals=Functions.referenceEquality) => {
  let shouldUpdate = (a, b) => ! equals(a, b);
  subscriber => {
    let state = MutableOption.create();
    let predicate = next => MutableOption.setIf(shouldUpdate, next, state);
    KeepOperator.operator(predicate, subscriber);
  };
};

let lift = (~equals=?, observable) =>
  observable |> ObservableSource.lift(operator(~equals?));