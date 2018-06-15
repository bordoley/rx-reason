/** 
 * An Observable that shares a single underlying subscription to the original source 
 * Observable. As long as there is at least one active subscription, a MulticastObservable 
 * will maintain a subscription to the original source and emit data. When all subscribers 
 * have disposed it will dispose the underlying subscription to the source.
 * */;

type t('a);

include Observable.S1 with type t('a) := t('a);

/**
 * Returns a new MulticastObservable that shares a single subscription to the provided source Observable. 
 */
let share: Observable.t('a) => t('a);

/**
 * Returns a new MulticastObservable that shares a single subscription to the provided source Observable. 
 * In addition, it replays the last n events each time it is subscribed to.
 */
let shareWithReplayBuffer: (int, Observable.t('a)) => t('a);