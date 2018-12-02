let createMulticast = () =>
  RxSubject.create(
    ~onNext=RxFunctions.alwaysUnit1,
    ~onComplete=RxFunctions.alwaysUnit1,
    ~onSubscribe=RxFunctions.alwaysUnit1,
    ~onDispose=RxFunctions.alwaysUnit1,
  );

let createWithReplayBuffer = {
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

  maxBufferCount => {
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
};