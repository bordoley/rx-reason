let read = (ref: ref('a)) : 'a => ref^;

let write = (value: 'a, ref: ref('a)) : unit => ref := value;

let writeTrue = (ref: ref(bool)) => ref := true;