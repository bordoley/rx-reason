type context('a) = {
  mutable connect: unit => Disposable.t,
  timeoutSubscription: SerialDisposable.t,
};

let operator = {
  let subscribeToTimeout = ({connect, timeoutSubscription}) => {
    timeoutSubscription
    |> SerialDisposable.getInnerDisposable
    |> Disposable.dispose;
    timeoutSubscription |> SerialDisposable.setInnerDisposable(connect());
  };

  let onNext = (ctx, delegate, next) => {
    delegate |> Subscriber.next(next);
    subscribeToTimeout(ctx);
  };

  let onComplete = ({timeoutSubscription}, delegate, exn) => {
    timeoutSubscription |> SerialDisposable.dispose;
    delegate |> Subscriber.complete(~exn?);
  };

  (~scheduler, ~due) => {
    let timeoutObservable =
      Observables.raise(TimeoutException.Exn)
      |> Observable.lift(DelayOperator.operator(~scheduler, ~delay=due));

    subscriber => {
      let context = {
        connect: Disposable.empty,
        timeoutSubscription: SerialDisposable.create(),
      };

      let self =
        subscriber
        |> Subscriber.decorate1(~onNext, ~onComplete, context)
        |> Subscriber.addTeardown1(
             SerialDisposable.dispose,
             context.timeoutSubscription,
           );

      context.connect =
        Observables.publish1(
          ~onNext=Functions.alwaysUnit2,
          ~onComplete=Subscriber.forwardOnComplete,
          self,
          timeoutObservable,
        );

      subscribeToTimeout(context);
      self;
    };
  };
};