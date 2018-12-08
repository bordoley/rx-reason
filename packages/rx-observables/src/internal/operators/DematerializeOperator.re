let onNext = {
  let onNext = (_, subscriber, v) => subscriber |> RxSubscriber.next(v);
  let onComplete = (self, _, exn) => self^ |> RxSubscriber.complete(~exn?);

  (self, delegate, notif) =>
    notif |> RxNotification.map2(~onNext, ~onComplete, self, delegate);
};

let create = subscriber => {
  let self = ref(RxSubscriber.disposed);
  self := subscriber |> RxSubscriber.decorateOnNext1(onNext, self);
  self^;
};