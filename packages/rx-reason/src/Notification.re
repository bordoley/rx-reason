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

let equals =
    (
      ~exnEquals=Functions.referenceEquality,
      ~nextEquals=Functions.referenceEquality,
      a,
      b,
    ) =>
  switch (a, b) {
  | (Next(a), Next(b)) => nextEquals(a, b)
  | (Complete(Some(a)), Complete(Some(b))) => exnEquals(a, b)
  | (Complete(None), Complete(None)) => true
  | _ => false
  };

let toString = (~exnToString=_ => "exn", ~nextToString, notif) =>
  switch (notif) {
  | Next(v) => "Next(" ++ nextToString(v) ++ ")"
  | Complete(Some(exn)) =>
    "CompleteWithException(" ++ exnToString(exn) ++ ")"
  | Complete(None) => "Complete"
  };