type t('a) =
  | Next('a)
  | Complete
  | CompleteWithException(exn);

let equals =
    (
      ~exnEquals=Functions.referenceEquality,
      ~nextEquals=Functions.referenceEquality,
      a,
      b,
    ) =>
  switch (a, b) {
  | (Next(a), Next(b)) => nextEquals(a, b)
  | (CompleteWithException(a), CompleteWithException(b)) => exnEquals(a, b)
  | (Complete, Complete) => true
  | _ => false
  };

let toString = (~exnToString=_ => "exn", ~nextToString, notif) =>
  switch (notif) {
  | Next(v) => "Next(" ++ nextToString(v) ++ ")"
  | CompleteWithException(exn) =>
    "CompleteWithException(" ++ exnToString(exn) ++ ")"
  | Complete => "Complete"
  };