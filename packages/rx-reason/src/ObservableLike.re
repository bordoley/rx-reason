module type S1 = {
  type t('a);

  let publish:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a), unit) =>
    Disposable.t;

  let publish1:
    (
      ~onNext: ('ctx, 'a) => unit,
      ~onComplete: ('ctx, option(exn)) => unit,
      'ctx,
      t('a),
      unit
    ) =>
    Disposable.t;

  let subscribe: t('a) => Disposable.t;

  let subscribeWith: (Subscriber.t('a), t('a)) => unit;
};