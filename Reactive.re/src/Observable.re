type t('a) = Observer.t('a) => Disposable.t;

let subscribeObserver =
    (observer: Observer.t('a), subscribe: t('a))
    : Disposable.t =>
  subscribe(observer);

let subscribe =
    (
      ~onNext=Functions.alwaysUnit,
      ~onComplete=Functions.alwaysUnit,
      observable: t('a),
    )
    : Disposable.t => {
  let observer = Observer.create(~onComplete, ~onNext, ());
  observable |> subscribeObserver(observer);
};

let create = subscribe : t('a) =>
  observer => {
    let subscription = ref(Disposable.disposed);
    let delegateObserver =
      Observer.createWithCallbacks(
        ~onComplete=exn => observer |> Observer.complete(~exn),
        ~onNext=next => observer |> Observer.next(next),
        ~onDispose=
          () =>
            Interlocked.exchange(Disposable.disposed, subscription)
            |> Disposable.dispose,
      );
    subscription :=
      (
        try (
          subscribe(
            ~next=next => delegateObserver |> Observer.next(next),
            ~complete=exn => delegateObserver |> Observer.complete(~exn),
          )
        ) {
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
        }
      );
    delegateObserver |> Observer.toDisposable;
  };

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  create((~next, ~complete) => {
    let observer = Observer.create(~onNext=next, ~onComplete=complete, ());
    let lifted = operator(observer);
    observable |> subscribeObserver(lifted);
  });

/*
 let concat =
     (~scheduler=Scheduler.immediate, observables: list(t('a)))
     : t('a) =>
   create(observer => {
     let remaining = ref(observables);
     let rec scheduleSubscription = () =>
       scheduler(() =>
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
   });*/
let defer = (f: unit => t('a)) : t('a) =>
  create((~next, ~complete) => {
    let observer = Observer.create(~onNext=next, ~onComplete=complete, ());
    f() |> subscribeObserver(observer);
  });

let empty = (~scheduler=Scheduler.immediate, ()) =>
  scheduler === Scheduler.immediate ?
    create((~next as _, ~complete) => {
      complete(None);
      Disposable.disposed;
    }) :
    create((~next as _, ~complete) =>
      scheduler(() => {
        complete(None);
        Disposable.disposed;
      })
    );

/*
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
         Observer.createWithCallbacks(
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
         );
       observables
       |> List.iteri((index, observable) =>
            subscriptions[index] =
              scheduler(() =>
                observable |> subscribeObserver(childObserver(index))
              )
          );
       subscription;
     });
 };*/
let never = () : t('a) =>
  create((~next as _, ~complete as _) => Disposable.empty());

let ofList = (~scheduler=Scheduler.immediate, list: list('a)) : t('a) =>
  scheduler === Scheduler.immediate ?
    create((~next, ~complete) => {
      let rec loop = list =>
        switch (list) {
        | [hd, ...tail] =>
          next(hd);
          loop(tail);
        | [] => complete(None)
        };
      loop(list);
      Disposable.disposed;
    }) :
    create((~next, ~complete) => {
      let rec loop = (list, ()) =>
        switch (list) {
        | [hd, ...tail] =>
          next(hd);
          scheduler(loop(tail));
        | [] =>
          complete(None);
          Disposable.disposed;
        };
      scheduler(loop(list));
    });

let ofValue = (~scheduler=Scheduler.immediate, value: 'a) : t('a) =>
  scheduler === Scheduler.immediate ?
    create((~next, ~complete) => {
      next(value);
      complete(None);
      Disposable.disposed;
    }) :
    create((~next, ~complete) =>
      scheduler(() => {
        next(value);
        scheduler(() => {
          complete(None);
          Disposable.disposed;
        });
      })
    );
/*
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
   create(observer =>
     scheduler(() => {
       let lock = Lock.create();
       let value0 = MutableOption.empty();
       let value1 = MutableOption.empty();
       let parentSubscription = AssignableDisposable.create();
       let onComplete = exn => {
         Lock.aquire(lock);
         switch (exn) {
         | Some(_) =>
           observer |> Observer.complete(~exn);
           parentSubscription
           |> AssignableDisposable.toDisposable
           |> Disposable.dispose;
         | _ => ()
         };
         Lock.release(lock);
       };
       let onNext = (value, next) => {
         Lock.aquire(lock);
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
         });
         Lock.release(lock);
       };
       let subscription0 = observable0 |> subscribe(~onNext=onNext(value0));
       let subscription1 = observable1 |> subscribe(~onNext=onNext(value1));
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
   */