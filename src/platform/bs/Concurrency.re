let interlockedExchange (ref: ref 'a) (value: 'a): 'a => {
  let old = !ref;
  ref := value;
  old
};

let volatileRead (ref: ref 'a): 'a => !ref;
