module type S1 = {
  type t('a);

  let subscribe: t('a) => Disposable.t;

  let subscribeWith: (Subscriber.t('a), t('a)) => unit;
};