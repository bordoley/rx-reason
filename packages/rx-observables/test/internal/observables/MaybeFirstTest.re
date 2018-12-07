open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "maybeFirst",
    [
      observableIt(
        "publishes the first observed value",
        ~nextToString=string_of_int,
        ~source=
          _ => RxObservables.ofList([1, 2, 3]) |> RxObservables.maybeFirst,
        ~expected=[RxNotification.next(1), RxNotification.complete(None)],
        (),
      ),
      observableIt(
        "passes through completed exceptions",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.raise(Division_by_zero) |> RxObservables.maybeFirst,
        ~expected=[RxNotification.complete(Some(Division_by_zero))],
        (),
      ),
      observableIt(
        "ignores EmptyException and completes normally",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.empty()
            |> RxObservables.first
            |> RxObservables.maybeFirst,
        ~expected=[RxNotification.complete(None)],
        (),
      ),
    ],
  );