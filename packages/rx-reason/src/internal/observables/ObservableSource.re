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
    let shouldRaise =
      subscriber |> Subscriber.completeWithResult(~exn) |> (!);
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

let subscribeWith1 = (~onNext, ~onComplete, ctx0, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing1(~onNext, ~onComplete, ctx0);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asCompositeDisposable;
};

let subscribeWith2 = (~onNext, ~onComplete, ctx0, ctx1, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing2(~onNext, ~onComplete, ctx0, ctx1);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asCompositeDisposable;
};

let subscribeWith3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing3(~onNext, ~onComplete, ctx0, ctx1, ctx2);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asCompositeDisposable;
};

let subscribeWith4 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing4(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
    );

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asCompositeDisposable;
};

let subscribeWith5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing5(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
    );

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asCompositeDisposable;
};

let subscribeWith6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing6(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
    );

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asCompositeDisposable;
};

let subscribeWith7 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing7(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
    );

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

let subscribe1 =
    (
      ~onNext=Functions.alwaysUnit2,
      ~onComplete=Functions.alwaysUnit2,
      ctx0,
      observable,
    ) =>
  observable |> subscribeWith1(~onNext, ~onComplete, ctx0);

let subscribe2 =
    (
      ~onNext=Functions.alwaysUnit3,
      ~onComplete=Functions.alwaysUnit3,
      ctx0,
      ctx1,
      observable,
    ) =>
  observable |> subscribeWith2(~onNext, ~onComplete, ctx0, ctx1);

let subscribe3 =
    (
      ~onNext=Functions.alwaysUnit4,
      ~onComplete=Functions.alwaysUnit4,
      ctx0,
      ctx1,
      ctx2,
      observable,
    ) =>
  observable |> subscribeWith3(~onNext, ~onComplete, ctx0, ctx1, ctx2);

let subscribe4 =
    (
      ~onNext=Functions.alwaysUnit5,
      ~onComplete=Functions.alwaysUnit5,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      observable,
    ) =>
  observable |> subscribeWith4(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3);

let subscribe5 =
    (
      ~onNext=Functions.alwaysUnit6,
      ~onComplete=Functions.alwaysUnit6,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      observable,
    ) =>
  observable
  |> subscribeWith5(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4);

let subscribe6 =
    (
      ~onNext=Functions.alwaysUnit7,
      ~onComplete=Functions.alwaysUnit7,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      observable,
    ) =>
  observable
  |> subscribeWith6(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5);

let subscribe7 =
    (
      ~onNext=Functions.alwaysUnit8,
      ~onComplete=Functions.alwaysUnit8,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      observable,
    ) =>
  observable
  |> subscribeWith7(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6);

let publishTo = {
  let teardown = (subscription, active) => {
    subscription |> CompositeDisposable.dispose;
    Volatile.write(false, active);
  };

  (~onNext, ~onComplete, observable) => {
    let connection = ref(Disposable.disposed);
    let active = ref(false);

    () => {
      if (! Interlocked.exchange(true, active)) {
        let subscription = observable |> subscribeWith(~onNext, ~onComplete);
        let newConnection =
          Disposable.create2(teardown, subscription, active);

        Volatile.write(newConnection, connection);
      };
      Volatile.read(connection);
    };
  };
};

let publishTo1 = {
  let teardown = (subscription, active) => {
    subscription |> CompositeDisposable.dispose;
    Volatile.write(false, active);
  };

  (~onNext, ~onComplete, ctx0, observable) => {
    let connection = ref(Disposable.disposed);
    let active = ref(false);

    () => {
      if (! Interlocked.exchange(true, active)) {
        let subscription = observable |> subscribeWith1(~onNext, ~onComplete, ctx0);
        let newConnection =
          Disposable.create2(teardown, subscription, active);

        Volatile.write(newConnection, connection);
      };
      Volatile.read(connection);
    };
  };
};

let publish =
    (
      ~onNext=Functions.alwaysUnit1,
      ~onComplete=Functions.alwaysUnit1,
      observable,
    ) =>
  publishTo(~onNext, ~onComplete, observable);

let publish1 =
  (
    ~onNext=Functions.alwaysUnit2,
    ~onComplete=Functions.alwaysUnit2,
    ctx,
    observable,
  ) =>
publishTo1(~onNext, ~onComplete, ctx, observable);

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