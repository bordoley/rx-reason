/** 
 * Interoperability functions for converting between Promises and Observables.
 * 
 */;

/** 
 * Converts a JS Promise to a Observable. 
 * 
 * Note: Due the underlying eagerness of JS Promises the resulting 
 * Observable is effectively a hot, and does not support
 * proper cancellation.
 */
let toObservable: Js.Promise.t('a) => RxReason.Observable.t('a);


/**
 * Converts an Observable to a Promise.
 */
let fromObservable: RxReason.Observable.t('a) => Js.Promise.t('a);
