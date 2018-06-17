let operator = mapper =>
  observer => {
    let mapObserver = ref(Observer.disposed);
    mapObserver :=
      Observer.create(
        ~onNext=
          Functions.earlyReturnsUnit1(next => {
            let mapped =
              try (mapper(next)) {
              | exn =>
                mapObserver^ |> Observer.complete(~exn);
                Functions.returnUnit();
              };
            observer |> Observer.next(mapped);
          }),
        ~onComplete=Observer.forwardOnComplete(observer),
        ~onDispose=Observer.forwardOnDispose(observer),
      );
    mapObserver^;
  };

let lift = (mapper, observable) =>
  observable |> ObservableSource.lift(operator(mapper));