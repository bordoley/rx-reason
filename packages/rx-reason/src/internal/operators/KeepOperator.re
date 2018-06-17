let operator = predicate =>
  observer => {
    let keepObserver = ref(Observer.disposed);
    keepObserver :=
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let shouldKeep =
              try (predicate(next)) {
              | exn =>
                keepObserver^ |> Observer.complete(~exn);
                Functions.returnUnit();
              };
            if (shouldKeep) {
              observer |> Observer.next(next);
            };
          }),
        ~onComplete=Observer.forwardOnComplete(observer),
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    keepObserver^;
  };

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));