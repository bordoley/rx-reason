type t('a);

let make: 'a => t('a);
let get: t('a) => 'a;
let set: (t('a), 'a) => unit;
let setTrue: t(bool) => unit;
let compare_and_set: (t('a), 'a, 'a) => bool;
let exchange: (t('a), 'a) => 'a;
let incr: t(int) => int;
let decr: t(int) => int;