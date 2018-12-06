module type S = {
  type t;

  include RxDisposableLike.S with type t := t;

  let addDisposable: (RxDisposable.t, t) => t;
  let removeDisposable: (RxDisposable.t, t) => t;
};

module type S1 = {
  type t('a);

  include RxDisposableLike.S1 with type t('a) := t('a);

  let addDisposable: (RxDisposable.t, t('a)) => t('a);
  let removeDisposable: (RxDisposable.t, t('a)) => t('a);
};

module type S2 = {
  type t('a, 'b);

  include RxDisposableLike.S2 with type t('a, 'b) := t('a, 'b);

  let addDisposable: (RxDisposable.t, t('a, 'b)) => t('a, 'b);
  let removeDisposable: (RxDisposable.t, t('a, 'b)) => t('a, 'b);
};