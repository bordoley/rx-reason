/** 
 * Interoperability functions for converting between Promises and Singles.
 * 
 */;

/** 
 * Converts a JS Promise to a Single. 
 * 
 * Note: Due the underlying eagerness of JS Promises the resulting 
 * Single is effectively a hot Observable, and does not support
 * proper cancellation.
 */
let toSingle: Js.Promise.t('a) => RxReason.Single.t('a);


/**
 * Converts a Single to a Promise.
 */
let fromSingle: RxReason.Single.t('a) => Js.Promise.t('a);
