module type S1 = {
  type t('a);

  include RxObservable.S1 with type t('a) := t('a);
  include RxObserverLike.S1 with type t('a) := t('a);
};