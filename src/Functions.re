let alwaysUnit = (_: 'a) : unit => ();

let compose = (f1: 'a => 'b, f2: 'b => 'c, a: 'a) : 'c => f2(f1(a));

let identity = (a: 'a) : 'a => a;

let referenceEquality = (this: 'a, that: 'a) => this === that;

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

let tuple2 = (a, b) => (a, b);

let tuple3 = (a, b, c) => (a, b, c);

let tuple4 = (a, b, c, d) => (a, b, c, d);

let tuple5 = (a, b, c, d, e) => (a, b, c, d, e);

let tuple6 = (a, b, c, d, e, f) => (a, b, c, d, e, f);

let tuple7 = (a, b, c, d, e, f, g) => (a, b, c, d, e, f, g);

module Operators = {
  let (>>) = (f1: 'a => 'b, f2: 'b => 'c) : ('a => 'c) => compose(f1, f2);
};