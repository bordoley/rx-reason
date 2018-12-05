type t('a) = RxSubject.t('a);

let asDisposable = RxSubject.asDisposable;
let asObservable = RxSubject.asObservable;
let create = RxSubject.createMulticast;
let dispose = RxSubject.dispose;
let disposed = RxSubject.disposed;
let isDisposed = RxSubject.isDisposed;
let dispatch = RxSubject.next;