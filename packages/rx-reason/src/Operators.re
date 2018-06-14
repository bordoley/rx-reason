exception EmptyException;
exception TimeoutException;

exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let (>>) =
    (o0: Operator.t('a, 'b), o1: Operator.t('b, 'c))
    : Operator.t('a, 'c) =>
  observer => o0(o1(observer));

let debounce = (scheduler: Scheduler.t) : Operator.t('a, 'a) =>
  observer => {
    let lastValue = MutableOption.create();
    let debounceSubscription = AssignableDisposable.create();

    let clearDebounce = () =>
      debounceSubscription |> AssignableDisposable.set(Disposable.disposed);

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
      ~onNext=
        next => {
          clearDebounce();
          MutableOption.set(next, lastValue);
          debounceSubscription
          |> AssignableDisposable.set(scheduler(debouncedNext));
        },
      ~onComplete=
        exn => {
          switch (exn) {
          | Some(_) => clearDebounce()
          | None => debouncedNext() |> ignore
          };
          observer |> Observer.complete(~exn?);
        },
      ~onDispose=
        () => {
          debounceSubscription |> AssignableDisposable.dispose;
          observer |> Observer.dispose;
        },
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
          observer |> Observer.complete(~exn?);
        },
      ~onDispose=Observer.forwardOnDispose(observer),
    );
  };

let exhaust: Operator.t(Observable.t('a), 'a) =
  observer => {
    let innerSubscription = AssignableDisposable.create();
    let exhaustObserver = ref(Observer.disposed);

    let hasActiveSubscription = () =>
      innerSubscription
      |> AssignableDisposable.get
      |> Disposable.isDisposed
      |> (!);

    let completeObserver = exn => {
      innerSubscription |> AssignableDisposable.dispose;
      observer |> Observer.complete(~exn?);
    };

    exhaustObserver :=
      Observer.create(
        ~onNext=
          next => {
            let hasActiveSubscription = hasActiveSubscription();
            if (! hasActiveSubscription) {
              let subscription =
                Observable.subscribeWithCallbacks(
                  ~onNext=Observer.forwardOnNext(observer),
                  ~onComplete=
                    exn =>
                      if (exhaustObserver^ |> Observer.isStopped) {
                        completeObserver(exn);
                      } else if (exn !== None) {
                        exhaustObserver^ |> Observer.complete(~exn?);
                      },
                  next,
                );
              innerSubscription |> AssignableDisposable.set(subscription);
            };
          },
        ~onComplete=
          exn => {
            let hasActiveSubscription = hasActiveSubscription();
            switch (exn) {
            | Some(_)
            | None when ! hasActiveSubscription => completeObserver(exn)
            | _ => ()
            };
          },
        ~onDispose=
          () => {
            innerSubscription |> AssignableDisposable.dispose;
            observer |> Observer.dispose;
          },
      );
    exhaustObserver^;
  };

let find = (predicate: 'a => bool, observer) => {
  let findObserver = ref(Observer.disposed);
  findObserver :=
    Observer.create(
      ~onNext=
        Functions.earlyReturnsUnit1(next => {
          let found =
            try (predicate(next)) {
            | exn =>
              findObserver^ |> Observer.complete(~exn);
              Functions.returnUnit();
            };
          if (found) {
            observer |> Observer.next(next);
            findObserver^ |> Observer.complete;
          };
        }),
      ~onComplete=Observer.forwardOnComplete(observer),
      ~onDispose=Observer.forwardOnDispose(observer),
    );
  findObserver^;
};

let first: Operator.t('a, 'a) =
  observer => {
    let firstObserver = ref(Observer.disposed);
    firstObserver :=
      Observer.create(
        ~onNext=
          next => {
            observer |> Observer.next(next);
            firstObserver^ |> Observer.complete(~exn=?completeWithoutErrorExn);
          },
        ~onComplete=
          exn => {
            let exn =
              switch (exn) {
              | Some(CompleteWithoutErrorException) => None
              | Some(_) => exn
              | _ => Some(EmptyException)
              };
            observer |> Observer.complete(~exn?);
          },
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    firstObserver^;
  };

let ignoreElements: Operator.t('a, 'a) =
  observer =>
    Observer.create(
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Observer.forwardOnComplete(observer),
      ~onDispose=Observer.forwardOnDispose(observer),
    );

let isEmpty: Operator.t('a, bool) =
  observer => {
    let isEmptyObserver = ref(Observer.disposed);
    isEmptyObserver :=
      Observer.create(
        ~onNext=
          _ => {
            observer |> Observer.next(false);
            isEmptyObserver^
            |> Observer.complete(~exn=?completeWithoutErrorExn);
          },
        ~onComplete=
          exn => {
            let exn =
              switch (exn) {
              | Some(CompleteWithoutErrorException) => None
              | Some(_) => exn
              | None =>
                observer |> Observer.next(true);
                exn;
              };
            observer |> Observer.complete(~exn?);
          },
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    isEmptyObserver^;
  };

let keep = (predicate: 'a => bool) : Operator.t('a, 'a) =>
  observer => {
    let keepObserver = ref(Observer.disposed);
    keepObserver :=
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let shouldKeep =
              try (predicate(next)) {
              | exn =>
                keepObserver^ |> Observer.complete(~exn);
                Functions.returnUnit();
              };
            if (shouldKeep) {
              observer |> Observer.next(next);
            };
          }),
        ~onComplete=Observer.forwardOnComplete(observer),
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    keepObserver^;
  };

let distinctUntilChanged =
    (~equals=Functions.referenceEquality)
    : Operator.t('a, 'a) => {
  let shouldUpdate = (a, b) => ! equals(a, b);
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
        observer |> Observer.complete(~exn?);
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
    let mapObserver = ref(Observer.disposed);
    mapObserver :=
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let mapped =
              try (mapper(next)) {
              | exn =>
                mapObserver^ |> Observer.complete(~exn);
                Functions.returnUnit();
              };
            observer |> Observer.next(mapped);
          }),
        ~onComplete=Observer.forwardOnComplete(observer),
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    mapObserver^;
  };

let firstOrNone = observer =>
  first @@ map(a => Some(a)) @@ defaultIfEmpty(None) @@ observer;

let lastOrNone = observer =>
  last @@ map(a => Some(a)) @@ defaultIfEmpty(None) @@ observer;

let mapTo = (value: 'b) : Operator.t('a, 'b) => map(_ => value);

let materialize = observer =>
  Observer.create(
    ~onNext=next => observer |> Observer.next(Notification.Next(next)),
    ~onComplete=
      exn => {
        let next =
          switch (exn) {
          | Some(exn) => Notification.CompleteWithException(exn)
          | None => Notification.Complete
          };
        observer |> Observer.next(next);
        observer |> Observer.complete;
      },
    ~onDispose=Observer.forwardOnDispose(observer),
  );

let maybe: Operator.t('a, 'a) =
  observer =>
    Observer.create(
      ~onNext=Observer.forwardOnNext(observer),
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(EmptyException) => None
            | _ => exn
            };
          observer |> Observer.complete(~exn?);
        },
      ~onDispose=Observer.forwardOnDispose(observer),
    );

let maybeFirst = observer => first @@ maybe @@ observer;

let maybeLast = observer => last @@ maybe @@ observer;

let none = (predicate: 'a => bool) : Operator.t('a, bool) =>
  observer => map(predicate) @@ keep(x => x) @@ isEmpty @@ observer;

let observe =
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit)
    : Operator.t('a, 'a) =>
  observer => {
    let observeObserver = ref(Observer.disposed);
    observeObserver :=
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            try (onNext(next)) {
            | exn =>
              observeObserver^ |> Observer.complete(~exn);
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
            observer |> Observer.complete(~exn?);
          },
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    observeObserver^;
  };

let observeOn =
    (
      ~bufferStrategy=BufferStrategy.Throw,
      ~bufferSize=(-1),
      scheduler: Scheduler.t,
    )
    : Operator.t('a, 'a) =>
  observer => {
    let queue =
      QueueWithBufferStrategy.create(~bufferStrategy, ~maxSize=bufferSize);
    let shouldComplete = ref(false);
    let completedState = ref(None);

    let wip = ref(0);
    let innerSubscription =
      Disposable.create(() => {
        Volatile.write(0, wip);
        queue |> QueueWithBufferStrategy.clear;
        completedState := None;
      });

    let rec doWorkStep = () =>
      switch (QueueWithBufferStrategy.tryDeque(queue)) {
      | _ when innerSubscription |> Disposable.isDisposed => Disposable.disposed
      | Some(next) =>
        observer |> Observer.next(next);
        Interlocked.decrement(wip) !== 0 ?
          scheduler(doWorkStep) : Disposable.disposed;
      | _ when Interlocked.exchange(false, shouldComplete) =>
        observer |> Observer.complete(~exn=?completedState^);
        innerSubscription |> Disposable.dispose;
        Disposable.disposed;
      | _ => Disposable.disposed
      };

    let schedule = () =>
      if (Interlocked.increment(wip) === 1) {
        scheduler(doWorkStep) |> ignore;
      };

    Observer.create(
      ~onNext=
        next => {
          queue |> QueueWithBufferStrategy.enqueue(next);
          schedule();
        },
      ~onComplete=
        exn => {
          shouldComplete := true;
          completedState := exn;
          schedule();
        },
      ~onDispose=
        () => {
          innerSubscription |> Disposable.dispose;
          observer |> Observer.dispose;
        },
    );
  };

let onComplete = (onComplete: option(exn) => unit) : Operator.t('a, 'a) =>
  observe(~onNext=Functions.alwaysUnit, ~onComplete);

let onNext = (onNext: 'a => unit) : Operator.t('a, 'a) =>
  observe(~onNext, ~onComplete=Functions.alwaysUnit);

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

let every = (predicate: 'a => bool, observer) => {
  let everyTrueObserver = ref(Observer.disposed);
  everyTrueObserver :=
    Observer.create(
      ~onNext=
        next =>
          if (! next) {
            observer |> Observer.next(next);
            everyTrueObserver^
            |> Observer.complete(~exn=?completeWithoutErrorExn);
          },
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(CompleteWithoutErrorException) =>
              observer |> Observer.next(false);
              None;
            | None =>
              observer |> Observer.next(true);
              None;
            | _ => exn
            };
          observer |> Observer.complete(~exn?);
        },
      ~onDispose=Observer.forwardOnDispose(observer),
    );
  everyTrueObserver^ |> map(predicate);
};

let some = (predicate: 'a => bool) : Operator.t('a, bool) =>
  observer => {
    let someTrueObserver = ref(Observer.disposed);
    someTrueObserver :=
      Observer.create(
        ~onNext=
          next =>
            if (next) {
              someTrueObserver^
              |> Observer.complete(~exn=?completeWithoutErrorExn);
            },
        ~onComplete=
          exn => {
            let exn =
              switch (exn) {
              | Some(CompleteWithoutErrorException) =>
                observer |> Observer.next(true);
                None;
              | None =>
                observer |> Observer.next(false);
                None;
              | _ => exn
              };
            observer |> Observer.complete(~exn?);
          },
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    someTrueObserver^ |> map(predicate);
  };

let switch_: Operator.t(Observable.t('a), 'a) =
  observer => {
    let switchObserver = ref(Observer.disposed);
    let innerSubscription = AssignableDisposable.create();
    let latest = ref(0);
    let lock = Lock.create();

    let doSubscribeInner = (id, next) => {
      innerSubscription |> AssignableDisposable.set(Disposable.disposed);
      let newInnerSubscription =
        Observable.subscribeWithCallbacks(
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
                if (latest^ === id) {
                  switchObserver^ |> Observer.complete(~exn?);
                }
              | None => ()
              },
          next,
        );
      innerSubscription |> AssignableDisposable.set(newInnerSubscription);
    };

    switchObserver :=
      Observer.create(
        ~onNext=
          next => {
            lock |> Lock.acquire;
            latest := latest^ + 1;
            let id = latest^;
            lock |> Lock.release;

            doSubscribeInner(id, next);
          },
        ~onComplete=
          exn => {
            lock |> Lock.acquire;
            innerSubscription |> AssignableDisposable.dispose;
            observer |> Observer.complete(~exn?);
            lock |> Lock.release;
          },
        ~onDispose=
          () => {
            innerSubscription |> AssignableDisposable.dispose;
            observer |> Observer.dispose;
          },
      );
    switchObserver^;
  };

let synchronize: Operator.t('a, 'a) =
  observer => {
    let gate = Lock.create();
    Observer.create(
      ~onComplete=
        exn => {
          Lock.acquire(gate);
          observer |> Observer.complete(~exn?);
          Lock.release(gate);
        },
      ~onNext=
        next => {
          Lock.acquire(gate);
          observer |> Observer.next(next);
          Lock.release(gate);
        },
      ~onDispose=Observer.forwardOnDispose(observer),
    );
  };

let timeout = (scheduler: Scheduler.t) : Operator.t('a, 'a) => {
  let timeoutObservable = Observable.raise(~scheduler, TimeoutException);
  observer => {
    let timeoutSubscription = AssignableDisposable.create();
    let timeOutObserver = ref(Observer.disposed);

    let subscribeToTimeout = () => {
      timeoutSubscription |> AssignableDisposable.set(Disposable.disposed);
      let subscription =
        Observable.subscribeWithCallbacks(
          ~onNext=_ => (),
          ~onComplete=exn => timeOutObserver^ |> Observer.complete(~exn?),
          timeoutObservable,
        );
      timeoutSubscription |> AssignableDisposable.set(subscription);
    };

    timeOutObserver :=
      Observer.create(
        ~onNext=
          next => {
            observer |> Observer.next(next);
            subscribeToTimeout();
          },
        ~onComplete=
          exn => {
            timeoutSubscription |> AssignableDisposable.dispose;
            observer |> Observer.complete(~exn?);
          },
        ~onDispose=
          () => {
            timeoutSubscription |> AssignableDisposable.dispose;
            observer |> Observer.dispose;
          },
      );

    subscribeToTimeout();
    timeOutObserver^;
  };
};

let withLatestFrom =
    (~selector: ('a, 'b) => 'c, other: Observable.t('b))
    : Operator.t('a, 'c) =>
  observer => {
    let otherLatest = MutableOption.create();

    let withLatestObserver = ref(Observer.disposed);
    let otherSubscription = ref(Disposable.disposed);

    withLatestObserver :=
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next =>
            if (MutableOption.isNotEmpty(otherLatest)) {
              let latest = otherLatest |> MutableOption.get;
              let nextWithLatest =
                try (selector(next, latest)) {
                | exn =>
                  withLatestObserver^ |> Observer.complete(~exn);
                  Functions.returnUnit();
                };
              observer |> Observer.next(nextWithLatest);
            }
          ),
        ~onComplete=
          exn => {
            observer |> Observer.complete(~exn?);
            otherSubscription^ |> Disposable.dispose;
          },
        ~onDispose=
          () => {
            otherSubscription^ |> Disposable.dispose;
            observer |> Observer.dispose;
          },
      );

    otherSubscription :=
      Observable.subscribeWithCallbacks(
        ~onNext=next => otherLatest |> MutableOption.set(next),
        ~onComplete=
          exn =>
            switch (exn) {
            | Some(_) => withLatestObserver^ |> Observer.complete(~exn?)
            | _ => ()
            },
        other,
      );

    withLatestObserver^;
  };