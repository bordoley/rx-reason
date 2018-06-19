type t('a) = ObservableSource.t('a);

type observable('a) = t('a);

module type S1 = {
  type t('a);

  let asObservable: t('a) => observable('a);

  let publish:
    (
      ~onNext: 'a => unit=?,
      ~onComplete: option(exn) => unit=?,
      t('a),
      unit
    ) =>
    Disposable.t;

  let publishTo:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a), unit) =>
    Disposable.t;

  let subscribe:
    (~onNext: 'a => unit=?, ~onComplete: option(exn) => unit=?, t('a)) =>
    CompositeDisposable.t;

  let subscribeWith:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    CompositeDisposable.t;
};

let asObservable = Functions.identity;
let combineLatest2 = ObservableSourceCombineLatest.combineLatest2;
let combineLatest3 = ObservableSourceCombineLatest.combineLatest3;
let combineLatest4 = ObservableSourceCombineLatest.combineLatest4;
let combineLatest5 = ObservableSourceCombineLatest.combineLatest5;
let combineLatest6 = ObservableSourceCombineLatest.combineLatest6;
let combineLatest7 = ObservableSourceCombineLatest.combineLatest7;
let concat = ObservableSourceConcat.concat;
let createWithObserver = ObservableSource.createWithObserver;
let create = ObservableSource.create;
let debounce = DebounceOperator.lift;
let defaultIfEmpty = DefaultIfEmptyOperator.lift;
let distinctUntilChanged = DistinctUntilChangedOperator.lift;
let every = EveryOperator.lift;
let exhaust = ExhaustOperator.lift;
let find = FindOperator.lift;
let first = FirstOperator.lift;
let firstOrNone = FirstOrNoneOperator.lift;
let ignoreElements = IgnoreElementsOperator.lift;
let isEmpty = IsEmptyOperator.lift;
let keep = KeepOperator.lift;
let last = LastOperator.lift;
let lastOrNone = LastOrNoneOperator.lift;
let lift = ObservableSource.lift;
let map = MapOperator.lift;
let mapTo = MapToOperator.lift;
let materialize = MaterializeOperator.lift;
let maybeFirst = MaybeFirstOperator.lift;
let maybeLast = MaybeLastOperator.lift;
let merge = ObservableSourceMerge.merge;
let never = ObservableSource.never;
let none = NoneOperator.lift;
let observe = ObserveOperator.lift;
let observeOn = ObserveOnOperator.lift;
let onComplete = OnCompleteOperator.lift;
let onNext = OnNextOperator.lift;
let onSubscribe = OnSubscribeOperator.lift;
let publish = ObservableSource.publish;
let publishTo = ObservableSource.publishTo;
let raise = ObservableSource.raise;
let scan = ScanOperator.lift;
let some = SomeOperator.lift;
let switch_ = SwitchOperator.lift;
let subscribeObserver = ObservableSource.subscribeObserver;
let subscribeWith = ObservableSource.subscribeWith;
let subscribe = ObservableSource.subscribe;
let synchronize = SynchronizeOperator.lift;
let timeout = TimeoutOperator.lift;
let withLatestFrom = WithLatestFromOperator.lift;

let defer = (f: unit => t('a)) : t('a) =>
  createWithObserver(observer => f() |> subscribeObserver(observer));

let empty = (~scheduler=Scheduler.immediate, ()) =>
  scheduler === Scheduler.immediate ?
    create((~onNext as _, ~onComplete) => {
      onComplete(None);
      Functions.alwaysUnit;
    }) :
    create((~onNext as _, ~onComplete) => {
      let schedulerSubscription =
        scheduler(() => {
          onComplete(None);
          Disposable.disposed;
        });

      () => schedulerSubscription |> Disposable.dispose;
    });

let ofAbsoluteTimeNotifications =
    (
      ~scheduler as {now, scheduleWithDelay}: ClockScheduler.t,
      notifications: list((float, Notification.t('a))),
    )
    : t('a) =>
  createWithObserver(observer => {
    let rec loop = lst =>
      switch (lst) {
      | [(time, notif), ...tail] =>
        let delay = time -. now();
        if (delay > 0.0) {
          scheduleWithDelay(
            delay,
            () => {
              observer |> Observer.notify(notif);
              loop(tail);
            },
          );
        } else {
          loop(tail);
        };
      | [] => Disposable.disposed
      };

    let loopSubscription = loop(notifications);
    observer
    |> Observer.addTeardown(() => loopSubscription |> Disposable.dispose)
    |> ignore;
  });

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
      Functions.alwaysUnit;
    }) :
    create((~onNext, ~onComplete) => {
      let rec loop = (list, ()) =>
        switch (list) {
        | [hd] =>
          onNext(hd);
          onComplete(None);
          Disposable.disposed;
        | [hd, ...tail] =>
          onNext(hd);
          scheduler(loop(tail));
        | [] =>
          onComplete(None);
          Disposable.disposed;
        };

      let schedulerSubscription = scheduler(loop(list));
      () => schedulerSubscription |> Disposable.dispose;
    });

let ofNotifications =
    (
      ~scheduler as schedule=Scheduler.immediate,
      notifications: list(Notification.t('a)),
    )
    : t('a) =>
  createWithObserver(
    schedule === Scheduler.immediate ?
      observer => {
        let rec loop = list =>
          switch (list) {
          | [hd, ...tail] =>
            observer |> Observer.notify(hd);
            loop(tail);
          | [] => ()
          };
        loop(notifications);
      } :
      (
        observer => {
          let rec loop = (list, ()) =>
            switch (list) {
            | [hd] =>
              observer |> Observer.notify(hd);
              Disposable.disposed;
            | [hd, ...tail] =>
              observer |> Observer.notify(hd);
              schedule(loop(tail));
            | [] => Disposable.disposed
            };
          let schedulerSubscription = schedule(loop(notifications));
          observer
          |> Observer.addTeardown(() =>
               schedulerSubscription |> Disposable.dispose
             )
          |> ignore;
        }
      ),
  );

let ofRelativeTimeNotifications =
    (
      ~scheduler as schedule: DelayScheduler.t,
      notifications: list((float, Notification.t('a))),
    )
    : t('a) =>
  createWithObserver(observer => {
    let rec loop = (lst, previousDelay) =>
      switch (lst) {
      | [(delay, notif), ...tail] =>
        schedule(
          max(0.0, delay -. previousDelay),
          () => {
            observer |> Observer.notify(notif);
            loop(tail, delay);
          },
        )
      | [] => Disposable.disposed
      };

    let loopSubscription = loop(notifications, 0.0);
    observer
    |> Observer.addTeardown(() => loopSubscription |> Disposable.dispose)
    |> ignore;
  });

let ofValue = (~scheduler=Scheduler.immediate, value: 'a) : t('a) =>
  scheduler === Scheduler.immediate ?
    create((~onNext, ~onComplete) => {
      onNext(value);
      onComplete(None);
      Functions.alwaysUnit;
    }) :
    create((~onNext, ~onComplete) => {
      let schedulerSubscription =
        scheduler(() => {
          onNext(value);
          scheduler(() => {
            onComplete(None);
            Disposable.disposed;
          });
        });

      () => schedulerSubscription |> Disposable.dispose;
    });

let repeat = (~predicate=Functions.alwaysTrue, observable) =>
  RepeatOperator.lift(
    exn =>
      switch (exn) {
      | None => predicate()
      | Some(_) => false
      },
    observable,
  );

let retry = (~predicate=Functions.alwaysTrue, observable) =>
  RepeatOperator.lift(
    exn =>
      switch (exn) {
      | None => false
      | Some(exn) => predicate(exn)
      },
    observable,
  );

let startWithList =
    (~scheduler=Scheduler.immediate, values: list('a), observable: t('a)) =>
  concat([ofList(~scheduler, values), observable]);

let startWithValue =
    (~scheduler=Scheduler.immediate, value: 'a, observable: t('a)) =>
  concat([ofValue(~scheduler, value), observable]);

let subscribeOn = (scheduler: Scheduler.t, observable: t('a)) : t('a) =>
  createWithObserver(observer => {
    let schedulerSubscription =
      scheduler(() => {
        observable |> subscribeObserver(observer);
        Disposable.disposed;
      });
    observer
    |> Observer.addTeardown(() => schedulerSubscription |> Disposable.dispose)
    |> ignore;
  });

let toList = observable =>
  observable
  |> scan((acc, next) => [next, ...acc], [])
  |> last
  |> map(List.rev);