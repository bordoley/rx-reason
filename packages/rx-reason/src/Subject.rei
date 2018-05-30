type t('a);

let create: unit => t('a);
let createWithReplayBuffer: int => t('a);
let share: Observable.t('a) => Observable.t('a);
let shareWithReplayBuffer: (int, Observable.t('a)) => Observable.t('a);
let toObserver: t('a) => Observer.t('a);
let toObservable: t('a) => Observable.t('a);