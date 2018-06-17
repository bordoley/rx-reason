type source('a) = Observer.t('a) => Disposable.t;

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

let createWithObserver = onSubscribe => Source(onSubscribe);

let create = onSubscribe =>
  Source(
    observer =>
      onSubscribe(
        ~onNext=Observer.forwardOnNext(observer),
        ~onComplete=Observer.forwardOnComplete(observer),
      ),
  );

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

let never = Source(_ => Disposable.empty());

let pipe2 = (opA, opB, observable) =>
  switch (observable) {
  | Source(source) => Lift2(source, opA, opB)
  | Lift1(source, op0) => Lift3(source, op0, opA, opB)
  | Lift2(source, op0, op1) => Lift4(source, op0, op1, opA, opB)
  | Lift3(source, op0, op1, op2) =>
    let op0 = observer => op0 @@ op1 @@ op2 @@ opA @@ opB @@ observer;
    Lift1(source, op0);
  | Lift4(source, op0, op1, op2, op3) =>
    let op0 = observer => op0 @@ op1 @@ op2 @@ op3 @@ opA @@ opB @@ observer;
    Lift1(source, op0);
  };

let pipe3 = (opA, opB, opC, observable) =>
  switch (observable) {
  | Source(source) => Lift3(source, opA, opB, opC)
  | Lift1(source, op0) => Lift4(source, op0, opA, opB, opC)
  | Lift2(source, op0, op1) =>
    let op0 = observer => op0 @@ op1 @@ opA @@ opB @@ opC @@ observer;
    Lift1(source, op0);
  | Lift3(source, op0, op1, op2) =>
    let op0 = observer => op0 @@ op1 @@ op2 @@ opA @@ opB @@ opC @@ observer;
    Lift1(source, op0);
  | Lift4(source, op0, op1, op2, op3) =>
    let op0 = observer =>
      op0 @@ op1 @@ op2 @@ op3 @@ opA @@ opB @@ opC @@ observer;
    Lift1(source, op0);
  };

let pipe4 = (opA, opB, opC, opD, observable) =>
  switch (observable) {
  | Source(source) => Lift4(source, opA, opB, opC, opD)
  | Lift1(source, op0) =>
    let op0 = observer => op0 @@ opA @@ opB @@ opC @@ opD @@ observer;
    Lift1(source, op0);
  | Lift2(source, op0, op1) =>
    let op0 = observer => op0 @@ op1 @@ opA @@ opB @@ opC @@ opD @@ observer;
    Lift1(source, op0);
  | Lift3(source, op0, op1, op2) =>
    let op0 = observer =>
      op0 @@ op1 @@ op2 @@ opA @@ opB @@ opC @@ opD @@ observer;
    Lift1(source, op0);
  | Lift4(source, op0, op1, op2, op3) =>
    let op0 = observer =>
      op0 @@ op1 @@ op2 @@ op3 @@ opA @@ opB @@ opC @@ opD @@ observer;
    Lift1(source, op0);
  };

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
    Disposable.disposed;
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
  let subscription = SerialDisposable.create();

  let observer =
    Observer.createAutoDisposing(~onNext, ~onComplete, ~onDispose=() =>
      subscription |> SerialDisposable.dispose
    );

  subscription
  |> SerialDisposable.set(subscribeObserver(observer, observable));
  observer |> Observer.asDisposable;
};

let publishTo = (~onNext, ~onComplete, observable) => {
  let connection = ref(Disposable.disposed);
  let active = ref(false);

  () => {
    if (! Interlocked.exchange(true, active)) {
      let subscription = observable |> subscribeWith(~onNext, ~onComplete);
      let newConnection =
        Disposable.create(() => {
          subscription |> Disposable.dispose;
          Volatile.write(false, active);
        });

      Volatile.write(newConnection, connection);
    };
    Volatile.read(connection);
  };
};

let raise = (~scheduler=Scheduler.immediate, exn: exn) => {
  let exn = Some(exn);

  scheduler === Scheduler.immediate ?
    create((~onNext as _, ~onComplete) => {
      onComplete(exn);
      Disposable.disposed;
    }) :
    create((~onNext as _, ~onComplete) =>
      scheduler(() => {
        onComplete(exn);
        Disposable.disposed;
      })
    );
};