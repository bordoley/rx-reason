/**
 * A Disposable container that allows atomically updating/replacing the contained 
 * Disposable with another Disposable, disposing the old one when updating plus 
 * handling the disposition when the container itself is disposed.
 */;

type t;

include Disposable.S with type t := t;

let create: unit => t;

let get: t => Disposable.t;
let set: (Disposable.t, t) => unit;