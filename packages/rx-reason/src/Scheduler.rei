type t = (unit => Disposable.t) => Disposable.t;

let immediate: t;