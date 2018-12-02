type source('a) = RxSubscriber.t('a) => unit;
type source1('ctx0, 'a) = ('ctx0, RxSubscriber.t('a)) => unit;
type source2('ctx0, 'ctx1, 'a) = ('ctx0, 'ctx1, RxSubscriber.t('a)) => unit;
type source3('ctx0, 'ctx1, 'ctx2, 'a) =
  ('ctx0, 'ctx1, 'ctx2, RxSubscriber.t('a)) => unit;
type source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, RxSubscriber.t('a)) => unit;
type source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, RxSubscriber.t('a)) => unit;

type t('a) =
  | Never
  | Source(source('b), RxOperator.t('b, 'a)): t('a)
  | Source1(source1('ctx0, 'b), RxOperator.t('b, 'a), 'ctx0): t('a)
  | Source2(source2('ctx0, 'ctx1, 'b), RxOperator.t('b, 'a), 'ctx0, 'ctx1): t(
                                                                    'a,
                                                                    )
  | Source3(
             source3('ctx0, 'ctx1, 'ctx2, 'b),
             RxOperator.t('b, 'a),
             'ctx0,
             'ctx1,
             'ctx2,
           ): t('a)
  | Source4(
             source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'b),
             RxOperator.t('b, 'a),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
           ): t('a)
  | Source5(
             source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'b),
             RxOperator.t('b, 'a),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
           ): t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  /** Cast to Observable.t. */
  let asObservable: t('a) => observable('a);
};

let create = onSubscribe => Source(onSubscribe, RxFunctions.identity);
let create1 = (onSubscribe, ctx0) =>
  Source1(onSubscribe, RxFunctions.identity, ctx0);
let create2 = (onSubscribe, ctx0, ctx1) =>
  Source2(onSubscribe, RxFunctions.identity, ctx0, ctx1);
let create3 = (onSubscribe, ctx0, ctx1, ctx2) =>
  Source3(onSubscribe, RxFunctions.identity, ctx0, ctx1, ctx2);
let create4 = (onSubscribe, ctx0, ctx1, ctx2, ctx3) =>
  Source4(onSubscribe, RxFunctions.identity, ctx0, ctx1, ctx2, ctx3);
let create5 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4) =>
  Source5(onSubscribe, RxFunctions.identity, ctx0, ctx1, ctx2, ctx3, ctx4);

let liftOperator = (op0, op1, subscriber) => op0 @@ op1 @@ subscriber;

let lift = (operator, observable) =>
  switch (observable) {
  | Never => Never
  | Source(source, op) => Source(source, liftOperator(op, operator))
  | Source1(source, op, ctx0) =>
    Source1(source, liftOperator(op, operator), ctx0)
  | Source2(source, op, ctx0, ctx1) =>
    Source2(source, liftOperator(op, operator), ctx0, ctx1)
  | Source3(source, op, ctx0, ctx1, ctx2) =>
    Source3(source, liftOperator(op, operator), ctx0, ctx1, ctx2)
  | Source4(source, op, ctx0, ctx1, ctx2, ctx3) =>
    Source4(source, liftOperator(op, operator), ctx0, ctx1, ctx2, ctx3)
  | Source5(source, op, ctx0, ctx1, ctx2, ctx3, ctx4) =>
    Source5(source, liftOperator(op, operator), ctx0, ctx1, ctx2, ctx3, ctx4)
  };

let never = Never;

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

let subscribe = observable => {
  let subscriber = RxSubscriber.create();

  switch (observable) {
  | Never => subscriber |> RxSubscriber.dispose;
  | Source(source, op) =>
    let subscriber = op(subscriber);
    try (source(subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source1(source, op, ctx0) =>
    let subscriber = op(subscriber);
    try (source(ctx0, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source2(source, op, ctx0, ctx1) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source3(source, op, ctx0, ctx1, ctx2) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source4(source, op, ctx0, ctx1, ctx2, ctx3) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  | Source5(source, op, ctx0, ctx1, ctx2, ctx3, ctx4) =>
    let subscriber = op(subscriber);
    try (source(ctx0, ctx1, ctx2, ctx3, ctx4, subscriber)) {
    | exn => subscriber |> tryCompleteWithExceptionOrRaise(exn)
    };
  };

  subscriber |> RxSubscriber.asDisposable;
};