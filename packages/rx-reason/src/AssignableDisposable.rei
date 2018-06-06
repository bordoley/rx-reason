type t;

include Disposable.S with type t := t;

let create: unit => t;

let set: (Disposable.t, t) => unit;