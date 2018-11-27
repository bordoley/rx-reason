module type S = {
  type t;

  include RxDisposableLike.S with type t := t;

  let addTeardown: (RxDisposableTeardownLogic.t, t) => t;

  let addTeardown1: (RxDisposableTeardownLogic.t1('b), 'b, t) => t;

  let addTeardown2: (RxDisposableTeardownLogic.t2('b, 'c), 'b, 'c, t) => t;

  let addTeardown3: (RxDisposableTeardownLogic.t3('b, 'c, 'd), 'b, 'c, 'd, t) => t;

  let addTeardown4:
    (RxDisposableTeardownLogic.t4('b, 'c, 'd, 'e), 'b, 'c, 'd, 'e, t) => t;

  let addTeardown5:
    (RxDisposableTeardownLogic.t5('b, 'c, 'd, 'e, 'f), 'b, 'c, 'd, 'e, 'f, t) => t;

  let addTeardown6:
    (RxDisposableTeardownLogic.t6('b, 'c, 'd, 'e, 'f, 'g), 'b, 'c, 'd, 'e, 'f, 'g, t) => t;

  let addTeardown7:
    (
      RxDisposableTeardownLogic.t7('b, 'c, 'd, 'e, 'f, 'g, 'h),
      'b,
      'c,
      'd,
      'e,
      'f,
      'g,
      'h,
      t
    ) =>
    t;
};

module type S1 = {
  type t('a);

  include RxDisposableLike.S1 with type t('a) := t('a);

  let addTeardown: (RxDisposableTeardownLogic.t, t('a)) => t('a);

  let addTeardown1: (RxDisposableTeardownLogic.t1('b), 'b, t('a)) => t('a);

  let addTeardown2: (RxDisposableTeardownLogic.t2('b, 'c), 'b, 'c, t('a)) => t('a);

  let addTeardown3:
    (RxDisposableTeardownLogic.t3('b, 'c, 'd), 'b, 'c, 'd, t('a)) => t('a);

  let addTeardown4:
    (RxDisposableTeardownLogic.t4('b, 'c, 'd, 'e), 'b, 'c, 'd, 'e, t('a)) => t('a);

  let addTeardown5:
    (RxDisposableTeardownLogic.t5('b, 'c, 'd, 'e, 'f), 'b, 'c, 'd, 'e, 'f, t('a)) =>
    t('a);

  let addTeardown6:
    (
      RxDisposableTeardownLogic.t6('b, 'c, 'd, 'e, 'f, 'g),
      'b,
      'c,
      'd,
      'e,
      'f,
      'g,
      t('a)
    ) =>
    t('a);

  let addTeardown7:
    (
      RxDisposableTeardownLogic.t7('b, 'c, 'd, 'e, 'f, 'g, 'h),
      'b,
      'c,
      'd,
      'e,
      'f,
      'g,
      'h,
      t('a)
    ) =>
    t('a);
};