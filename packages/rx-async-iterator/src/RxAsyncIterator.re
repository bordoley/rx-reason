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

let onRequestSubscriptionComplete = (dispose, disposable, _) =>
  dispose(disposable);

let create = (~request, ~disposeResource) => {
  let mapper = (resource, next) => resource |> request(next);

  resource => {
    let event = RxEvent.create();
    let requestStream = event |> RxEvent.asObservable;

    let subject = RxSubject.createMulticast();
    let observable = subject |> RxSubject.asObservable;

    let compositeDisposable =
      RxCompositeDisposable.create()
      |> RxCompositeDisposable.addDisposable(
           RxDisposable.create1(disposeResource, resource),
         )
      |> RxCompositeDisposable.addDisposable(RxEvent.asDisposable(event))
      |> RxCompositeDisposable.addDisposable(RxSubject.asDisposable(subject));

    let requestSubscription =
      requestStream
      |> RxObservables.switchMap1(mapper, resource)
      |> RxObservables.publishToSubject(subject)
      |> RxObservables.onComplete2(
           onRequestSubscriptionComplete,
           RxCompositeDisposable.dispose,
           compositeDisposable,
         )
      |> RxObservable.connect;

    let disposable =
      compositeDisposable
      |> RxCompositeDisposable.addDisposable(requestSubscription)
      |> RxCompositeDisposable.asDisposable;

    Instance(event, observable, disposable);
  };
};

let disposed = Disposed;