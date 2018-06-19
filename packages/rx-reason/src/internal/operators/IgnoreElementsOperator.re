let operator = observer =>
  Observer.delegate(
    ~onNext=Functions.alwaysUnit,
    ~onComplete=Observer.forwardOnComplete(observer),
    observer,
  );

let lift = observable => observable |> ObservableSource.lift(operator);