/**
 * Represents reactive event types and their values.
 */;
 
type t('a) =
  | Next('a)
  | Complete(option(exn));

let complete: (option(exn)) => t('a);
let next: 'a => t('a);

let equals:
  (
    ~exnEquals: (exn, exn) => bool=?,
    ~nextEquals: ('a, 'a) => bool=?,
    t('a),
    t('a)
  ) =>
  bool;

let toString:
  (~exnToString: exn => string=?, ~nextToString: 'a => string, t('a)) =>
  string;