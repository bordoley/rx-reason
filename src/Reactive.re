let module Subscription = Subscription;

let module Scheduler = Scheduler;

let module Observer = {
  include Observer;

  module type S1 = {
    type t 'a;

    let onCompleted: (option exn) => (t 'a) => unit;
    let onNext: 'a  => (t 'a) => unit;
    let toObserver: (t 'a) => Observer.t 'a;
  };
};

let module Observable = {
  include Observable;
  include Subject.ObservableExtensions;

  module type S1 = {
    type t 'a;

    let subscribe: Observer.t 'a => t 'a => Subscription.t;
    let toObservable: (t 'a) => Observable.t 'a;
  };
};

let module AsyncIterable = {
  include AsyncIterable;

  module type S1 = {
    type t 'a;

    let reduce:
        while_::('acc => 'a => Observable.t bool)? =>
        ('acc => 'a => Observable.t 'acc) =>
        'acc =>
        (t 'a) =>
        Observable.t 'acc;
    let toAsyncIterable: t 'a => (AsyncIterable.t 'a);
  };
};

let module Subject = Subject;
