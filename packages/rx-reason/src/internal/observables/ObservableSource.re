type source1('ctx0, 'a) = ('ctx0, Subscriber.t('a)) => unit;

type t('a) =
  | Source1(source1('ctx0, 'a), 'ctx0): t('a)
  | Source1Lift1(source1('ctx0, 'b), 'ctx0, Operator.t('b, 'a)): t('a)
  | Source1Lift2(
                  source1('ctx0, 'c),
                  'ctx0,
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift3(
                  source1('ctx0, 'd),
                  'ctx0,
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift4(
                  source1('ctx0, 'e),
                  'ctx0,
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift5(
                  source1('ctx0, 'f),
                  'ctx0,
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift6(
                  source1('ctx0, 'g),
                  'ctx0,
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift7(
                  source1('ctx0, 'h),
                  'ctx0,
                  Operator.t('h, 'g),
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a);

let create = onSubscribe => Source1(onSubscribe, ());
let create1 = (onSubscribe, ctx0) => Source1(onSubscribe, ctx0);

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  switch (observable) {
  | Source1(source, ctx0) => Source1Lift1(source, ctx0, operator)
  | Source1Lift1(source, ctx0, op0) =>
    Source1Lift2(source, ctx0, op0, operator)
  | Source1Lift2(source, ctx0, op0, op1) =>
    Source1Lift3(source, ctx0, op0, op1, operator)
  | Source1Lift3(source, ctx0, op0, op1, op2) =>
    Source1Lift4(source, ctx0, op0, op1, op2, operator)
  | Source1Lift4(source, ctx0, op0, op1, op2, op3) =>
    Source1Lift5(source, ctx0, op0, op1, op2, op3, operator)
  | Source1Lift5(source, ctx0, op0, op1, op2, op3, op4) =>
    Source1Lift6(source, ctx0, op0, op1, op2, op3, op4, operator)
  | Source1Lift6(source, ctx0, op0, op1, op2, op3, op4, op5) =>
    Source1Lift7(source, ctx0, op0, op1, op2, op3, op4, op5, operator)
  | Source1Lift7(source, ctx0, op0, op1, op2, op3, op4, op5, op6) =>
    let op0 = subscriber =>
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ operator @@ subscriber;
    Source1Lift1(source, ctx0, op0);
  };

let never = Source1(Functions.alwaysUnit2, ());

let subscribeSafe = (subscriber, ctx, source) =>
  try (source(ctx, subscriber)) {
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
  | Source1(source, ctx0) => source |> subscribeSafe(subscriber, ctx0)
  | Source1Lift1(source, ctx0, op0) =>
    let subscriber = op0(subscriber);
    source |> subscribeSafe(subscriber, ctx0);
  | Source1Lift2(source, ctx0, op0, op1) =>
    let subscriber = op0 @@ op1 @@ subscriber;
    source |> subscribeSafe(subscriber, ctx0);
  | Source1Lift3(source, ctx0, op0, op1, op2) =>
    let subscriber = op0 @@ op1 @@ op2 @@ subscriber;
    source |> subscribeSafe(subscriber, ctx0);
  | Source1Lift4(source, ctx0, op0, op1, op2, op3) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
    source |> subscribeSafe(subscriber, ctx0);
  | Source1Lift5(source, ctx0, op0, op1, op2, op3, op4) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
    source |> subscribeSafe(subscriber, ctx0);
  | Source1Lift6(source, ctx0, op0, op1, op2, op3, op4, op5) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
    source |> subscribeSafe(subscriber, ctx0);
  | Source1Lift7(source, ctx0, op0, op1, op2, op3, op4, op5, op6) =>
    let subscriber =
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
    source |> subscribeSafe(subscriber, ctx0);
  };

let subscribeWith = (~onNext, ~onComplete, observable) => {
  let subscriber = Subscriber.createAutoDisposing(~onNext, ~onComplete);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith1 = (~onNext, ~onComplete, ctx0, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing1(~onNext, ~onComplete, ctx0);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith2 = (~onNext, ~onComplete, ctx0, ctx1, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing2(~onNext, ~onComplete, ctx0, ctx1);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing3(~onNext, ~onComplete, ctx0, ctx1, ctx2);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
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
  subscriber |> Subscriber.asDisposable;
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
  subscriber |> Subscriber.asDisposable;
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
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith7 =
    (
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      observable,
    ) => {
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
  subscriber |> Subscriber.asDisposable;
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
  |> subscribeWith7(
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

let publishTo = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscription = observable |> subscribeWith(~onNext, ~onComplete);
        let newConnection =
          Disposable.create2(teardown, subscription, active);

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publishTo1 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscription =
          observable |> subscribeWith1(~onNext, ~onComplete, ctx0);
        let newConnection =
          Disposable.create2(teardown, subscription, active);

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
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