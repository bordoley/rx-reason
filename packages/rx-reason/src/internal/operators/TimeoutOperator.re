type context('a) = {
  mutable connect: unit => Disposable.t,
  timeoutSubscription: SerialDisposable.t,
};

let operator = {
  let subscribeToTimeout = ({connect, timeoutSubscription}) => {
    timeoutSubscription |> SerialDisposable.get |> Disposable.dispose;
    timeoutSubscription |> SerialDisposable.set(connect());
  };

  let onNext = (ctx, delegate, next) => {
    delegate |> Subscriber.next(next);
    subscribeToTimeout(ctx);
  };

  let onComplete = ({timeoutSubscription}, delegate, exn) => {
    timeoutSubscription |> SerialDisposable.dispose;
    delegate |> Subscriber.complete(~exn?);
  };

  scheduler => {
    let timeoutObservable =
      RaiseObservableSource.raise(~scheduler, TimeoutException.Exn);

    subscriber => {
      let context = {
        connect: Disposable.empty,
        timeoutSubscription: SerialDisposable.create(),
      };

      let self =
        subscriber
        |> Subscriber.delegate1(~onNext, ~onComplete, context)
        |> Subscriber.addSerialDisposable(context.timeoutSubscription);

      context.connect =
        ObservableSource.publishTo1(
          ~onNext=Functions.alwaysUnit2,
          ~onComplete=Subscriber.delegateOnComplete,
          self,
          timeoutObservable,
        );
      
      subscribeToTimeout(context);
      self;
    };
  };
};

let lift = (scheduler, observable) =>
  observable |> ObservableSource.lift(operator(scheduler));