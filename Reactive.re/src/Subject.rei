type t('a);

let create: unit => t('a);
let share:
  (~createSubject: unit => t('a)=?, Observable.t('a)) => Observable.t('a);
let toObserver: t('a) => Observer.t('a);
let toObservable: t('a) => Observable.t('a);