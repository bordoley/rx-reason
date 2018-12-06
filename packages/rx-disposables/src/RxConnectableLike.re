module type S = {
  type t;

  let connect: t => RxDisposable.t;
};

module type S1 = {
  type t('a);

  let connect: t('a) => RxDisposable.t;
};

module type S2 = {
  type t('a, 'b);

  let connect: t('a, 'b) => RxDisposable.t;
};