module type S = {
  type a;
  type t;

  include RxObservable.S with type a := a and type t := t;

  let update: (a => a, t) => unit;

  let update1: (('ctx0, a) => a, 'ctx0, t) => unit;

  let update2: (('ctx0, 'ctx1, a) => a, 'ctx0, 'ctx1, t) => unit;

  let update3: (('ctx0, 'ctx1, 'ctx2, a) => a, 'ctx0, 'ctx1, 'ctx2, t) => unit;

  let update4:
    (('ctx0, 'ctx1, 'ctx2, 'ctx3, a) => a, 'ctx0, 'ctx1, 'ctx2, 'ctx3, t) =>
    unit;

  let update5:
    (
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, a) => a,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t
    ) =>
    unit;
};

module type S1 = {
  type t('a);

  include RxObservable.S1 with type t('a) := t('a);

  let update: ('a => 'a, t('a)) => unit;

  let update1: (('ctx0, 'a) => 'a, 'ctx0, t('a)) => unit;

  let update2: (('ctx0, 'ctx1, 'a) => 'a, 'ctx0, 'ctx1, t('a)) => unit;

  let update3:
    (('ctx0, 'ctx1, 'ctx2, 'a) => 'a, 'ctx0, 'ctx1, 'ctx2, t('a)) => unit;

  let update4:
    (
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) => 'a,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      t('a)
    ) =>
    unit;

  let update5:
    (
      ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) => 'a,
      'ctx0,
      'ctx1,
      'ctx2,
      'ctx3,
      'ctx4,
      t('a)
    ) =>
    unit;
};