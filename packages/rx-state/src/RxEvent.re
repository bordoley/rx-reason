type t('a) = RxSubject.t('a);

let asDisposable = RxSubject.asDisposable;
let asObservable = RxSubject.asObservable
let create = RxSubjects.createMulticast;
let dispose = RxSubject.dispose;
let isDisposed = RxSubject.isDisposed;
let raiseIfDisposed = RxSubject.raiseIfDisposed;
let trigger = (value, event) =>
  event |> RxSubject.next(value);