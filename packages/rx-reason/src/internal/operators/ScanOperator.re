let operator = (scanner, initialValue, observer) => {
  let acc = ref(initialValue);
  let mapper = next => {
    let prevAcc = acc^;
    let nextAcc = scanner(prevAcc, next);
    acc := nextAcc;
    nextAcc;
  };
  observer |> Observer.next(initialValue);
  MapOperator.operator(mapper, observer);
};

let lift = (scanner, initialValue, observable) =>
  observable |> ObservableSource.lift(operator(scanner, initialValue));