/** 
 * Interoperability functions for converting between Promises and Singles.
 * 
 */
exception Exn(Js.Promise.error);

let raise = error => raise(Exn(error));