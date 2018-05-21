let interlockedExchange = (value: 'a, ref: ref('a)) : 'a => {
  let old = ref^;
  ref := value;
  old;
};

let volatileRead = (ref: ref('a)) : 'a => ref^;

let volatileWrite = (value: 'a, ref: ref('a)) : unit => ref := value;

module Lock = {
  type t = unit;
  let create: unit => t = () => ();
  let execute = (f: unit => 'a, _: t) : 'a => f();
  let execute1 = (f: 'a => 'b, value: 'a, _: t) : 'a => f(value);
  let execute2 = (f: ('a, 'b) => 'c, a: 'a, b: 'b, _: t) : 'a => f(a, b);
};