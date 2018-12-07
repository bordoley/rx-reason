open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "keep",
    [
      observableIt(
        "completes the subscriber when the predicate throws an exception",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofValue(1)
            |> RxObservables.keep(_ => raise(Division_by_zero)),
        ~expected=[RxNotification.complete(Some(Division_by_zero))],
        (),
      ),
      observableIt(
        "completes the subscriber when the keep subscriber is completed",
        ~nextToString=string_of_int,
        ~source=
          _ => RxObservables.ofValue(1) |> RxObservables.keep(_ => true),
        ~expected=[RxNotification.next(1), RxNotification.complete(None)],
        (),
      ),
    ],
  );