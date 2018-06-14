let alwaysUnit = (_: 'a) : unit => ();

let compose = (f1: 'a => 'b, f2: 'b => 'c, a: 'a) : 'c => f2(f1(a));

let identity = (a: 'a) : 'a => a;

let referenceEquality = (===);

exception ReturnUnitException;

let returnUnit = () => raise(ReturnUnitException);

let earlyReturnsUnit = (f: unit => unit, ()) =>
  try (f()) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit1 = (f: 'a => unit, a: 'a) =>
  try (f(a)) {
  | ReturnUnitException => ()
  };

let earlyReturnsUnit2 = (f: ('a, 'b) => unit, a: 'a, b: 'b) =>
  try (f(a, b)) {
  | ReturnUnitException => ()
  };

let (>>) = (f1: 'a => 'b, f2: 'b => 'c) : ('a => 'c) => compose(f1, f2);