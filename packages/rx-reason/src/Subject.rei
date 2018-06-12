type t('a);

include Observer.S1 with type t('a) := t('a);

let create: unit => t('a);
let createWithReplayBuffer: int => t('a);
let disposed: t('a);
let share: Observable.t('a) => Observable.t('a);
let shareWithReplayBuffer: (int, Observable.t('a)) => Observable.t('a);
let toObservable: t('a) => Observable.t('a);