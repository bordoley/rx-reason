let pipe =
    (operator: Operator.t('a, 'b), observable: Observable.t('a))
    : Observable.t('b) =>
  Observable.create(observer => {
    let subscription = AssignableDisposable.create();
    let dispose = () =>
      subscription |> AssignableDisposable.toDisposable |> Disposable.dispose;
    let observer = {
      let pipedObserver = operator(observer);
      Observer.create(
        ~onComplete=
          exn => {
            pipedObserver |> Observer.complete(~exn);
            dispose();
          },
        ~onNext=
          next => {
            pipedObserver |> Observer.next(next);
            if (pipedObserver |> Observer.toDisposable |> Disposable.isDisposed) {
              dispose();
            };
          },
        ~onDispose=
          () => pipedObserver |> Observer.toDisposable |> Disposable.dispose,
        (),
      );
    };
    let sourceSubscription =
      Observable.subscribeObserver(observer, observable);
    subscription
    |> AssignableDisposable.assign(
         Disposable.compose([
           sourceSubscription,
           Observer.toDisposable(observer),
         ]),
       )
    |> AssignableDisposable.toDisposable;
  });

let pipe2 =
    (op0: Operator.t('a, 'b), op1: Operator.t('b, 'c))
    : (Observable.t('a) => Observable.t('c)) =>
  pipe(Operator.pipe2(op0, op1));

let pipe3 =
    (
      op0: Operator.t('a, 'b),
      op1: Operator.t('b, 'c),
      op2: Operator.t('c, 'd),
    )
    : (Observable.t('a) => Observable.t('d)) =>
  pipe(Operator.pipe3(op0, op1, op2));

let pipe4 =
    (
      op0: Operator.t('a, 'b),
      op1: Operator.t('b, 'c),
      op2: Operator.t('c, 'd),
      op3: Operator.t('d, 'e),
    )
    : (Observable.t('a) => Observable.t('e)) =>
  pipe(Operator.pipe4(op0, op1, op2, op3));

let pipe5 =
    (
      op0: Operator.t('a, 'b),
      op1: Operator.t('b, 'c),
      op2: Operator.t('c, 'd),
      op3: Operator.t('d, 'e),
      op4: Operator.t('e, 'f),
    )
    : (Observable.t('a) => Observable.t('f)) =>
  pipe(Operator.pipe5(op0, op1, op2, op3, op4));

let pipe6 =
    (
      op0: Operator.t('a, 'b),
      op1: Operator.t('b, 'c),
      op2: Operator.t('c, 'd),
      op3: Operator.t('d, 'e),
      op4: Operator.t('e, 'f),
      op5: Operator.t('f, 'g),
    )
    : (Observable.t('a) => Observable.t('g)) =>
  pipe(Operator.pipe6(op0, op1, op2, op3, op4, op5));

let pipe7 =
    (
      op0: Operator.t('a, 'b),
      op1: Operator.t('b, 'c),
      op2: Operator.t('c, 'd),
      op3: Operator.t('d, 'e),
      op4: Operator.t('e, 'f),
      op5: Operator.t('f, 'g),
      op6: Operator.t('g, 'h),
    )
    : (Observable.t('a) => Observable.t('h)) =>
  pipe(Operator.pipe7(op0, op1, op2, op3, op4, op5, op6));

let share =
    (~createSubject=Subject.create, source: Observable.t('a))
    : Observable.t('a) => {
  let subject = MutableOption.empty();
  let sourceSubscription = ref(Disposable.disposed);
  let refCount = ref(0);
  let reset = () => {
    sourceSubscription^ |> Disposable.dispose;
    subject |> MutableOption.unset;
    refCount := 0;
  };
  Observable.create((observer: Observer.t('a)) => {
    let currentSubject = {
      if (refCount^ === 0) {
        MutableOption.set(createSubject(), subject);
      };
      subject |> MutableOption.firstOrRaise;
    };
    let subjectObservable = Subject.toObservable(currentSubject);
    let subjectObserver = Subject.toObserver(currentSubject);
    let observerSubscription =
      subjectObservable |> Observable.subscribeObserver(observer);
    if (refCount^ === 0) {
      sourceSubscription :=
        source
        |> Observable.subscribe(
             ~onNext=next => subjectObserver |> Observer.next(next),
             ~onComplete=
               exn => {
                 subjectObserver |> Observer.complete(~exn);
                 reset();
               },
           );
    };
    if (subjectObserver |> Observer.toDisposable |> Disposable.isDisposed) {
      /* The source completed synchronously and reset */
      observerSubscription |> Disposable.dispose;
      Disposable.disposed;
    } else {
      refCount := refCount^ + 1;
      Disposable.create(() => {
        refCount := refCount^ > 0 ? refCount^ - 1 : 0;
        if (refCount^ === 0) {
          reset();
        };
        observerSubscription |> Disposable.dispose;
      });
    };
  });
};

let startWithValue =
    (~scheduler=Scheduler.immediate, value: 'a, observable: Observable.t('a)) =>
  Observable.concat([Observable.ofValue(~scheduler, value), observable]);

let startWithList =
    (
      ~scheduler=Scheduler.immediate,
      values: list('a),
      observable: Observable.t('a),
    ) =>
  Observable.concat([Observable.ofList(~scheduler, values), observable]);
/* shareReplay */
/* bufferCount */
/* debounce */