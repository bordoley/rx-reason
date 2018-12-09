let create = () => {
  let onComplete =
    (. delegate, exn) => {
      let exn =
        switch (exn) {
        | Some(RxEmptyException.Exn) => None
        | _ => exn
        };
      delegate |> RxSubscriber.complete(~exn?);
    };
    
  subscriber => subscriber |> RxSubscriber.decorateOnComplete(onComplete);
};