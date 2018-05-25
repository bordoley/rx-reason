let identity: Operator.t('a, 'a) = observer => observer;

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
        ~onDispose=
          () => observer |> Observer.toDisposable |> Disposable.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let mapTo = (value: 'b) : Operator.t('a, 'b) => map((_) => value);

let doOnNext = (onNext: 'a => unit) : Operator.t('a, 'a) =>
  map(next => {
    onNext(next);
    next;
  });

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
        ~onDispose=
          () => observer |> Observer.toDisposable |> Disposable.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

/* Only public for testing, should rework the tests to avoid needing to expose this publicly in order to maintain the contract. */
exception EmptyException;

let defaultIfEmpty = (default: 'a) : Operator.t('a, 'b) =>
  (observer: Observer.t('a)) => {
    let isEmpty = ref(true);
    Observer.create(
      ~onNext=
        next => {
          observer |> Observer.next(next);
          isEmpty := false;
        },
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(EmptyException)
            | None =>
              if (isEmpty^) {
                observer |> Observer.next(default);
              };
              None;
            | Some(_) => exn
            };
          observer |> Observer.complete(exn);
        },
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
    );
  };

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
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
    );

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
        ~onDispose=
          () => observer |> Observer.toDisposable |> Disposable.dispose,
      );
    outerDisposable := outerObserver |> Observer.toDisposable;
    outerObserver;
  };

let firstOrNone = observer =>
  first @@ map(a => Some(a)) @@ defaultIfEmpty(None) @@ observer;

let maybeFirst = observer => first @@ maybe @@ observer;

let last = observer => {
  let last = MutableOption.empty();
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
              let lastValue = MutableOption.firstOrRaise(last);
              observer |> Observer.next(lastValue);
              None;
            }
          };
        observer |> Observer.complete(exn);
      },
    ~onDispose=
      () => {
        MutableOption.unset(last);
        observer |> Observer.toDisposable |> Disposable.dispose;
      },
  );
};

let lastOrNone = observer =>
  last @@ map(a => Some(a)) @@ defaultIfEmpty(None) @@ observer;

let maybeLast = observer => last @@ maybe @@ observer;

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

let distinctUntilChanged =
    (~comparer=Functions.referenceEquality)
    : Operator.t('a, 'a) => {
  let shouldUpdate = (a, b) => ! comparer(a, b);
  observer => {
    let state = MutableOption.empty();
    let predicate = next => MutableOption.setIf(shouldUpdate, next, state);
    keep(predicate, observer);
  };
};

let switch_: Operator.t(Observable.t('a), 'a) =
  observer => {
    let innerSubscription = ref(Disposable.disposed);
    let onComplete = exn => {
      observer |> Observer.complete(exn);
      innerSubscription^ |> Disposable.dispose;
    };
    let innerObserver =
      Observer.create(
        ~onNext=next => observer |> Observer.next(next),
        ~onComplete=
          exn =>
            switch (exn) {
            | Some(_) => onComplete(exn)
            | None => innerSubscription^ |> Disposable.dispose
            },
        ~onDispose=Functions.alwaysUnit,
      );
    let onNext = next =>
      try (
        {
          innerSubscription^ |> Disposable.dispose;
          innerSubscription :=
            next |> Observable.subscribeObserver(innerObserver);
        }
      ) {
      | exn => onComplete(Some(exn))
      };
    Observer.create(
      ~onComplete,
      ~onNext,
      ~onDispose=() => {
        innerObserver |> Observer.toDisposable |> Disposable.dispose;
        innerSubscription^ |> Disposable.dispose;
        innerSubscription := Disposable.disposed;
      },
    );
  };

let switchMap = (mapper: 'a => Observable.t('b)) : Operator.t('a, 'b) =>
  observer => map(mapper) @@ switch_ @@ observer;

/* FIXME: Should define a sane default DelayScheduler */
let debounceTime =
    (~scheduler: DelayScheduler.t, duration: float)
    : Operator.t('a, 'a) =>
  observer => {
    let lastValue = MutableOption.empty();
    let debounceSubscription = ref(Disposable.disposed);
    let clearDebounce = () => {
      let currentDebounceSubscription = debounceSubscription^;
      currentDebounceSubscription |> Disposable.dispose;
      debounceSubscription := Disposable.disposed;
    };
    let debouncedNext = () => {
      clearDebounce();
      if (MutableOption.isNotEmpty(lastValue)) {
        let next = lastValue^;
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
          lastValue := next;
          debounceSubscription := scheduler(~delay=duration, debouncedNext);
        },
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
    );
  };
/* bufferCount */
/* BufferSkipCount */
/* BufferTime */
/* withLatestFrom */