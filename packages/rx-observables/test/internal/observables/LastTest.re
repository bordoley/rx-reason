open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "last",
    [
      observableIt(
        "publishes the last observed value and disposes",
        ~nextToString=string_of_int,
        ~source=_ => RxObservables.ofList([1, 2, 3]) |> RxObservables.last,
        ~expected=[RxNotification.next(3), RxNotification.complete(None)],
        (),
      ),
      observableIt(
        "passes through completed exceptions",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofNotifications([
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.complete(Some(Division_by_zero)),
            ])
            |> RxObservables.last,
        ~expected=[RxNotification.complete(Some(Division_by_zero))],
        (),
      ),
      observableIt(
        "completes with exception if no values are produced",
        ~nextToString=string_of_int,
        ~source=_ => RxObservables.empty() |> RxObservables.last,
        ~expected=[RxNotification.complete(Some(RxEmptyException.Exn))],
        (),
      ),
    ],
  );