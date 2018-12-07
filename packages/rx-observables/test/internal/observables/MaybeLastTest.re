open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "maybeLast",
    [
      observableIt(
        "publishes the last observed value",
        ~nextToString=string_of_int,
        ~source=
          _ => RxObservables.ofList([1, 2, 3]) |> RxObservables.maybeLast,
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
              RxNotification.complete(Some(Division_by_zero)),
            ])
            |> RxObservables.maybeLast,
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
            |> RxObservables.maybeLast,
        ~expected=[RxNotification.complete(None)],
        (),
      ),
    ],
  );