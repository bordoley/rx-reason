let operator = {
  let onNext = (self, delegate, notif) =>
    switch (notif) {
    | Notification.Next(next) => delegate |> Subscriber.next(next)
    | Notification.Complete => self^ |> Subscriber.complete
    | Notification.CompleteWithException(exn) =>
      self^ |> Subscriber.complete(~exn)
    };

  subscriber => {
    let self = ref(Subscriber.disposed);
    self :=
      subscriber
      |> Subscriber.decorate1(
           ~onNext,
           ~onComplete=Subscriber.forwardOnComplete1,
           self,
         );
    self^;
  };
};

let lift = observable => observable |> ObservableSource.lift(operator);