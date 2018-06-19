let operator = (predicate, observer) => {
  let keepObserver = ref(Observer.disposed);

  let onNext =
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
    });

  keepObserver :=
    observer
    |> Observer.delegate(
         ~onNext,
         ~onComplete=Observer.forwardOnComplete(observer),
       );
  keepObserver^;
};

let lift = (predicate, observable) =>
  observable |> ObservableSource.lift(operator(predicate));