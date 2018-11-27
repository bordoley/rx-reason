type source('a) = RxSubscriber.t('a) => unit;
type source1('ctx0, 'a) = ('ctx0, RxSubscriber.t('a)) => unit;
type source2('ctx0, 'ctx1, 'a) = ('ctx0, 'ctx1, RxSubscriber.t('a)) => unit;
type source3('ctx0, 'ctx1, 'ctx2, 'a) =
  ('ctx0, 'ctx1, 'ctx2, RxSubscriber.t('a)) => unit;
type source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, RxSubscriber.t('a)) => unit;
type source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, RxSubscriber.t('a)) => unit;
type source6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, RxSubscriber.t('a)) => unit;
type source7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, RxSubscriber.t('a)) =>
  unit;

type t('a) =
  | Source(source('b), RxOperator.t('b, 'a)): t('a)
  | Source1(source1('ctx0, 'b), 'ctx0, RxOperator.t('b, 'a)): t('a)
  | Source2(source2('ctx0, 'ctx1, 'b), 'ctx0, 'ctx1, RxOperator.t('b, 'a)): t(
                                                                    'a,
                                                                    )
  | Source3(
             source3('ctx0, 'ctx1, 'ctx2, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             RxOperator.t('b, 'a),
           ): t('a)
  | Source4(
             source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             RxOperator.t('b, 'a),
           ): t('a)
  | Source5(
             source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             RxOperator.t('b, 'a),
           ): t('a)
  | Source6(
             source6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'b),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
             'ctx5,
             RxOperator.t('b, 'a),
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
             RxOperator.t('b, 'a),
           ): t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  include RxObservableLike.S1 with type t('a) := t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

let create = onSubscribe => Source(onSubscribe, RxFunctions.identity);
let create1 = (onSubscribe, ctx0) =>
  Source1(onSubscribe, ctx0, RxFunctions.identity);
let create2 = (onSubscribe, ctx0, ctx1) =>
  Source2(onSubscribe, ctx0, ctx1, RxFunctions.identity);
let create3 = (onSubscribe, ctx0, ctx1, ctx2) =>
  Source3(onSubscribe, ctx0, ctx1, ctx2, RxFunctions.identity);
let create4 = (onSubscribe, ctx0, ctx1, ctx2, ctx3) =>
  Source4(onSubscribe, ctx0, ctx1, ctx2, ctx3, RxFunctions.identity);
let create5 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4) =>
  Source5(onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, RxFunctions.identity);
let create6 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5) =>
  Source6(
    onSubscribe,
    ctx0,
    ctx1,
    ctx2,
    ctx3,
    ctx4,
    ctx5,
    RxFunctions.identity,
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
    RxFunctions.identity,
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

let never = Source(RxFunctions.alwaysUnit1, RxFunctions.identity);

let tryCompleteWithExceptionOrRaise = (exn, subscriber) => {
  let shouldRaise =
    subscriber |> RxSubscriber.completeWithResult(~exn) |> (!);
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

let subscribe = observable => {
  let subscriber =
    RxSubscriber.create(
      ~onNext=RxFunctions.alwaysUnit1,
      ~onComplete=RxFunctions.alwaysUnit1,
    );

  subscribeWith(subscriber, observable);
  subscriber |> RxSubscriber.asDisposable;
};