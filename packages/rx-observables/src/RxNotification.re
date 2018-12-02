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

