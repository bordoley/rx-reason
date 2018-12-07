/**
 * A type that both an Observable sequence as well as an Subscriber.
 */;
type t('a);

include RxDisposable.S1 with type t('a) := t('a);
include RxSubjectLike.S1 with type t('a) := t('a);

let createMulticast: unit => t('a);
let createReplayBuffer: int => t('a);
let createReplayLast: unit => t('a);

/**
 * A disposed Subject instance.
 */
let disposed: t('a);