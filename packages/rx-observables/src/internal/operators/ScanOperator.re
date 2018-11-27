let operator = (scanner, initialValue, subscriber) => {
  let acc = ref(initialValue);
  let mapper = next => {
    let prevAcc = acc^;
    let nextAcc = scanner(prevAcc, next);
    acc := nextAcc;
    nextAcc;
  };
  subscriber |> RxSubscriber.next(initialValue);
  MapOperator.operator(mapper, subscriber);
};