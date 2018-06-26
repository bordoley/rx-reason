type t('a);

include CompositeDisposable.S1 with type t('a) := t('a);

let asSubscriber: t('a) => Subscriber.t('a);
let delegate: Subscriber.t('a) => t('a);
let disposed: t('a);
let onSuccess: ('a, t('a)) => unit;
let onError: (exn, t('a)) => unit;