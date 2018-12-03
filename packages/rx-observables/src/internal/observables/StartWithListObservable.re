let create = (~scheduler=?, values, observable) =>
  ConcatListObservable.create([
    OfListObservable.create(~scheduler?, values),
    observable,
  ]);