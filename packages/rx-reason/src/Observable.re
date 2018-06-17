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
    Disposable.t;

  let subscribeWith:
    (~onNext: 'a => unit, ~onComplete: option(exn) => unit, t('a)) =>
    Disposable.t;
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
let pipe2 = ObservableSource.pipe2;
let pipe3 = ObservableSource.pipe3;
let pipe4 = ObservableSource.pipe4;
let scan = ScanOperator.lift;
let some = SomeOperator.lift;
let switch_ = SwitchOperator.lift;
let subscribeObserver = ObservableSource.subscribeObserver;
let subscribeWith = ObservableSource.subscribeWith;
let synchronize = SynchronizeOperator.lift;
let withLatestFrom = WithLatestFromOperator.lift;

let defer = (f: unit => t('a)) : t('a) =>
  createWithObserver(observer => f() |> subscribeObserver(observer));

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

    loop(notifications);
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
      Disposable.disposed;
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
      scheduler(loop(list));
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
        Disposable.disposed;
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
          schedule(loop(notifications));
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

    loop(notifications, 0.0);
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

let raise = (~scheduler=Scheduler.immediate, exn: exn) : t('a) => {
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

let publish =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      observable,
    ) =>
  publishTo(~onNext, ~onComplete, observable);

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

let subscribe =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      observable,
    ) =>
  observable |> subscribeWith(~onNext, ~onComplete);

let subscribeOn = (scheduler: Scheduler.t, observable: t('a)) : t('a) =>
  createWithObserver(observer =>
    scheduler(() => observable |> subscribeObserver(observer))
  );

let toList = observable =>
  observable
  |> pipe3(
       ScanOperator.operator((acc, next) => [next, ...acc], []),
       LastOperator.operator,
       MapOperator.operator(List.rev),
     );