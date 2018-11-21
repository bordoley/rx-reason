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

type observable('a) = t('a);

module type S1 = {
  type t('a);

  include ObservableLike.S1 with type t('a) := t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

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

let pipe2 = (op0, op1, observable) => {
  let op = subscriber => op0 @@ op1 @@ subscriber;
  observable |> lift(op);
};

let pipe3 = (op0, op1, op2, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ subscriber;
  observable |> lift(op);
};

let pipe4 = (op0, op1, op2, op3, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
  observable |> lift(op);
};

let pipe5 = (op0, op1, op2, op3, op4, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
  observable |> lift(op);
};

let pipe6 = (op0, op1, op2, op3, op4, op5, observable) => {
  let op = subscriber => op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
  observable |> lift(op);
};

let pipe7 = (op0, op1, op2, op3, op4, op5, op6, observable) => {
  let op = subscriber =>
    op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
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

let subscribeWith = (subscriber, observable) =>
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

let subscribe = (observable) => {
  let subscriber = Subscriber.create(~onNext=Functions.alwaysUnit1, ~onComplete=Functions.alwaysUnit1);

  subscribeWith(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};
