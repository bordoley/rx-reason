module type S1 = {
  type t('a);

  include RxObservable.S1 with type t('a) := t('a);

  let dispatch: ('a, t('a)) => unit;
};