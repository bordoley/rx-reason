let predicate = (skipCount, count, _) => {
  let count = RxAtomic.incr(count);
  if (count <= skipCount) {
    false;
  } else {
    true;
  };
};

let create = skipCount => {
  RxPreconditions.checkArgument(
    skipCount > 0,
    "SkipOperator: skipCount must be greater than 0",
  );

  subscriber => {
    let count = RxAtomic.make(0);
    KeepOperator.create2(predicate, skipCount, count, subscriber);
  };
};