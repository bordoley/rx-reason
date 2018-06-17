let operator = observer =>
  Observer.create(
    ~onNext=Functions.alwaysUnit,
    ~onComplete=Observer.forwardOnComplete(observer),
    ~onDispose=Observer.forwardOnDispose(observer),
  );

let lift = observable => observable |> ObservableSource.lift(operator);