type t('a, 'request) =
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

let create = (~request, ~disposeResource) => {
  let mapper = (resource, next) => resource |> request(next);

  resource => {
    let event = RxEvent.create();
    let requestStream = event |> RxEvent.asObservable;

    let subject = RxSubject.createMulticast();
    let observable = subject |> RxSubject.asObservable;

    let requestSubscription =
      requestStream
      |> RxObservables.switchMap1(mapper, resource)
      |> RxObservables.publishToSubject(subject)
      |> RxObservable.connect;

    let disposable =
      RxDisposable.compose([
        requestSubscription,
        RxSubject.asDisposable(subject),
        RxEvent.asDisposable(event),
        RxDisposable.create1(disposeResource, resource),
      ]);

    Instance(event, observable, disposable);
  };
};

let disposed = Disposed;