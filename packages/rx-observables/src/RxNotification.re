type t('a) =
  | Next('a)
  | Complete(option(exn));

let completeWithoutExn = Complete(None);
let complete = exn =>
  switch (exn) {
  | Some(_) => Complete(exn)
  | _ => completeWithoutExn
  };

let next = next => Next(next);

let map = (~onNext, ~onComplete) =>
  fun
  | Next(next) => onNext(next)
  | Complete(exn) => onComplete(exn);

let map1 = (~onNext, ~onComplete, ctx0) =>
  fun
  | Next(next) => onNext(ctx0, next)
  | Complete(exn) => onComplete(ctx0, exn);

let map2 = (~onNext, ~onComplete, ctx0, ctx1) =>
  fun
  | Next(next) => onNext(ctx0, ctx1, next)
  | Complete(exn) => onComplete(ctx0, ctx1, exn);