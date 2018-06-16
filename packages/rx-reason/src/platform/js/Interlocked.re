let decrement = (ref: ref(int)): int => {
  decr(ref);
  ref^;
};
let increment = (ref: ref(int)): int => {
  incr(ref);
  ref^;
};
let exchange = (value: 'a, ref: ref('a)) : 'a => {
  let old = ref^;
  ref := value;
  old;
};