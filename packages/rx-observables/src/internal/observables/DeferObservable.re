let source = (f, subscriber) => {
  let innerSubscription =
    f()
    |> RxObservable.lift(ForwardingOperator.operator(subscriber))
    |> RxObservable.subscribe;

  subscriber |> RxSubscriber.addDisposable(innerSubscription) |> ignore;
};

let create = f => RxObservable.create1(source, f);