let operator = {
  let onComplete = (delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(RxEmptyException.Exn) => None
      | _ => exn
      };
    delegate |> RxSubscriber.complete(~exn?);
  };

  subscriber =>
    subscriber
    |> RxSubscriber.decorate(~onNext=SubscriberForward.onNext, ~onComplete);
};