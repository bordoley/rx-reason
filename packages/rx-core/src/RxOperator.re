/** 
 * RxOperator function signature.
 *
 * Operator functions are chained together via the RxObservable.lift function,
 * decorating the RxObservable's subscriber. RxOperators are used to tranform 
 * the observed notifications.
 */;

/** The RxOperator type */
type t('a, 'b) = RxSubscriber.t('b) => RxSubscriber.t('a);