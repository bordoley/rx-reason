type t('a) = RxSubject.t('a);

let asDisposable = RxSubject.asDisposable;
let asObservable = RxSubject.asObservable;
let create = RxSubject.createMulticast;
let dispose = RxSubject.dispose;
let isDisposed = RxSubject.isDisposed;
let trigger = RxSubject.next;