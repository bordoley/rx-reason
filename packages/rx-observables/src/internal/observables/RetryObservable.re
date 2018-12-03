let defaultPredicate =
  fun
  | None => false
  | Some(_) => true;

let create = (~predicate=RxFunctions.alwaysTrue1) => {
  let predicate =
    predicate === RxFunctions.alwaysTrue1 ?
      defaultPredicate :
      (
        fun
        | None => false
        | Some(exn) => predicate(exn)
      );

  observable =>
    observable
    |> RxObservable.lift(RepeatOperator.create(predicate, observable));
};