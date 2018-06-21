type source('a) = Subscriber.t('a) => unit;

type t('a) =
  | Source(source('a)): t('a)
  | Lift1(source('b), Operator.t('b, 'a)): t('a)
  | Lift2(source('c), Operator.t('c, 'b), Operator.t('b, 'a)): t('a)
  | Lift3(
           source('d),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Lift4(
           source('e),
           Operator.t('e, 'd),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a);

let create = onSubscribe => Source(onSubscribe);

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  switch (observable) {
  | Source(source) => Lift1(source, operator)
  | Lift1(source, op0) => Lift2(source, op0, operator)
  | Lift2(source, op0, op1) => Lift3(source, op0, op1, operator)
  | Lift3(source, op0, op1, op2) => Lift4(source, op0, op1, op2, operator)
  | Lift4(source, op0, op1, op2, op3) =>
    let op0 = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ operator @@ subscriber;
    Lift1(source, op0);
  };

let never = Source(Functions.alwaysUnit1);

let subscribeSafe = (subscriber, source) =>
  try (source(subscriber)) {
  | exn =>
    let shouldRaise = subscriber |> Subscriber.completeWithResult(~exn) |> (!);
    if (shouldRaise) {
      /* This could happen when the onComplete is called synchronously in the
       * subscribe function which also throws.
       */
      raise(
        exn,
      );
    };
  };

let subscribeSubscriber = (subscriber, observable) =>
  switch (observable) {
  | Source(source) => source |> subscribeSafe(subscriber)
  | Lift1(source, operator) =>
    let subscriber = operator(subscriber);
    source |> subscribeSafe(subscriber);
  | Lift2(source, op0, op1) =>
    let subscriber = op0(op1(subscriber));
    source |> subscribeSafe(subscriber);
  | Lift3(source, op0, op1, op2) =>
    let subscriber = op0(op1(op2(subscriber)));
    source |> subscribeSafe(subscriber);
  | Lift4(source, op0, op1, op2, op3) =>
    let subscriber = op0(op1(op2(op3(subscriber))));
    source |> subscribeSafe(subscriber);
  };

let subscribeWith = (~onNext, ~onComplete, observable) => {
  let subscriber = Subscriber.createAutoDisposing(~onNext, ~onComplete);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asCompositeDisposable;
};

let subscribe =
    (
      ~onNext=Functions.alwaysUnit1,
      ~onComplete=Functions.alwaysUnit1,
      observable,
    ) =>
  observable |> subscribeWith(~onNext, ~onComplete);

let publishTo = (~onNext, ~onComplete, observable) => {
  let connection = ref(Disposable.disposed);
  let active = ref(false);

  () => {
    if (! Interlocked.exchange(true, active)) {
      let subscription = observable |> subscribeWith(~onNext, ~onComplete);
      let newConnection =
        Disposable.create(() => {
          subscription |> CompositeDisposable.dispose;
          Volatile.write(false, active);
        });

      Volatile.write(newConnection, connection);
    };
    Volatile.read(connection);
  };
};

let publish =
    (
      ~onNext=Functions.alwaysUnit1,
      ~onComplete=Functions.alwaysUnit1,
      observable,
    ) =>
  publishTo(~onNext, ~onComplete, observable);

let raise = (~scheduler=Scheduler.immediate, exn: exn) => {
  let exn = Some(exn);

  scheduler === Scheduler.immediate ?
    create(subscriber => subscriber |> Subscriber.complete(~exn?)) :
    create(subscriber => {
      let schedulerDisposable =
        scheduler(() => {
          subscriber |> Subscriber.complete(~exn?);
          Disposable.disposed;
        });
      subscriber |> Subscriber.addDisposable(schedulerDisposable) |> ignore;
    });
};