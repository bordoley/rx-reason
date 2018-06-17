exception DisposedException;

let exn = DisposedException;
let raise = () => raise(exn);