type t 'a = {
  onNext: 'a => unit,
  onCompleted: (option exn) => unit,
};

let create
    onCompleted::(onCompleted: (option exn) => unit)
    onNext::(onNext: 'a => unit): (t 'a) => {
  let isStopped = ref false;

  let onNext ev =>
    if (Concurrency.volatileRead isStopped |> not) (onNext ev)
    else ();

  let onCompleted exn =>  {
    let isStopped = Concurrency.interlockedExchange isStopped true |> not;

    if (not isStopped) (onCompleted exn)
    else ()
  };

  { onNext, onCompleted }
};

let defaultImpl = {
  onNext: fun _ => (),
  onCompleted: fun _ => (),
};

let default () => defaultImpl;

let onCompleted (exn: option exn) ({ onCompleted }: t 'a) => onCompleted exn;

let onNext (next: 'a) ({ onNext }: t 'a) => onNext next;

let toObserver (observer: t 'a): (t 'a) => observer;
