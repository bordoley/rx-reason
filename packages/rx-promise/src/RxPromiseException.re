exception Exn(Js.Promise.error);

let raise = error => raise(Exn(error));