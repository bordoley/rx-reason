type t('a) =
  | Next('a)
  | Complete
  | CompleteWithException(exn);

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