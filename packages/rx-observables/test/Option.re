let equals = (~equals=(===), a, b) =>
  switch (a, b) {
  | (Some(a), Some(b)) => equals(a, b)
  | (None, None) => true
  | _ => false
  };

let toString = (~toString, x) =>
  switch (x) {
  | Some(v) => "Some(" ++ toString(v) ++ ")"
  | _ => "None"
  };