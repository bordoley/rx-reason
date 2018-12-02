type t('a) = RxSubject.t('a);

let asDisposable = RxSubject.asDisposable;
let asObservable = RxSubject.asObservable;
let create = () =>
  RxSubject.create(
    ~onNext=RxFunctions.alwaysUnit1,
    ~onComplete=RxFunctions.alwaysUnit1,
    ~onSubscribe=RxFunctions.alwaysUnit1,
    ~onDispose=RxFunctions.alwaysUnit1,
  );
let dispose = RxSubject.dispose;
let isDisposed = RxSubject.isDisposed;
let raiseIfDisposed = RxSubject.raiseIfDisposed;
let trigger = (value, event) => event |> RxSubject.next(value);