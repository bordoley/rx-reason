let mapper = (scanner, acc, next) => {
  let prevAcc = acc^;
  let nextAcc = scanner(prevAcc, next);
  acc := nextAcc;
  nextAcc;
};

let create = (scanner, initialValue, subscriber) => {
  let acc = ref(initialValue);

  subscriber |> RxSubscriber.next(initialValue);
  MapOperator.create2(mapper, scanner, acc, subscriber);
};