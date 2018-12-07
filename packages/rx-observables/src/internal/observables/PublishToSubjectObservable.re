let create = (subject, observable) => observable |> RxObservable.lift(
  PublishToSubjectOperator.create(subject)
);