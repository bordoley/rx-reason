type t('a) = ref('a);

let make = v => ref(v);

let get = atomic => atomic^;

let set = (atomic, v) => atomic := v;

let setTrue = atomic => {
  atomic := true;
};

let compare_and_set = (atomic, vold, vnew) =>
  if (atomic^ === vold) {
    atomic := vnew;
    true;
  } else {
    false;
  };

let exchange = (atomic, v) => {
  let old = atomic^;
  atomic := v;
  old;
};

let incr = atomic => {
  incr(atomic);
  atomic^;
};

let decr = atomic => {
  decr(atomic);
  atomic^;
};