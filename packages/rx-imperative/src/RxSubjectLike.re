module type S = {
  type a;
  type t;

  include RxObservable.S with type a := a and type t := t;
  include RxObserverLike.S with type a := a and type t := t;
};

module type S1 = {
  type t('a);

  include RxObservable.S1 with type t('a) := t('a);
  include RxObserverLike.S1 with type t('a) := t('a);
};