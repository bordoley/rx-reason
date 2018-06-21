let operator = {
  let onComplete = (delegate, exn) => {
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
         ~onNext=Subscriber.delegateOnNext,
         ~onComplete,
       );
};

let lift = observable => observable |> ObservableSource.lift(operator);