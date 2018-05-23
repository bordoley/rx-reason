let exchange = (value: 'a, ref: ref('a)) : 'a => {
  let old = ref^;
  ref := value;
  old;
};