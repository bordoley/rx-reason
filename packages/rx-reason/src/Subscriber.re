type t('a) = {
  isAutoDispose: bool,
  isStopped: ref(bool),
  onComplete: option(exn) => unit,
  onNext: 'a => unit,
  disposable: CompositeDisposable.t,
};

type subscriber('a) = t('a);

module type S1 = {
  type t('a);

  include CompositeDisposable.S1 with type t('a) := t('a);
  include Observer.S1 with type t('a) := t('a);

  let asSubscriber: t('a) => subscriber('a);
};

let asCompositeDisposable = ({disposable}) => disposable;

let addDisposable = (disposable, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addDisposable(disposable)
  |> ignore;
  subscriber;
};

let addTeardown = (teardown, subscriber) => {
  subscriber
  |> asCompositeDisposable
  |> CompositeDisposable.addTeardown(teardown)
  |> ignore;
  subscriber;
};

let asDisposable = subscriber =>
  subscriber |> asCompositeDisposable |> CompositeDisposable.asDisposable;

let asSubscriber = Functions.identity;

let createInternal = (~isAutoDispose, ~onNext, ~onComplete, ~disposable) => {
  let isStopped = ref(false);
  disposable
  |> CompositeDisposable.addTeardown(() => Volatile.write(true, isStopped))
  |> ignore;
  {isAutoDispose, isStopped, onComplete, onNext, disposable};
};

let delegate = (~onNext, ~onComplete, subscriber) => {
  let disposable = subscriber |> asCompositeDisposable;
  createInternal(~isAutoDispose=false, ~onNext, ~onComplete, ~disposable);
};

let create = (~onNext, ~onComplete) =>
  createInternal(
    ~isAutoDispose=false,
    ~onNext,
    ~onComplete,
    ~disposable=CompositeDisposable.create(),
  );

let createAutoDisposing = (~onNext, ~onComplete) =>
  createInternal(
    ~isAutoDispose=true,
    ~onNext,
    ~onComplete,
    ~disposable=CompositeDisposable.create(),
  );

let autoDispose = ({isAutoDispose, disposable}) =>
  if (isAutoDispose) {
    disposable |> CompositeDisposable.dispose;
  };

let completeWithResult = (~exn=?, {isStopped, onComplete} as subscriber) : bool => {
  let shouldComplete = ! Interlocked.exchange(true, isStopped);
  if (shouldComplete) {
    try (onComplete(exn)) {
    | exn =>
      autoDispose(subscriber);
      raise(exn);
    };
  };
  autoDispose(subscriber);
  shouldComplete;
};

let complete = (~exn=?, subscriber: t('a)) : unit =>
  subscriber |> completeWithResult(~exn?) |> ignore;

let dispose = ({disposable}) => disposable |> CompositeDisposable.dispose;

let disposedIsStopped = ref(true);
let disposed = {
  isAutoDispose: false,
  isStopped: disposedIsStopped,
  onComplete: Functions.alwaysUnit,
  onNext: Functions.alwaysUnit,
  disposable: CompositeDisposable.disposed,
};

let isDisposed = ({disposable}) =>
  disposable |> CompositeDisposable.isDisposed;

let isStopped = ({isStopped}) => Volatile.read(isStopped);

let next = (next: 'a, {onNext, isStopped} as subscriber) : unit => {
  let isStopped = Volatile.read(isStopped);
  if (! isStopped) {
    try (onNext(next)) {
    | exn =>
      autoDispose(subscriber);
      raise(exn);
    };
  };
};

let notify = (notif, subscriber) =>
  switch (notif) {
  | Notification.Next(v) => subscriber |> next(v)
  | Notification.Complete => subscriber |> complete
  | Notification.CompleteWithException(exn) => subscriber |> complete(~exn)
  };

let raiseIfDisposed = ({disposable}) =>
  disposable |> CompositeDisposable.raiseIfDisposed;

let forwardOnComplete = (subscriber, exn) => subscriber |> complete(~exn?);

let forwardOnNext = (subscriber, v) => subscriber |> next(v);