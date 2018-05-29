exception EmptyException;

let bufferCount =
    (~size as _: int, ~startEvery as _: int=0)
    : Operator.t('a, array('a)) =>
  failwith("Not Implemented");

let bufferTime =
    (
      ~bufferCreationInterval as _: float=0.0,
      ~maxBufferSize as _: int,
      ~scheduler as _: DelayScheduler.t=DelayScheduler.default,
      ~timespan as _: float,
    )
    : Operator.t('a, list('a)) =>
  failwith("Not Implemented");

let debounceTime =
    (~scheduler=DelayScheduler.default, duration: float)
    : Operator.t('a, 'a) =>
  observer => {
    let lastValue = MutableOption.create();
    let debounceSubscription = ref(Disposable.disposed);
    let clearDebounce = () => {
      let currentDebounceSubscription = debounceSubscription^;
      currentDebounceSubscription |> Disposable.dispose;
      debounceSubscription := Disposable.disposed;
    };
    let debouncedNext = () => {
      clearDebounce();
      if (MutableOption.isNotEmpty(lastValue)) {
        let next = MutableOption.get(lastValue);
        MutableOption.unset(lastValue);
        observer |> Observer.next(next);
      };
      Disposable.disposed;
    };
    Observer.create(
      ~onComplete=
        exn => {
          switch (exn) {
          | Some(_) => clearDebounce()
          | None => debouncedNext() |> ignore
          };
          observer |> Observer.complete(exn);
        },
      ~onNext=
        next => {
          clearDebounce();
          MutableOption.set(next, lastValue);
          debounceSubscription := scheduler(~delay=duration, debouncedNext);
        },
      ~onDispose=() => observer |> Observer.dispose,
    );
  };

let defaultIfEmpty = (default: 'a) : Operator.t('a, 'b) =>
  (observer: Observer.t('a)) => {
    let isEmpty = ref(true);
    Observer.create(
      ~onNext=
        next => {
          observer |> Observer.next(next);
          Volatile.write(false, isEmpty);
        },
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(EmptyException)
            | None =>
              if (Volatile.read(isEmpty)) {
                observer |> Observer.next(default);
              };
              None;
            | Some(_) => exn
            };
          observer |> Observer.complete(exn);
        },
      ~onDispose=() => observer |> Observer.dispose,
    );
  };

let dispose = (disposable: Disposable.t) : Operator.t('a, 'a) =>
  observer =>
    Observer.create(
      ~onNext=next => observer |> Observer.next(next),
      ~onComplete=exn => observer |> Observer.complete(exn),
      ~onDispose=
        () => {
          disposable |> Disposable.dispose;
          observer |> Observer.dispose;
        },
    );

let every = (predicate: 'a => bool) => failwith("Not Implemented");

let exhaust: Operator.t(Observable.t('a), 'a) =
  _ => failwith("Not Implemented");

let find = (predicate: 'a => bool, observer) => {
  let outerDisposable = ref(Disposable.disposed);
  let outerObserver =
    Observer.create(
      ~onNext=
        next =>
          if (predicate(next)) {
            observer |> Observer.next(next);
            observer |> Observer.complete(None);
            outerDisposable^ |> Disposable.dispose;
          },
      ~onComplete=exn => observer |> Observer.complete(exn),
      ~onDispose=() => observer |> Observer.dispose,
    );
  outerDisposable := outerObserver |> Observer.toDisposable;
  outerObserver;
};

let first: Operator.t('a, 'a) =
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.create(
        ~onNext=
          next => {
            observer |> Observer.next(next);
            observer |> Observer.complete(None);
            outerDisposable^ |> Disposable.dispose;
          },
        ~onComplete=
          exn => {
            let exn =
              switch (exn) {
              | Some(_) => exn
              | _ => Some(EmptyException)
              };
            observer |> Observer.complete(exn);
          },
        ~onDispose=() => observer |> Observer.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let identity: Operator.t('a, 'a) = observer => observer;

let ignoreElements: Operator.t('a, unit) =
  observer =>
    Observer.create(
      ~onNext=Functions.alwaysUnit,
      ~onComplete=exn => observer |> Observer.complete(exn),
      ~onDispose=() => observer |> Observer.dispose,
    );

let isEmpty: Operator.t('a, bool) =
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.create(
        ~onNext=
          _ => {
            observer |> Observer.next(false);
            observer |> Observer.complete(None);
            outerDisposable^ |> Disposable.dispose;
          },
        ~onComplete=
          exn => {
            if (exn === None) {
              observer |> Observer.next(false);
            };
            observer |> Observer.complete(exn);
          },
        ~onDispose=() => observer |> Observer.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let keep = (predicate: 'a => bool) : Operator.t('a, 'a) =>
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let shouldKeep =
              try (predicate(next)) {
              | exn =>
                observer |> Observer.complete(Some(exn));
                outerDisposable^ |> Disposable.dispose;
                Functions.returnUnit();
              };
            if (shouldKeep) {
              observer |> Observer.next(next);
            };
          }),
        ~onComplete=exn => observer |> Observer.complete(exn),
        ~onDispose=() => observer |> Observer.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let distinctUntilChanged =
    (~comparer=Functions.referenceEquality)
    : Operator.t('a, 'a) => {
  let shouldUpdate = (a, b) => ! comparer(a, b);
  observer => {
    let state = MutableOption.create();
    let predicate = next => MutableOption.setIf(shouldUpdate, next, state);
    keep(predicate, observer);
  };
};

let last = observer => {
  let last = MutableOption.create();
  Observer.create(
    ~onNext=next => MutableOption.set(next, last),
    ~onComplete=
      exn => {
        let exn =
          switch (exn) {
          | Some(_) => exn
          | None =>
            if (MutableOption.isEmpty(last)) {
              Some(EmptyException);
            } else {
              let lastValue = MutableOption.get(last);
              observer |> Observer.next(lastValue);
              None;
            }
          };
        observer |> Observer.complete(exn);
      },
    ~onDispose=
      () => {
        MutableOption.unset(last);
        observer |> Observer.dispose;
      },
  );
};

let map = (mapper: 'a => 'b) : Operator.t('a, 'b) =>
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let mapped =
              try (mapper(next)) {
              | exn =>
                observer |> Observer.complete(Some(exn));
                outerDisposable^ |> Disposable.dispose;
                Functions.returnUnit();
              };
            observer |> Observer.next(mapped);
          }),
        ~onComplete=exn => observer |> Observer.complete(exn),
        ~onDispose=() => observer |> Observer.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let mapTo = (value: 'b) : Operator.t('a, 'b) => map(_ => value);

let firstOrNone = observer =>
  first @@ map(a => Some(a)) @@ defaultIfEmpty(None) @@ observer;

let lastOrNone = observer =>
  last @@ map(a => Some(a)) @@ defaultIfEmpty(None) @@ observer;

let maybe: Operator.t('a, 'a) =
  observer =>
    Observer.create(
      ~onNext=next => observer |> Observer.next(next),
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(EmptyException) => None
            | _ => exn
            };
          observer |> Observer.complete(exn);
        },
      ~onDispose=() => observer |> Observer.dispose,
    );

let maybeFirst = observer => first @@ maybe @@ observer;

let maybeLast = observer => last @@ maybe @@ observer;

let none = (predicate: 'a => bool) : Operator.t('a, bool) =>
  observer => map(predicate) @@ keep(x => x) @@ isEmpty @@ observer;

let observe =
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit)
    : Operator.t('a, 'a) =>
  observer => {
    let outerDisposable = ref(Disposable.disposed);
    let outerObserver =
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            try (onNext(next)) {
            | exn =>
              observer |> Observer.complete(Some(exn));
              outerDisposable^ |> Disposable.dispose;
              Functions.returnUnit();
            };
            observer |> Observer.next(next);
          }),
        ~onComplete=
          exn => {
            let exn =
              try (
                {
                  onComplete(exn);
                  exn;
                }
              ) {
              | exn => Some(exn)
              };
            observer |> Observer.complete(exn);
          },
        ~onDispose=() => observer |> Observer.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let observeOn = (_: Scheduler.t) : Operator.t('a, 'a) =>
  failwith("Not Implemented");

let onComplete = (onComplete: option(exn) => unit) : Operator.t('a, 'a) =>
  observer =>
    Observer.create(
      ~onNext=next => observer |> Observer.next(next),
      ~onComplete=
        exn => {
          let exn =
            try (
              {
                onComplete(exn);
                exn;
              }
            ) {
            | exn => Some(exn)
            };
          observer |> Observer.complete(exn);
        },
      ~onDispose=() => observer |> Observer.dispose,
    );

let onDispose = (dispose: unit => unit) : Operator.t('a, 'a) =>
  observer =>
    Observer.create(
      ~onNext=next => observer |> Observer.next(next),
      ~onComplete=exn => observer |> Observer.complete(exn),
      ~onDispose=
        () => {
          dispose();
          observer |> Observer.dispose;
        },
    );

let onNext = (onNext: 'a => unit) : Operator.t('a, 'a) =>
  map(next => {
    onNext(next);
    next;
  });

let scan =
    (scanner: ('acc, 'a) => 'acc, initialValue: 'acc)
    : Operator.t('a, 'acc) =>
  observer => {
    let acc = ref(initialValue);
    let mapper = next => {
      let prevAcc = acc^;
      let nextAcc = scanner(prevAcc, next);
      acc := nextAcc;
      nextAcc;
    };
    observer |> Observer.next(initialValue);
    map(mapper, observer);
  };

let some = (_: 'a => bool) : Operator.t('a, bool) =>
  failwith("Not Implemented");

let switch_: Operator.t(Observable.t('a), 'a) =
  observer => {
    let innerSubscription = AssignableDisposable.create();
    let outerSubscription = ref(Disposable.disposed);
    let latest = ref(0);
    let lock = Lock.create();

    let doSubscribeInner = (id, next) => {
      innerSubscription |> AssignableDisposable.set(Disposable.disposed);
      let newInnerSubscription =
        next
        |> Observable.subscribeWithCallbacks(
             ~onNext=
               next => {
                 lock |> Lock.acquire;
                 if (latest^ === id) {
                   observer |> Observer.next(next);
                 };
                 lock |> Lock.release;
               },
             ~onComplete=
               exn =>
                 switch (exn) {
                 | Some(_) =>
                   lock |> Lock.acquire;
                   if (latest^ === id) {
                     observer |> Observer.complete(exn);
                     outerSubscription^ |> Disposable.dispose;
                   };
                   lock |> Lock.release;
                 | None => ()
                 },
           );
      innerSubscription |> AssignableDisposable.set(newInnerSubscription);
    };

    let observer =
      Observer.create(
        ~onComplete=
          exn => {
            lock |> Lock.acquire;
            observer |> Observer.complete(exn);
            lock |> Lock.release;
          },
        ~onNext=
          next => {
            lock |> Lock.acquire;
            latest := latest^ + 1;
            let id = latest^;
            lock |> Lock.release;

            doSubscribeInner(id, next);
          },
        ~onDispose=
          () => {
            innerSubscription |> AssignableDisposable.dispose;
            observer |> Observer.dispose;
          },
      );

    outerSubscription := observer |> Observer.toDisposable;
    observer;
  };

let synchronize: Operator.t('a, 'a) =
  observer => {
    let gate = Lock.create();
    Observer.create(
      ~onComplete=
        exn => {
          Lock.acquire(gate);
          observer |> Observer.complete(exn);
          Lock.release(gate);
        },
      ~onNext=
        next => {
          Lock.acquire(gate);
          observer |> Observer.next(next);
          Lock.release(gate);
        },
      ~onDispose=() => observer |> Observer.dispose,
    );
  };

let timeout =
    (~scheduler as _: DelayScheduler.t=DelayScheduler.default, _: float)
    : Operator.t('a, 'a) =>
  failwith("Not Implemented");

let withLatestFrom =
    (~selector: ('a, 'b) => 'c, other: Observable.t('b))
    : Operator.t('a, 'c) =>
  observer => {
    let otherLatest = MutableOption.create();
    let otherSubscription = AssignableDisposable.create();

    let withLatestObserver =
      Observer.create(
        ~onNext=
          next =>
            if (MutableOption.isNotEmpty(otherLatest)) {
              let latest = otherLatest |> MutableOption.get;
              let nextWithLatest = selector(next, latest);
              observer |> Observer.next(nextWithLatest);
            },
        ~onComplete=exn => observer |> Observer.complete(exn),
        ~onDispose=
          () => {
            otherSubscription |> AssignableDisposable.dispose;
            observer |> Observer.dispose;
          },
      );
    AssignableDisposable.set(
      other
      |> Observable.subscribeWithCallbacks(
           ~onNext=next => otherLatest |> MutableOption.set(next),
           ~onComplete=
             exn =>
               switch (exn) {
               | Some(_) => observer |> Observer.complete(exn)
               | _ => otherSubscription |> AssignableDisposable.dispose
               },
         ),
      otherSubscription,
    );

    withLatestObserver;
  };