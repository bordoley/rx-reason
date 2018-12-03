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
    RxNotification.map1(
      ~onNext=SubscriberForward.onNext,
      ~onComplete=SubscriberForward.onComplete,
      subscriber,
    ),
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