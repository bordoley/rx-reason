type t 'a = {
  observer: Observer.t 'a,
  observable: Observable.t 'a,
};

let toObserver ({ observer }: t 'a): (Observer.t 'a) => observer;

let onCompleted (exn: option exn) ({ observer }: t 'a) =>
  observer |> Observer.onCompleted exn;

let onNext (next: 'a) ({ observer }: t 'a) =>
  observer |> Observer.onNext next;

let create (): (t 'a) =>
  failwith "not implemented";

let withInitialValue (value: 'a): (t 'a) =>
  failwith "not implemented";

let replayLast (bufferSize: int): (t 'a) =>
  failwith "not implemented";

let toObservable ({ observable }: t 'a): (Observable.t 'a) =>
  observable;

let subscribe (observer: Observer.t 'a) ({ observable }: t 'a): Subscription.t =>
  observable |> Observable.subscribe observer;

let module ObservableExtensions = {
  let share (observable: Observable.t 'a): (Observable.t 'a) =>
    failwith "not implemented";

  let shareWithInitialValue (value: 'a) (observable: Observable.t 'a): (Observable.t 'a) =>
    failwith "not implemented";

  let shareReplayLast (observable: Observable.t 'a): (Observable.t 'a) =>
    failwith "not implemented";
};
