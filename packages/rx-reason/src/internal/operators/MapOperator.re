let operator = (mapper, observer) => {
  let mapObserver = ref(Observer.disposed);
  let onNext =
    Functions.earlyReturnsUnit1(next => {
      let mapped =
        try (mapper(next)) {
        | exn =>
          mapObserver^ |> Observer.complete(~exn);
          Functions.returnUnit();
        };
      observer |> Observer.next(mapped);
    });
  mapObserver :=
    observer
    |> Observer.delegate(
         ~onNext,
         ~onComplete=Observer.forwardOnComplete(observer),
       );
  mapObserver^;
};

let lift = (mapper, observable) =>
  observable |> ObservableSource.lift(operator(mapper));