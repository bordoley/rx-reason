let decrement = (ref: ref(int)): int => {
  ref := ref^ - 1;
  ref^;
};
let increment = (ref: ref(int)): int => {
  ref := ref^ + 1;
  ref^;
};
let exchange = (value: 'a, ref: ref('a)) : 'a => {
  let old = ref^;
  ref := value;
  old;
};