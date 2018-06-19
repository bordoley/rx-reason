type source('a) = Observer.t('a) => unit;

type t('a) =
  | Source(source('a)): t('a)
  | Lift1(source('b), Operator.t('b, 'a)): t('a)
  | Lift2(source('c), Operator.t('c, 'b), Operator.t('b, 'a)): t('a)
  | Lift3(
           source('d),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Lift4(
           source('e),
           Operator.t('e, 'd),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a);

let create = onSubscribe => Source(onSubscribe);

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  switch (observable) {
  | Source(source) => Lift1(source, operator)
  | Lift1(source, op0) => Lift2(source, op0, operator)
  | Lift2(source, op0, op1) => Lift3(source, op0, op1, operator)
  | Lift3(source, op0, op1, op2) => Lift4(source, op0, op1, op2, operator)
  | Lift4(source, op0, op1, op2, op3) =>
    let op0 = observer => op0 @@ op1 @@ op2 @@ op3 @@ operator @@ observer;
    Lift1(source, op0);
  };

let never = Source(Functions.alwaysUnit);

let subscribeSafe = (observer, source) =>
  try (source(observer)) {
  | exn =>
    let shouldRaise = observer |> Observer.completeWithResult(~exn) |> (!);
    if (shouldRaise) {
      /* This could happen when the onComplete is called synchronously in the
       * subscribe function which also throws.
       */
      raise(
        exn,
      );
    };
  };

let subscribeObserver = (observer, observable) =>
  switch (observable) {
  | Source(source) => source |> subscribeSafe(observer)
  | Lift1(source, operator) =>
    let observer = operator(observer);
    source |> subscribeSafe(observer);
  | Lift2(source, op0, op1) =>
    let observer = op0(op1(observer));
    source |> subscribeSafe(observer);
  | Lift3(source, op0, op1, op2) =>
    let observer = op0(op1(op2(observer)));
    source |> subscribeSafe(observer);
  | Lift4(source, op0, op1, op2, op3) =>
    let observer = op0(op1(op2(op3(observer))));
    source |> subscribeSafe(observer);
  };

let subscribeWith = (~onNext, ~onComplete, observable) => {
  let observer = Observer.createAutoDisposing(~onNext, ~onComplete);

  subscribeObserver(observer, observable);
  observer |> Observer.asCompositeDisposable;
};

let subscribe =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      observable,
    ) =>
  observable |> subscribeWith(~onNext, ~onComplete);

let publishTo = (~onNext, ~onComplete, observable) => {
  let connection = ref(Disposable.disposed);
  let active = ref(false);

  () => {
    if (! Interlocked.exchange(true, active)) {
      let subscription = observable |> subscribeWith(~onNext, ~onComplete);
      let newConnection =
        Disposable.create(() => {
          subscription |> CompositeDisposable.dispose;
          Volatile.write(false, active);
        });

      Volatile.write(newConnection, connection);
    };
    Volatile.read(connection);
  };
};

let publish =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      observable,
    ) =>
  publishTo(~onNext, ~onComplete, observable);

let raise = (~scheduler=Scheduler.immediate, exn: exn) => {
  let exn = Some(exn);

  scheduler === Scheduler.immediate ?
    create(observer => observer |> Observer.complete(~exn?)) :
    create(observer => {
      let schedulerDisposable =
        scheduler(() => {
          observer |> Observer.complete(~exn?);
          Disposable.disposed;
        });
      observer |> Observer.addDisposable(schedulerDisposable) |> ignore;
    });
};