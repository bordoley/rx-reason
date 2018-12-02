let onNext = (maxBufferCount, buffer, next) => {
  let currentBuffer = buffer^;

  buffer :=
    currentBuffer
    |> RxCopyOnWriteArray.addLastWithMaxCount(
         maxBufferCount,
         RxNotification.next(next),
       );
};

let onComplete = (maxBufferCount, buffer, exn) => {
  let currentBuffer = buffer^;

  buffer :=
    currentBuffer
    |> RxCopyOnWriteArray.addLastWithMaxCount(
         maxBufferCount,
         RxNotification.complete(exn),
       );
};

let onSubscribe = (_, buffer, subscriber) => {
  let currentBuffer = buffer^;
  RxCopyOnWriteArray.forEach(
    next =>
      switch (next) {
      | RxNotification.Next(v) => subscriber |> RxSubscriber.next(v)
      | RxNotification.Complete(exn) =>
        subscriber |> RxSubscriber.complete(~exn?)
      },
    currentBuffer,
  );
};

let onDispose = (_, buffer) => buffer := RxCopyOnWriteArray.empty();

let create = maxBufferCount => {
  let buffer = ref(RxCopyOnWriteArray.empty());
  RxSubject.create2(
    ~onNext,
    ~onComplete,
    ~onSubscribe,
    ~onDispose,
    maxBufferCount,
    buffer,
  );
};