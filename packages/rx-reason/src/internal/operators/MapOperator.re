let operator = (mapper, observer) => {
  let mapObserver = ref(Observer.disposed);
  mapObserver :=
    Observer.delegate(
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
      observer,
    );
  mapObserver^;
};

let lift = (mapper, observable) =>
  observable |> ObservableSource.lift(operator(mapper));