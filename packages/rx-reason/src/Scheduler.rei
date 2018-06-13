type t = (unit => Disposable.t) => Disposable.t;

module type S = {
  let schedule: t;
};

let immediate: t;