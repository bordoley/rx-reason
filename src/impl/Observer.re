type t 'a = {
  onNext: 'a => unit,
  onCompleted: (option exn) => unit,
};

let create
    onCompleted::(onCompleted: (option exn) => unit)
    onNext::(onNext: 'a => unit): (t 'a) => {
  let isStopped = ref false;

  {
    onNext: fun ev =>
      if (Concurrency.volatileRead isStopped |> not) (onNext ev) else (),
    onCompleted: fun exn => {
      let isStopped = Concurrency.interlockedExchange isStopped true |> not;

      if (not isStopped) (onCompleted exn)
      else ()
    },
  }
};

let defaultImpl = {
  onNext: fun _ => (),
  onCompleted: fun _ => (),
};

let default () => defaultImpl;

let onCompleted (exn: option exn) (observer: t 'a) =>
  observer.onCompleted exn;

let onNext (next: 'a) (observer: t 'a) =>
  observer.onNext next;

let toObserver (observer: t 'a): (t 'a) => observer;
