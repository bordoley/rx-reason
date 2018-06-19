let operator = (scanner, initialValue, subscriber) => {
  let acc = ref(initialValue);
  let mapper = next => {
    let prevAcc = acc^;
    let nextAcc = scanner(prevAcc, next);
    acc := nextAcc;
    nextAcc;
  };
  subscriber |> Subscriber.next(initialValue);
  MapOperator.operator(mapper, subscriber);
};

let lift = (scanner, initialValue, observable) =>
  observable |> ObservableSource.lift(operator(scanner, initialValue));