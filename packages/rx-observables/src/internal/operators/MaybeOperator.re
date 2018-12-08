let onComplete = (delegate, exn) => {
  let exn =
    switch (exn) {
    | Some(RxEmptyException.Exn) => None
    | _ => exn
    };
  delegate |> RxSubscriber.complete(~exn?);
};

let create = subscriber =>
  subscriber |> RxSubscriber.decorateOnComplete(~onComplete);