exception CompleteWithoutErrorException;
let completeWithoutErrorExn = Some(CompleteWithoutErrorException);

let operator = observer => {
  let firstObserver = ref(Observer.disposed);
  firstObserver :=
    Observer.create(
      ~onNext=
        next => {
          observer |> Observer.next(next);
          firstObserver^ |> Observer.complete(~exn=?completeWithoutErrorExn);
        },
      ~onComplete=
        exn => {
          let exn =
            switch (exn) {
            | Some(CompleteWithoutErrorException) => None
            | Some(_) => exn
            | _ => Some(EmptyException.exn)
            };
          observer |> Observer.complete(~exn?);
        },
      ~onDispose=Observer.forwardOnDispose(observer),
    );
  firstObserver^;
};

let lift = observable => observable |> ObservableSource.lift(operator);