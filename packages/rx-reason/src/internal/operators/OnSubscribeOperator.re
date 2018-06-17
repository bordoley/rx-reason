let operator = (f, observer) => {
  let callbackDisposable = f();
  Observer.create(
    ~onNext=Observer.forwardOnNext(observer),
    ~onComplete=Observer.forwardOnComplete(observer),
    ~onDispose=() => {
      callbackDisposable |> Disposable.dispose;
      observer |> Observer.dispose;
    },
  );
};

let lift = (f, observable) =>
  observable |> ObservableSource.lift(operator(f));