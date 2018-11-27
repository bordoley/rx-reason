let alwaysFalse1 = _ => false;

let alwaysTrue1 = _ => true;

let alwaysUnit1 = _ => ();

let alwaysUnit: unit => unit = alwaysUnit1;

let alwaysUnit2 = (_, _) => ();

let alwaysUnit3 = (_, _, _) => ();

let alwaysUnit4 = (_, _, _, _) => ();

let alwaysUnit5 = (_, _, _, _, _) => ();

let alwaysUnit6 = (_, _, _, _, _, _) => ();

let alwaysUnit7 = (_, _, _, _, _, _, _) => ();

let alwaysUnit8 = (_, _, _, _, _, _, _, _) => ();

let identity = a => a;

let referenceEquality = (===);

let some = a => Some(a);

exception ReturnUnitException;

let returnUnit = () => raise(ReturnUnitException);

let earlyReturnsUnit = (call, ()) =>
  try (call()) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit1 = (call, a) =>
  try (call(a)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit2 = (call, a, b) =>
  try (call(a, b)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit3 = (call, a, b, c) =>
  try (call(a, b, c)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit4 = (call, a, b, c, d) =>
  try (call(a, b, c, d)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit5 = (call, a, b, c, d, e) =>
  try (call(a, b, c, d, e)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit6 = (call, a, b, c, d, e, f) =>
  try (call(a, b, c, d, e, f)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit7 = (call, a, b, c, d, e, f, g) =>
  try (call(a, b, c, d, e, f, g)) {
  | ReturnUnitException => ()
  };