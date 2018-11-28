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