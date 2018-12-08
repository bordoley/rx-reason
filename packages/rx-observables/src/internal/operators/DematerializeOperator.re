let forwardOnCompleteToDelegate = (self, _, exn) =>
  self^ |> RxSubscriber.complete(~exn?);

let onNext = (self, delegate, notif) =>
  notif
  |> RxNotification.map2(
       ~onNext=SubscriberForward.onNext1,
       ~onComplete=forwardOnCompleteToDelegate,
       self,
       delegate,
     );

let create = subscriber => {
  let self = ref(RxSubscriber.disposed);
  self := subscriber |> RxSubscriber.decorateOnNext1(~onNext, self);
  self^;
};