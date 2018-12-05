module type S1 = {
  type t('a);

  let dispatch: ('a, t('a)) => unit;
};