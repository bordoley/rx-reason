type t('a) =
  (~onNext: 'a => unit, ~onComplete: option(exn) => unit) => Disposable.t;

let subscribe = (~onNext, ~onComplete, observable: t('a)) : Disposable.t =>
  observable(~onNext, ~onComplete);

let subscribeObserver =
    (observer: Observer.t('a), observable: t('a))
    : Disposable.t =>
  subscribe(
    ~onNext=next => observer |> Observer.next(next),
    ~onComplete=exn => observer |> Observer.complete(exn),
    observable,
  );

let subscribeOn = (_: Scheduler.t, _: t('a)) : t('a) =>
  failwith("Not implemented");

let createWithObserver =
    (onSubscribe: Observer.t('a) => Disposable.t)
    : t('a) =>
  (~onNext, ~onComplete) => {
    let subscription = ref(Disposable.disposed);
    let observer =
      Observer.create(~onNext, ~onComplete, ~onDispose=() =>
        Interlocked.exchange(Disposable.disposed, subscription)
        |> Disposable.dispose
      );
    subscription :=
      (
        try (onSubscribe(observer)) {
        | exn =>
          let shouldRaise =
            observer |> Observer.completeWithResult(Some(exn)) |> (!);
          if (shouldRaise) {
            /* This could happen when the onComplete is called synchronously in the
             * subscribe function which also throws.
             */
            raise(
              exn,
            );
          };
          Disposable.disposed;
        }
      );
    observer |> Observer.toDisposable;
  };

let create = onSubscribe : t('a) =>
  createWithObserver(observer => subscribeObserver(observer, onSubscribe));

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  createWithObserver(observer => {
    let lifted = operator(observer);
    let subscription = observable |> subscribeObserver(lifted);
    Disposable.compose([subscription, lifted |> Observer.toDisposable]);
  });

let concat =
    (~scheduler=Scheduler.immediate, observables: list(t('a)))
    : t('a) =>
  create((~onNext, ~onComplete) => {
    let remaining = ref(observables);
    let innerSubscription = ref(Disposable.disposed);
    let rec scheduleSubscription = () =>
      scheduler(() =>
        switch (remaining^) {
        | [hd, ...tail] =>
          remaining := tail;
          doSubscribe(hd);
        | [] =>
          onComplete(None);
          Disposable.disposed;
        }
      )
    and doSubscribe = observable =>
      observable
      |> subscribe(~onNext, ~onComplete=exn =>
           switch (exn) {
           | Some(_) => onComplete(exn)
           | None =>
             Interlocked.exchange(scheduleSubscription(), innerSubscription)
             |> Disposable.dispose
           }
         );
    innerSubscription := scheduleSubscription();
    Disposable.create(() =>
      Interlocked.exchange(Disposable.disposed, innerSubscription)
      |> Disposable.dispose
    );
  });

let defer = (f: unit => t('a)) : t('a) =>
  create((~onNext, ~onComplete) => f() |> subscribe(~onNext, ~onComplete));

let empty = (~scheduler=Scheduler.immediate, ()) =>
  scheduler === Scheduler.immediate ?
    create((~onNext as _, ~onComplete) => {
      onComplete(None);
      Disposable.disposed;
    }) :
    create((~onNext as _, ~onComplete) =>
      scheduler(() => {
        onComplete(None);
        Disposable.disposed;
      })
    );

let merge =
    /* FIXME: Should use a trampoline scheduler as the default for this function */
    (~scheduler as _=Scheduler.immediate, _: list(t('a)))
    : t('a) =>
  failwith("Not implemented");

let never = () : t('a) =>
  create((~onNext as _, ~onComplete as _) => Disposable.empty());

let ofList = (~scheduler=Scheduler.immediate, list: list('a)) : t('a) =>
  scheduler === Scheduler.immediate ?
    create((~onNext, ~onComplete) => {
      let rec loop = list =>
        switch (list) {
        | [hd, ...tail] =>
          onNext(hd);
          loop(tail);
        | [] => onComplete(None)
        };
      loop(list);
      Disposable.disposed;
    }) :
    create((~onNext, ~onComplete) => {
      let rec loop = (list, ()) =>
        switch (list) {
        | [hd, ...tail] =>
          onNext(hd);
          scheduler(loop(tail));
        | [] =>
          onComplete(None);
          Disposable.disposed;
        };
      scheduler(loop(list));
    });

let ofValue = (~scheduler=Scheduler.immediate, value: 'a) : t('a) =>
  scheduler === Scheduler.immediate ?
    create((~onNext, ~onComplete) => {
      onNext(value);
      onComplete(None);
      Disposable.disposed;
    }) :
    create((~onNext, ~onComplete) =>
      scheduler(() => {
        onNext(value);
        scheduler(() => {
          onComplete(None);
          Disposable.disposed;
        });
      })
    );

let startWithList =
    (~scheduler=Scheduler.immediate, values: list('a), observable: t('a)) =>
  concat([ofList(~scheduler, values), observable]);

let startWithValue =
    (~scheduler=Scheduler.immediate, value: 'a, observable: t('a)) =>
  concat([ofValue(~scheduler, value), observable]);

let combineLatest2 =
    (
      ~scheduler=Scheduler.immediate,
      ~selector: ('a, 'b) => 'c,
      observable0: t('a),
      observable1: t('b),
    )
    : t('c) =>
  failwith("Not implemented");

let combineLatest3 =
    (
      ~scheduler=Scheduler.immediate,
      ~selector: ('a, 'b, 'c) => 'd,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
    )
    : t('d) =>
  failwith("Not implemented");

let combineLatest4 =
    (
      ~scheduler=Scheduler.immediate,
      ~selector: ('a, 'b, 'c, 'd) => 'e,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
    )
    : t('e) =>
  failwith("Not implemented");

let combineLatest5 =
    (
      ~scheduler=Scheduler.immediate,
      ~selector: ('a, 'b, 'c, 'd, 'e) => 'f,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
      observable4: t('e),
    )
    : t('f) =>
  failwith("Not implemented");

let combineLatest6 =
    (
      ~scheduler=Scheduler.immediate,
      ~selector: ('a, 'b, 'c, 'd, 'e, 'f) => 'g,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
      observable4: t('e),
      observable5: t('f),
    )
    : t('g) =>
  failwith("Not implemented");

let combineLatest7 =
    (
      ~scheduler=Scheduler.immediate,
      ~selector: ('a, 'b, 'c, 'd, 'e, 'f, 'g) => 'h,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
      observable3: t('d),
      observable4: t('e),
      observable5: t('f),
      observable6: t('g),
    )
    : t('h) =>
  failwith("Not implemented");