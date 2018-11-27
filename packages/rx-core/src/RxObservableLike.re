module type S1 = {
  type t('a);

  let subscribe: t('a) => RxDisposable.t;

  let subscribeWith: (RxSubscriber.t('a), t('a)) => unit;
};