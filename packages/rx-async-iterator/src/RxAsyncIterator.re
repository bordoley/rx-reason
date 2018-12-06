type t('request, 'a) =
  | Disposed
  | Instance(RxEvent.t('request), RxObservable.t('a), RxDisposable.t);

let asDisposable =
  fun
  | Disposed => RxDisposable.disposed
  | Instance(_, _, disposable) => disposable;

let asObservable =
  fun
  | Disposed => RxObservable.never
  | Instance(_, observable, _) => observable;

let dispose = self => self |> asDisposable |> RxDisposable.dispose;

let isDisposed = self => self |> asDisposable |> RxDisposable.isDisposed;

let request = action =>
  fun
  | Disposed => ()
  | Instance(event, _, _) => event |> RxEvent.dispatch(action);

let createInternal = {
  let onNext = (subject, next) => subject |> RxSubject.next(next);
  let onComplete = (subject, exn) => subject |> RxSubject.complete(~exn?);

  flatMap => {
    let event = RxEvent.create();
    let requestStream = event |> RxEvent.asObservable;

    let subject = RxSubject.createMulticast();
    let observable = subject |> RxSubject.asObservable;

    let requestSubscription =
      requestStream
      |> flatMap
      |> RxObservables.observe1(~onNext, ~onComplete, subject)
      |> RxObservable.connect;

    let disposable =
      RxDisposable.compose([
        requestSubscription,
        RxSubject.asDisposable(subject),
        RxEvent.asDisposable(event),
      ]);

    Instance(event, observable, disposable);
  };
};

let create = f => {
  let switchMap = RxObservables.switchMap(f);
  createInternal(switchMap);
};

let create1 = (f, ctx0) => {
  let switchMap = RxObservables.switchMap1(f, ctx0);
  createInternal(switchMap);
};

let disposed = Disposed;