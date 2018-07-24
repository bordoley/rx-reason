type source('a) = Subscriber.t('a) => unit;
type source1('ctx0, 'a) = ('ctx0, Subscriber.t('a)) => unit;
type source2('ctx0, 'ctx1, 'a) = ('ctx0, 'ctx1, Subscriber.t('a)) => unit;
type source3('ctx0, 'ctx1, 'ctx2, 'a) =
  ('ctx0, 'ctx1, 'ctx2, Subscriber.t('a)) => unit;
type source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, Subscriber.t('a)) => unit;
type source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, Subscriber.t('a)) => unit;
type source6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, Subscriber.t('a)) => unit;
type source7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, Subscriber.t('a)) => unit;

type t('a) =
  | Source(source('b), Operator.t('b, 'a)): t('a)
  | Source1(source1('ctx0, 'b), 'ctx0, Operator.t('b, 'a)): t('a)
  | Source2(source2('ctx0, 'ctx1, 'b), 'ctx0, 'ctx1, Operator.t('b, 'a)): t(
                                                                    'a,
                                                                    )
  | Source3(
             source3('ctx0, 'ctx1, 'ctx2, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             Operator.t('b, 'a),
           ): t('a)
  | Source4(
             source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             Operator.t('b, 'a),
           ): t('a)
  | Source5(
             source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             Operator.t('b, 'a),
           ): t('a)
  | Source6(
             source6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             'ctx5,
             Operator.t('b, 'a),
           ): t('a)
  | Source7(
             source7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             'ctx5,
             'ctx6,
             Operator.t('b, 'a),
           ): t('a);

let create = onSubscribe => Source(onSubscribe, Functions.identity);
let create1 = (onSubscribe, ctx0) =>
  Source1(onSubscribe, ctx0, Functions.identity);
let create2 = (onSubscribe, ctx0, ctx1) =>
  Source2(onSubscribe, ctx0, ctx1, Functions.identity);
let create3 = (onSubscribe, ctx0, ctx1, ctx2) =>
  Source3(onSubscribe, ctx0, ctx1, ctx2, Functions.identity);
let create4 = (onSubscribe, ctx0, ctx1, ctx2, ctx3) =>
  Source4(onSubscribe, ctx0, ctx1, ctx2, ctx3, Functions.identity);
let create5 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4) =>
  Source5(onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, Functions.identity);
let create6 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
  Source6(
    onSubscribe,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    Functions.identity,
  );
let create7 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6) =>
  Source7(
    onSubscribe,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    ctx6,
    Functions.identity,
  );

let liftOperator = (op0, op1, subscriber) => op0 @@ op1 @@ subscriber;

let lift = (operator, observable) =>
  switch (observable) {
  | Source(source, op) => Source(source, liftOperator(op, operator))
  | Source1(source, ctx0, op) =>
    Source1(source, ctx0, liftOperator(op, operator))
  | Source2(source, ctx0, ctx1, op) =>
    Source2(source, ctx0, ctx1, liftOperator(op, operator))
  | Source3(source, ctx0, ctx1, ctx2, op) =>
    Source3(source, ctx0, ctx1, ctx2, liftOperator(op, operator))
  | Source4(source, ctx0, ctx1, ctx2, ctx3, op) =>
    Source4(source, ctx0, ctx1, ctx2, ctx3, liftOperator(op, operator))
  | Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4, op) =>
    Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4, liftOperator(op, operator))
  | Source6(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, op) =>
    Source6(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      liftOperator(op, operator),
    )
  | Source7(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, op) =>
    Source7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      liftOperator(op, operator),
    )
  };

let pipe2 = (op0, op1, observable)=> {
  let op = subscriber => op0 @@ op1 @@ subscriber;
  observable |> lift(op);
};

let pipe3 = (op0, op1, op2, observable)=> {
  let op = subscriber => op0 @@ op1 @@ op2 @@ subscriber;
  observable |> lift(op);
};

let pipe4 = (op0, op1, op2, op3, observable)=> {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
  observable |> lift(op);
};

let pipe5 = (op0, op1, op2, op3, op4, observable)=> {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
  observable |> lift(op);
};

let pipe6 = (op0, op1, op2, op3, op4, op5, observable)=> {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
  observable |> lift(op);
};

let pipe7 = (op0, op1, op2, op3, op4, op5, op6, observable)=> {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
  observable |> lift(op);
};

let never = Source(Functions.alwaysUnit1, Functions.identity);

let tryCompleteWithExceptionOrRaise = (exn, subscriber) => {
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
  | Source(source, op) =>
    let subscriber = op(subscriber);
    try (source(subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source1(source, ctx0, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source2(source, ctx0, ctx1, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source3(source, ctx0, ctx1, ctx2, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source4(source, ctx0, ctx1, ctx2, ctx3, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, ctx4, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source6(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source7(source, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, op) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
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