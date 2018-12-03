let onNext = (self, delegate, notif) =>
  switch (notif) {
  | RxNotification.Next(next) => delegate |> RxSubscriber.next(next)
  | RxNotification.Complete(None) => self^ |> RxSubscriber.complete
  | RxNotification.Complete(exn) => self^ |> RxSubscriber.complete(~exn?)
  };

let create = subscriber => {
  let self = ref(RxSubscriber.disposed);
  self :=
    subscriber
    |> RxSubscriber.decorate1(
         ~onNext,
         ~onComplete=SubscriberForward.onComplete1,
         self,
       );
  self^;
};