module type S = {
  type a;
  type t;

  include RxObservable.S with type a := a and type t := t;

  let dispatch: ('a, t) => unit;
};

module type S1 = {
  type t('a);

  include RxObservable.S1 with type t('a) := t('a);

  let dispatch: ('a, t('a)) => unit;
};