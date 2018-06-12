type t('a);

include Observer.S1 with type t('a) := t('a);
include Observable.S1 with type t('a) := t('a);

let create: unit => t('a);
let createWithReplayBuffer: int => t('a);
let disposed: t('a);
let publish: (t('a), Observable.t('a), unit) => Disposable.t;
let toObservable: t('a) => Observable.t('a);