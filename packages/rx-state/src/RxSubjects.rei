
/** Constructs a new Subject instance. */
let createMulticast: unit => RxSubject.t('a);

/**
 * Constructs a new Subject instance with the specified buffer size
 * which will replay the most recent buffered notifications when
 * subscribed to.
 * */
let createWithReplayBuffer: int => RxSubject.t('a);