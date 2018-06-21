let operator = {
  let onComplete = (_, delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(EmptyException.Exn) => None
      | _ => exn
      };
    delegate |> Subscriber.complete(~exn?);
  };

  subscriber =>
    subscriber
    |> Subscriber.delegate(
         ~onNext=Subscriber.forwardOnNext,
         ~onComplete,
         None,
       );
};

let lift = observable => observable |> ObservableSource.lift(operator);