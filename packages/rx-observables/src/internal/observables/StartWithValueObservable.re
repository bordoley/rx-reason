let create = (~scheduler=?, value, observable) =>
  ConcatListObservable.create([
    OfValueObservable.create(~scheduler?, value),
    observable,
  ]);