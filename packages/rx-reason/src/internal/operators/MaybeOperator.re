let operator = subscriber => {
  let onComplete = exn => {
    let exn =
      switch (exn) {
      | Some(EmptyException.Exn) => None
      | _ => exn
      };
    subscriber |> Subscriber.complete(~exn?);
  };
  subscriber
  |> Subscriber.delegate(~onNext=Subscriber.forwardOnNext(subscriber), ~onComplete);
};

let lift = observable => observable |> ObservableSource.lift(operator);