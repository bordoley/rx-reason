let module Equality = {
  type t 'a = 'a => 'a => bool;
};

let module Streamable = {
  module type S1 = {
    type t 'a;

    let defer: (unit => t 'a) => (t 'a);
    let distinctUntilChangedWith: (Equality.t 'a) => (t 'a) => (t 'a);
    let doOnNext: ('a => unit) => (t 'a) => (t 'a);
    let filter: ('a => bool) => (t 'a) => (t 'a);
    let flatMap: ('a => t 'b) => (t 'a) => (t 'b);
    let flatten: (t (t 'a)) => (t 'a);
    let map: ('a => 'b) => (t 'a) => (t 'b);
    let scan: ('acc => 'a => 'acc) => 'acc => (t 'a) => (t 'acc);
    let skip: int => (t 'a) => (t 'a);
    let skipWhile: ('a => bool) => (t 'a) => (t 'a);
    let startWith: 'a => (t 'a) => (t 'a);
    let take: int => (t 'a) => (t 'a);
    let takeWhile: ('a => bool) => (t 'a) => (t 'a);
  };
};

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
