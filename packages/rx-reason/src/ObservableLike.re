module type S1 = {
  type t('a);

  let publish:
    (
      ~onNext: 'a => unit=?,
      ~onComplete: option(exn) => unit=?,
      t('a),
      unit
    ) =>
    Disposable.t;

  let publish1:
    (
      ~onNext: ('ctx, 'a) => unit=?,
      ~onComplete: ('ctx, option(exn)) => unit=?,
      'ctx,
      t('a),
      unit
    ) =>
    Disposable.t;

  let publishTo:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a), unit) =>
    Disposable.t;

  let publishTo1:
    (
      ~onNext: ('ctx, 'a) => unit,
      ~onComplete: ('ctx, option(exn)) => unit,
      'ctx,
      t('a),
      unit
    ) =>
    Disposable.t;

  let subscribe:
    (~onNext: 'a => unit=?, ~onComplete: option(exn) => unit=?, t('a)) =>
    CompositeDisposable.t;

  let subscribe1:
    (
      ~onNext: ('ctx0, 'a) => unit=?,
      ~onComplete: ('ctx0, option(exn)) => unit=?,
      'ctx0,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe2:
    (
      ~onNext: ('ctx0, 'ctx1, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe3:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      'ctx2,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe4:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe5:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit=?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe6:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit=?,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                   unit
                     =?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribe7:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) => unit=?,
      ~onComplete: (
                     'ctx0,
                     'ctx1,
                     'ctx2,
                     'ctx3,
                     'ctx4,
                     'ctx5,
                     'ctx6,
                     option(exn)
                   ) =>
                   unit
                     =?,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      'ctx6,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    CompositeDisposable.t;

  let subscribeWith1:
    (
      ~onNext: ('ctx0, 'a) => unit,
      ~onComplete: ('ctx0, option(exn)) => unit,
      'ctx0,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith2:
    (
      ~onNext: ('ctx0, 'ctx1, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, option(exn)) => unit,
      'ctx0,
      'ctx1,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith3:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, option(exn)) => unit,
      'ctx0,
      'ctx1,
      'ctx2,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith4:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, option(exn)) => unit,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith5:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, option(exn)) => unit,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith6:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'a) => unit,
      ~onComplete: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, option(exn)) =>
                   unit,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      t('a)
    ) =>
    CompositeDisposable.t;

  let subscribeWith7:
    (
      ~onNext: ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'a) => unit,
      ~onComplete: (
                     'ctx0,
                     'ctx1,
                     'ctx2,
                     'ctx3,
                     'ctx4,
                     'ctx5,
                     'ctx6,
                     option(exn)
                   ) =>
                   unit,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      'ctx5,
      'ctx6,
      t('a)
    ) =>
    CompositeDisposable.t;
};