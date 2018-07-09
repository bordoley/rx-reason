type t('state, 'a) =
  (~delay: float, 'a, 'state, ('state, 'a) => unit) => Disposable.t;