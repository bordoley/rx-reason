let alwaysTrue1 = _ => true;

let alwaysUnit1 = _ => ();

let alwaysUnit: unit => unit = alwaysUnit1;

let alwaysUnit2 = (_, _) : unit => ();

let alwaysUnit3 = (_, _, _) : unit => ();

let alwaysUnit4 = (_, _, _, _) : unit => ();

let alwaysUnit5 = (_, _, _, _, _) : unit => ();

let identity = a => a;

let referenceEquality = (===);

exception ReturnUnitException;

let returnUnit = () => raise(ReturnUnitException);

let earlyReturnsUnit = (f, ()) =>
  try (f()) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit1 = (f, a) =>
  try (f(a)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit3 = (f, a, b, c) =>
  try (f(a, b, c)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit5 = (f, a, b, c, d, e) =>
  try (f(a, b, c, d, e)) {
  | ReturnUnitException => ()
  };

let (>>) = (f1, f2, a) => f2(f1(a));