let module rec Equality: {
  type t 'a = 'a => 'a => bool;
};

let module rec Streamable: {
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

let module rec Subscription: {
  type t;

  let compose: (list Subscription.t) => Subscription.t;
  let create: (unit => unit) => Subscription.t;
  let dispose: Subscription.t => unit;
};

let module rec Scheduler: {
  type t;

  let immediate: Scheduler.t;
};

let module rec Observer: {
  module type S1 = {
    type t 'a;

    let onCompleted: (option exn) => (t 'a) => unit;
    let onNext: 'a  => (t 'a) => unit;
    let toObserver: (t 'a) => Observer.t 'a;
  };

  type t 'a;

  include S1 with type t 'a := Observer.t 'a;

  let create:
      onCompleted::(option exn => unit) =>
      onNext::('a => unit) =>
      (Observer.t 'a);

  let default: unit => (Observer.t 'a);
};

let module rec Observable: {
  module type S1 = {
    type t 'a;

    let subscribe: Observer.t 'a => t 'a => Subscription.t;
    let toObservable: (t 'a) => Observable.t 'a;
  };

  type t 'a;

  include S1 with type t 'a := t 'a;
  include Streamable.S1 with type t 'a := t 'a;

  let combineLatest2With:
    selector::('a => 'b => 'c) =>
    (Observable.t 'a) =>
    (Observable.t 'b) =>
    (Observable.t 'c);

  let combineLatest3With:
    selector::('a => 'b => 'c => 'd) =>
    (Observable.t 'a) =>
    (Observable.t 'b) =>
    (Observable.t 'c) =>
    (Observable.t 'd);

  let combineLatest4With:
    selector::('a => 'b => 'c => 'd => 'e) =>
    (Observable.t 'a) =>
    (Observable.t 'b) =>
    (Observable.t 'c) =>
    (Observable.t 'd) =>
    (Observable.t 'e);

  let combineLatest5With:
    selector::('a => 'b => 'c => 'd => 'e => 'f) =>
    (Observable.t 'a) =>
    (Observable.t 'b) =>
    (Observable.t 'c) =>
    (Observable.t 'd) =>
    (Observable.t 'e) =>
    (Observable.t 'f);

  let create: (Observer.t 'a => Subscription.t) => Observable.t 'a;

  let doOnCompleted: ((option exn) => unit) => (Observable.t 'a) => (Observable.t 'a);

  let empty: unit => (Observable.t 'a);

  let first: (Observable.t 'a) => (Observable.t 'a);

  let merge: (list (Observable.t 'a)) => (Observable.t 'a);

  let observeOn: Scheduler.t => (Observable.t 'a) => (Observable.t 'a);

  let share: (Observable.t 'a) => (Observable.t 'a);

  let shareWithInitialValue: 'a => (Observable.t 'a) => (Observable.t 'a);

  let shareReplayLast: (Observable.t 'a) => (Observable.t 'a);

  let subscribeDefault: Observable.t 'a => Subscription.t;

  let subscribeOn: Scheduler.t => Observable.t 'a => Observable.t 'a;

  let subscribeSafe: Observer.t 'a => Observable.t 'a => Subscription.t;

  let withLatestFrom:
    selector::('a => 'b => 'c) =>
    (Observable.t 'b) =>
    (Observable.t 'a) =>
    (Observable.t 'c);
};

let module rec Subject: {
  type t 'a;

  include Observable.S1 with type t 'a := Subject.t 'a;
  include Observer.S1 with type t 'a := Subject.t 'a;

  let create: unit => (Subject.t 'a);
  let withInitialValue: 'a => (Subject.t 'a);
  let replayLast: int => (Subject.t 'a);
};

let module rec AsyncIterable: {
  /* Functional back pressure for asynchronous sources that support back preassure */
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

  type t 'a;

  include S1 with type t 'a := AsyncIterable.t 'a;
  include Streamable.S1 with type t 'a := t 'a;
};
