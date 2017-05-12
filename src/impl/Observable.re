type t 'a = (Observer.t 'a) => Subscription.t;

let subscribe (observer: Observer.t 'a) (observable: t 'a): Subscription.t =>
  observable observer;

let subscribeSafe (observer: Observer.t 'a) (observable: t 'a): Subscription.t =>
  try { observable |> subscribe observer }
  { | exn => observer |> Observer.onCompleted (Some exn); Subscription.empty };

let create (f: Observer.t 'a => Subscription.t): (t 'a) => fun (observer : Observer.t 'a) =>
  try { f observer }
  { | exn => observer |> Observer.onCompleted (Some exn); Subscription.empty };

let defer (f: unit => t 'a): (t 'a) => create (
  fun (observer : Observer.t 'a) => f () |> subscribe observer
);

exception ContinueException;

let doOnCompleted (onCompleted: (option exn) => unit) (observable: t 'a): (t 'a) => create (
  fun (observer: Observer.t 'a) => {
    let completed = ref false;

    let onCompleted exn => {
      let result = try { onCompleted exn; ContinueException } { | exn => exn };
      switch result {
        | ContinueException => observer |> Observer.onCompleted exn
        | exn => observer |> Observer.onCompleted (Some exn)
      };
      completed := true;
    };

    let observer = Observer.create onNext::observer.onNext ::onCompleted;

    let subscription = observable |> subscribeSafe observer;
    if (!completed) {
      subscription |> Subscription.dispose;
      Subscription.empty
    }
    else subscription;
  }
);

let doOnNext (onNext: 'a => unit) (observable: t 'a): (t 'a) => create (
  fun (observer: Observer.t 'a) => {
    let completed = ref false;
    let subscription = ref Subscription.empty;

    let onNext next => {
      let result = try { onNext next; ContinueException } { | exn => exn };
      switch result {
        | ContinueException => observer |> Observer.onNext next
        | exn =>
            observer |> Observer.onCompleted (Some exn);
            completed := true;
            Concurrency.volatileRead subscription |> Subscription.dispose;
      }
    };

    let observer = Observer.create ::onNext onCompleted::observer.onCompleted;

    subscription := observable |> subscribeSafe observer;
    if (!completed) {
      !subscription |> Subscription.dispose;
      Subscription.empty
    }
    else !subscription;
  }
);

let empty (): (t 'a) => fun (observer: Observer.t 'a) => {
  observer |> Observer.onCompleted None;
  Subscription.empty;
};

let first (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

exception FilterException;

let filter (f: 'a => bool) (observable: t 'a): (t 'a) => create (
  fun (observer: Observer.t 'a) => {
    let completed = ref false;
    let subscription = ref Subscription.empty;

    let onNext next => {
      let result = try { if (f next) ContinueException else FilterException } { | exn => exn };
      switch result {
        | ContinueException => observer |> Observer.onNext next
        | FilterException => ()
        | exn =>
            observer |> Observer.onCompleted (Some exn);
            completed := true;
            Concurrency.volatileRead subscription |> Subscription.dispose;
      }
    };
    let observer = Observer.create ::onNext onCompleted::observer.onCompleted;

    subscription := observable |> subscribeSafe observer;
    if (!completed) {
      !subscription |> Subscription.dispose;
      Subscription.empty
    }
    else !subscription;
  }
);

let combineLatest2With selector::(selector: 'a => 'b => 'c) (a: t 'a) (b: t 'b): (t 'c) => create (
  fun (observer: Observer.t 'acc) => {/*
    let lock = Concurrency.Lock.create ();

    let aValue = MutableOption.empty ();
    let bValue = MutableOption.empty ();

    let aIsCompleted = ref false;
    let bIsCompleted = ref false;

    let aOnCompleted exn => {
      Concurrency.interlockedExchange aIsCompleted true |> ignore;
      switch exn {
        | None => lock |> Concurrency.Lock.execute (
            fun () =>
              if (aValue |> MutableOption.isEmpty)

          )
        | Some exn =>
      }
    }

    let aSubscription = a |> subscribe (Observer.create)


    let criticalSection next => {
      if (prev |> MutableOption.isEmpty) {
        prev |> MutableOption.set next;
        true;
      } else { prev |> MutableOption.firstOrRaise |> equals next }
    };*/

    failwith "unimplemented";
  }
);

let combineLatest3With selector::(selector: 'a => 'b => 'c => 'd) (a: t 'a) (b: t 'b) (c: t 'c): (t 'd) =>
  failwith "unimplemented";

let combineLatest4With selector::(selector: 'a => 'b => 'c => 'd => 'e) (a: t 'a) (b: t 'b) (c: t 'c) (d: t 'd): (t 'e) =>
  failwith "unimplemented";

let combineLatest5With selector::(selector: 'a => 'b => 'c => 'd => 'e => 'f) (a: t 'a) (b: t 'b) (c: t 'c) (d: t 'd) (e: t 'e): (t 'f) =>
  failwith "unimplemented";

let distinctUntilChangedWith (equals: 'a => 'a => bool) (observable: t 'a): (t 'a) => create (
  fun (observer: Observer.t 'acc) => {
    let lock = Concurrency.Lock.create ();

    let prev = MutableOption.empty ();

    let criticalSection next => {
      if (prev |> MutableOption.isEmpty) {
        prev |> MutableOption.set next;
        true;
      } else { prev |> MutableOption.firstOrRaise |> equals next }
    };

    let filterFunction next => lock |> Concurrency.Lock.execute1 criticalSection next;

    observable |> filter filterFunction |> subscribe observer;
  }
);

let flatMap (f: 'a => t 'b) (observable: t 'a): (t 'b) =>
  failwith "unimplemented";

let flatten (observables: t (t 'a)): (t 'a) =>
  failwith "unimplemented";

let generate (f: 'a => 'a) (initialValue: 'a): (t 'a) =>
  failwith "unimplemented";

let map (f: 'a => 'b) (observable: t 'a): (t 'b) => create (
  fun (observer: Observer.t 'b) => {
    let completed = ref false;
    let subscription = ref Subscription.empty;

    let onNext next => try {
      let mapped = f next;
      observer |> Observer.onNext mapped;
    } { | exn =>
      observer |> Observer.onCompleted (Some exn);
      completed := true;
      Concurrency.volatileRead subscription |> Subscription.dispose;
    };

    let observer = Observer.create ::onNext onCompleted::observer.onCompleted;

    subscription := observable |> subscribe observer;
    if (!completed) {
      !subscription |> Subscription.dispose;
      Subscription.empty
    }
    else !subscription;
  }
);

let merge (observables: list (t 'a)): (t 'a) =>
  failwith "unimplemented";

let observeOn (scheduler: Scheduler.t) (observable: t 'a): (t 'a) => create (
  fun (observer: Observer.t 'a) => {
    /*
    let completed = ref false;
    let subscription = ref Subscription.empty;

    let onNext next => try {
      scheduler |> Scheduler.schedule (fun () => {
        observer |> Observer.onNext next;
        Subscription.empty;
      })
    } { | exn =>
      observer |> Observer.onCompleted (Some exn);
      completed := true;
      Concurrency.volatileRead subscription |> Subscription.dispose;
    };

    let observer = Observer.create ::onNext onCompleted::observer.onCompleted;

    subscription := observable |> subscribe observer;
    if (!completed) {
      !subscription |> Subscription.dispose;
      Subscription.empty
    }
    else !subscription;*/
    Subscription.empty;
  }
);

let return (value: 'a): (t 'a) => create (fun observer => {
  observer |> Observer.onNext value;
  Subscription.empty;
});

let scan (reducer: 'acc => 'a => 'acc) (initialValue: 'acc) (observable: t 'a): (t 'acc) => create (
  fun (observer: Observer.t 'acc) => {
    let lock = Concurrency.Lock.create ();

    let acc = ref initialValue;

    let criticalSection next => {
      let oldAcc = !acc;
      let nextAcc = reducer oldAcc next;
      acc := nextAcc;
      nextAcc
    };

    let mapper next => lock |> Concurrency.Lock.execute1 criticalSection next;

    observable |> map mapper |> subscribe observer;
  }
);

let skip (count: int) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let skipWhile (f: 'a => bool) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

let startWith (value: 'a) (observable: t 'a): (t 'a) => create (
  fun observer => {
    observer |> Observer.onNext value;
    observable |> subscribe observer;
  }
);

let take (count: int) (observable: t 'a): (t 'a) =>
  failwith "unimplemented";

exception CompleteException;

let takeWhile (f: 'a => bool) (observable: t 'a): (t 'a) => create (
  fun observer => {
    let completed = ref false;
    let subscription = ref Subscription.empty;

    let onNext next => {
      let result = try { if (f next) ContinueException else CompleteException } { | exn => exn };
      switch result {
        | ContinueException => observer |> Observer.onNext next
        | CompleteException =>
            observer |> Observer.onCompleted None;
            completed := true;
            Concurrency.volatileRead subscription |> Subscription.dispose;
        | exn =>
            observer |> Observer.onCompleted (Some exn);
            completed := true;
            Concurrency.volatileRead subscription |> Subscription.dispose;
      }
    };
    let observer = Observer.create ::onNext onCompleted::observer.onCompleted;

    subscription := observable |> subscribe observer;
    if (!completed) {
      !subscription |> Subscription.dispose;
      Subscription.empty
    }
    else !subscription;
  }
);

let subscribeDefault (observable: t 'a): Subscription.t =>
  observable |> subscribe (Observer.default ());

let subscribeOn (scheduler: Scheduler.t) (observable: t 'a): (t 'a) => create (
  fun observer => scheduler |> Scheduler.schedule (fun () => observable |> subscribe observer)
);

let toObservable (observable: t 'a): (t 'a) => observable;

let withLatestFrom selector::(selector: 'a => 'b => 'c) (b: t 'b) (a: t 'a): (t 'c) =>
    failwith "unimplemented";
