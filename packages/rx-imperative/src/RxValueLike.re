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