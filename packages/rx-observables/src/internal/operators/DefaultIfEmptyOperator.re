type context('a) = {
  default: 'a,
  mutable isEmpty: bool,
};

let create = default => {
  let onNext =
    (. ctx, delegate, next) => {
      delegate |> RxSubscriber.next(next);
      ctx.isEmpty = false;
    };

  let onComplete =
    (. {default, isEmpty}, delegate, exn) => {
      let exn =
        switch (exn) {
        | Some(RxEmptyException.Exn)
        | None =>
          if (isEmpty) {
            delegate |> RxSubscriber.next(default);
          };
          None;
        | Some(_) => exn
        };
      delegate |> RxSubscriber.complete(~exn?);
    };
  subscriber => {
    let ctx = {default, isEmpty: true};
    subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, ctx);
  };
};