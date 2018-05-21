open Functions.Operators;

type t('a, 'b) = Observer.t('b) => Observer.t('a);

let pipe2 =
    (op0: t('a, 'b), op1: t('b, 'c))
    : t('a, 'c) =>
  op1 >> op0;

let pipe3 =
    (
      op0: t('a, 'b),
      op1: t('b, 'c),
      op2: t('c, 'd),
    )
    : t('a, 'd) =>
  op2 >> op1 >> op0;

let pipe4 =
    (
      op0: t('a, 'b),
      op1: t('b, 'c),
      op2: t('c, 'd),
      op3: t('d, 'e),
    )
    : t('a, 'e) =>
  op3 >> op2 >> op1 >> op0;

let pipe5 =
    (
      op0: t('a, 'b),
      op1: t('b, 'c),
      op2: t('c, 'd),
      op3: t('d, 'e),
      op4: t('e, 'f),
    )
    : t('a, 'f) =>
  op4 >> op3 >> op2 >> op1 >> op0;

let pipe6 =
    (
      op0: t('a, 'b),
      op1: t('b, 'c),
      op2: t('c, 'd),
      op3: t('d, 'e),
      op4: t('e, 'f),
      op5: t('f, 'g),
    )
    : t('a, 'c) =>
  op5 >> op4 >> op3 >> op2 >> op1 >> op0;

let pipe7 =
    (
      op0: t('a, 'b),
      op1: t('b, 'c),
      op2: t('c, 'd),
      op3: t('d, 'e),
      op4: t('e, 'f),
      op5: t('f, 'g),
      op6: t('g, 'h),
    )
    : t('a, 'h) =>
  op6 >> op5 >> op4 >> op3 >> op2 >> op1 >> op0;

let map = (mapper: 'a => 'b) : t('a, 'b) =>
  observer =>
    Observer.create(
      ~onNext=
        Functions.earlyReturnsUnit1(next => {
          let mapped =
            try (mapper(next)) {
            | exn =>
              observer |> Observer.complete(~exn=Some(exn));
              Functions.returnUnit();
            };
          observer |> Observer.next(mapped);
        }),
      ~onComplete=exn => observer |> Observer.complete(~exn),
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
      (),
    );

let mapTo = (value: 'b) : t('a, 'b) => map((_) => value);

let tap = (sideEffect: 'a => unit) : t('a, 'a) =>
  map(next => {
    sideEffect(next);
    next;
  });

let keep = (predicate: 'a => bool) : t('a, 'a) =>
  observer =>
    Observer.create(
      ~onNext=
        Functions.earlyReturnsUnit1(next => {
          let shouldKeep =
            try (predicate(next)) {
            | exn =>
              observer |> Observer.complete(~exn=Some(exn));
              Functions.returnUnit();
            };
          if (shouldKeep) {
            observer |> Observer.next(next);
          };
        }),
      ~onComplete=exn => observer |> Observer.complete(~exn),
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
      (),
    );

exception EmptyError;

let first: t('a, 'a) =
  observer => (
    Observer.create(
      ~onNext=
        next => {
          observer |> Observer.next(next);
          observer |> Observer.complete;
        },
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(_) => exn
            | _ => Some(EmptyError)
            };
          observer |> Observer.complete(~exn);
        },
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
      (),
    ):
      Observer.t('a)
  );

let last: t('a, 'a) =
  observer => {
    let last = MutableOption.empty();
    Observer.create(
      ~onNext=next => MutableOption.set(next, last),
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(_) => exn
            | _ =>
              if (MutableOption.isEmpty(last)) {
                Some(EmptyError);
              } else {
                let lastValue = MutableOption.firstOrRaise(last);
                observer |> Observer.next(lastValue);
                None;
              }
            };
          observer |> Observer.complete(~exn);
        },
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
      (),
    );
  };

let scan = (scanner: ('acc, 'a) => 'acc, initialValue: 'acc) : t('a, 'acc) =>
  observer => {
    let acc = ref(initialValue);
    let mapper = next => {
      let prevAcc = acc^;
      let nextAcc = scanner(prevAcc, next);
      acc := nextAcc;
      nextAcc;
    };
    map(mapper, observer);
  };

let distinctUntilChanged = (comparer: ('a, 'a) => bool) : t('a, 'a) => {
  let shouldUpdate = (a, b) => ! comparer(a, b);
  observer => {
    let state = MutableOption.empty();
    let predicate = next => MutableOption.setIf(shouldUpdate, next, state);
    keep(predicate, observer);
  };
};

let switch_: t(Observable.t('a), 'a) =
  observer => {
    let innerSubscription = ref(Disposable.disposed);
    let onComplete = exn => {
      observer |> Observer.complete(~exn);
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
        (),
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
      ~onDispose=
        () => {
          innerObserver |> Observer.toDisposable |> Disposable.dispose;
          innerSubscription^ |> Disposable.dispose;
          innerSubscription := Disposable.disposed;
        },
      (),
    );
  };

let switchMap = (mapper: 'a => Observable.t('b)) : t('a, 'b) =>
  pipe2(map(mapper), switch_);

let withLatestFrom =
    (other: Observable.t('b), selector: ('a, 'b) => 'c)
    : t('a, 'c) =>
  observer =>
    Observer.create(
      ~onComplete=exn => observer |> Observer.complete(~exn),
      ~onNext=next => (),
      ~onDispose=() => observer |> Observer.toDisposable |> Disposable.dispose,
      (),
    );