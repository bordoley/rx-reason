let alwaysTrue = _ => true;

let alwaysUnit = (_: 'a) : unit => ();

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

let (>>) = (f1: 'a => 'b, f2: 'b => 'c) : ('a => 'c) => a => f2(f1(a));