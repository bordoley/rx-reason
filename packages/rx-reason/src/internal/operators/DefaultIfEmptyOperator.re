type context('a) = {
  default: 'a,
  mutable isEmpty: bool,
};

let operator = {
  let onNext = (ctx, delegate, next) => {
    delegate |> Subscriber.next(next);
    ctx.isEmpty = false;
  };

  let onComplete = ({default, isEmpty}, delegate, exn) => {
    let exn =
      switch (exn) {
      | Some(EmptyException.Exn)
      | None =>
        if (isEmpty) {
          delegate |> Subscriber.next(default);
        };
        None;
      | Some(_) => exn
      };
    delegate |> Subscriber.complete(~exn?);
  };

  (default, subscriber) => {
    let ctx = {default, isEmpty: true};
    subscriber |> Subscriber.decorate1(~onNext, ~onComplete, ctx);
  };
};

let lift = (default, observable) =>
  observable |> ObservableSource.lift(operator(default));