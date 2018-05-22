type t('a) = Observer.t('a) => Disposable.t;

let subscribeObserver =
    (observer: Observer.t('a), subscribe: t('a))
    : Disposable.t =>
  subscribe(observer);

let subscribe =
    (
      ~onComplete=Functions.alwaysUnit,
      ~onNext=Functions.alwaysUnit,
      observable: t('a),
    )
    : Disposable.t => {
  let observer = Observer.create(~onComplete, ~onNext, ());
  observable |> subscribeObserver(observer);
};

let create = (subscribe: Observer.t('a) => Disposable.t) : t('a) =>
  (observer: Observer.t('a)) => {
    let subscription = AssignableDisposable.create();
    let observer =
      Observer.create(
        ~onNext=
          next =>
            try (Observer.next(next, observer)) {
            | exn =>
              subscription
              |> AssignableDisposable.toDisposable
              |> Disposable.dispose;
              raise(exn);
            },
        ~onComplete=
          exn => {
            try (Observer.complete(~exn, observer)) {
            | exn =>
              subscription
              |> AssignableDisposable.toDisposable
              |> Disposable.dispose;
              raise(exn);
            };
            subscription
            |> AssignableDisposable.toDisposable
            |> Disposable.dispose;
          },
        (),
      );
    subscription
    |> AssignableDisposable.assign(
         try (subscribe(observer)) {
         | exn =>
           let shouldRaise =
             observer |> Observer.completeWithResult(~exn=Some(exn)) |> (!);
           if (shouldRaise) {
             /* This could happen when the onComplete is called synchronously in the
              * subscribe function which also throws.
              */
             raise(
               exn,
             );
           };
           Disposable.disposed;
         },
       )
    |> AssignableDisposable.toDisposable;
  };

let concat =
    (~scheduler=Scheduler.immediate, observables: list(t('a)))
    : t('a) =>
  create(observer => {
    let remaining = ref(observables);
    let rec scheduleSubscription = () =>
      scheduler
      |> Scheduler.schedule(() =>
           switch (remaining^) {
           | [hd, ...tail] =>
             remaining := tail;
             doSubscribe(hd);
           | [] => Disposable.disposed
           }
         )
    and doSubscribe = observable => {
      let subscription =
        observable
        |> subscribe(
             ~onNext=next => observer |> Observer.next(next),
             ~onComplete=
               exn =>
                 switch (exn) {
                 | Some(_) => observer |> Observer.complete(~exn)
                 | None =>
                   /*subscription |> Disposable.dispose;*/
                   scheduleSubscription() |> ignore
                 },
           );
      subscription;
    };
    scheduleSubscription();
  });

let defer = (f: unit => t('a)) : t('a) =>
  create(observer => f((), observer));

let empty = (~scheduler=Scheduler.immediate, ()) =>
  scheduler === Scheduler.immediate ?
    create(observer => {
      Observer.complete(observer);
      Disposable.disposed;
    }) :
    create(observer =>
      scheduler
      |> Scheduler.schedule(() => {
           Observer.complete(observer);
           Disposable.disposed;
         })
    );

let merge =
    /* FIXME: Should use a trampoline scheduler as the default for this function */
    (~scheduler=Scheduler.immediate, observables: list(t('a)))
    : t('a) => {
  let count = List.length(observables);
  count === 0 ?
    empty(~scheduler, ()) :
    create(observer => {
      let active = ref(count);
      let subscriptions = Array.make(count, Disposable.disposed);
      let subscription =
        Disposable.create(() => {
          Array.iter(Disposable.dispose, subscriptions);
          active := 0;
        });
      let childObserver = index =>
        Observer.create(
          ~onNext=next => Observer.next(next, observer),
          ~onComplete=
            exn =>
              switch (exn) {
              | Some(_) =>
                observer |> Observer.complete(~exn);
                subscription |> Disposable.dispose;
              | None =>
                active := active^ - 1;
                if (active^ === 0) {
                  subscription |> Disposable.dispose;
                };
              },
          ~onDispose=() => subscriptions[index] |> Disposable.dispose,
          (),
        );
      observables
      |> List.iteri((index, observable) =>
           subscriptions[index] =
             scheduler
             |> Scheduler.schedule(() =>
                  observable |> subscribeObserver(childObserver(index))
                )
         );
      subscription;
    });
};

let never = () : t('a) => create((_) => Disposable.empty());

let ofList = (~scheduler=Scheduler.immediate, list: list('a)) : t('a) =>
  scheduler === Scheduler.immediate ?
    create(observer => {
      let rec loop = list =>
        switch (list) {
        | [next, ...tail] =>
          Observer.next(next, observer);
          loop(tail);
        | [] => Observer.complete(observer)
        };
      loop(list);
      Disposable.disposed;
    }) :
    create(observer => {
      let rec loop = (list, ()) =>
        switch (list) {
        | [next, ...tail] =>
          Observer.next(next, observer);
          scheduler |> Scheduler.schedule(loop(tail));
        | [] =>
          Observer.complete(observer);
          Disposable.disposed;
        };
      scheduler |> Scheduler.schedule(loop(list));
    });

let ofValue = (~scheduler=Scheduler.immediate, value: 'a) : t('a) =>
  scheduler === Scheduler.immediate ?
    create(observer => {
      Observer.next(value, observer);
      Observer.complete(observer);
      Disposable.disposed;
    }) :
    create(observer =>
      scheduler
      |> Scheduler.schedule(() => {
           Observer.next(value, observer);
           scheduler
           |> Scheduler.schedule(() => {
                Observer.complete(observer);
                Disposable.disposed;
              });
         })
    );

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  create(observer => {
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
    let sourceSubscription = subscribeObserver(observer, observable);
    subscription
    |> AssignableDisposable.assign(
         Disposable.compose([
           sourceSubscription,
           Observer.toDisposable(observer),
         ]),
       )
    |> AssignableDisposable.toDisposable;
  });

let startWithValue =
    (~scheduler=Scheduler.immediate, value: 'a, observable: t('a)) =>
  concat([ofValue(~scheduler, value), observable]);

let startWithList =
    (~scheduler=Scheduler.immediate, values: list('a), observable: t('a)) =>
  concat([ofList(~scheduler, values), observable]);

let combineLatest2 =
    (
      ~scheduler=Scheduler.immediate,
      selector: ('a, 'b) => 'c,
      observable0: t('a),
      observable1: t('b),
    )
    : t('c) =>
  create(observer =>
    scheduler
    |> Scheduler.schedule(() => {
         let lock = Concurrency.Lock.create();
         let value0 = MutableOption.empty();
         let value1 = MutableOption.empty();
         let parentSubscription = AssignableDisposable.create();
         let onComplete = exn =>
           Concurrency.Lock.execute(
             () =>
               switch (exn) {
               | Some(_) =>
                 observer |> Observer.complete(~exn);
                 parentSubscription
                 |> AssignableDisposable.toDisposable
                 |> Disposable.dispose;
               | _ => ()
               },
             lock,
           );
         let onNext = (value, next) =>
           Concurrency.Lock.execute(
             Functions.earlyReturnsUnit(() => {
               MutableOption.set(next, value);
               if (value0
                   |> MutableOption.isNotEmpty
                   && value1
                   |> MutableOption.isNotEmpty) {
                 let mapped =
                   try (
                     selector(
                       MutableOption.firstOrRaise(value0),
                       MutableOption.firstOrRaise(value1),
                     )
                   ) {
                   | exn =>
                     onComplete(Some(exn));
                     Functions.returnUnit();
                   };
                 observer |> Observer.next(mapped);
               };
             }),
             lock,
           );
         let subscription0 =
           observable0 |> subscribe(~onNext=onNext(value0));
         let subscription1 =
           observable1 |> subscribe(~onNext=onNext(value1));
         parentSubscription
         |> AssignableDisposable.assign(
              Disposable.compose([subscription0, subscription1]),
            )
         |> AssignableDisposable.toDisposable;
       })
  );

let combineLatest3 =
    (
      ~scheduler=Scheduler.immediate,
      ~selector: ('a, 'b, 'c) => 'd,
      observable0: t('a),
      observable1: t('b),
      observable2: t('c),
    )
    : t('d) =>
  create(observer => Disposable.disposed);

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
  create(observer => Disposable.disposed);

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
  create(observer => Disposable.disposed);

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
  create(observer => Disposable.disposed);

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
  create(observer => Disposable.disposed);