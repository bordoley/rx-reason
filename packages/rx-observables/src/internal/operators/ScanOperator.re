let create = (scanner: (. 'a, 'b) => 'a, initialValue: 'a, subscriber: RxSubscriber.t('a)) => {
  let acc = ref(initialValue);
  let mapper = (. next: 'b) => {
    let prevAcc = acc^;
    let nextAcc = scanner(. prevAcc, next);
    acc := nextAcc;
    nextAcc;
  };

  subscriber |> RxSubscriber.next(initialValue);
  MapOperator.create(mapper, subscriber);
};