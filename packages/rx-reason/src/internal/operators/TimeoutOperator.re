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

  (~due, ~scheduler) => {
    let timeoutObservable =
      RaiseObservableSource.raise(TimeoutException.Exn)
      |> DelayOperator.lift(~scheduler, ~delay=due);

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
        ObservableSource.publishTo1(
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

let lift = (~due, ~scheduler, observable) =>
  observable |> ObservableSource.lift(operator(~due, ~scheduler));