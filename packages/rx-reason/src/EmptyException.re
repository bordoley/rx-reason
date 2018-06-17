exception EmptyException;

let exn = EmptyException;
let raise = () => raise(exn);