let operator = {
  let onNext = (self, delegate, notif) =>
    switch (notif) {
    | RxNotification.Next(next) => delegate |> RxSubscriber.next(next)
    | RxNotification.Complete(None) => self^ |> RxSubscriber.complete
    | RxNotification.Complete(exn) => self^ |> RxSubscriber.complete(~exn?)
    };

  subscriber => {
    let self = ref(RxSubscriber.disposed);
    self :=
      subscriber
      |> RxSubscriber.decorate1(
           ~onNext,
           ~onComplete=RxSubscriber.forwardOnComplete1,
           self,
         );
    self^;
  };
};