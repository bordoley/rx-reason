let interlockedExchange (ref: ref 'a) (value: 'a): 'a => {
  let old = !ref;
  ref := value;
  old
};

let volatileRead (ref: ref 'a): 'a => !ref;

let module Lock = {
  type t = unit;

  let create (): t => ();

  let execute (f: unit => 'a) (lock: t): 'a =>
    f ();

  let execute1 (f: 'a => 'b) (arg: 'a) (lock: t): 'b =>
    f arg;
};
