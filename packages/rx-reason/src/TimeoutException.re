exception TimeoutException;

let exn = TimeoutException;
let raise = () => raise(exn);