open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "ignoreElements",
    [
      observableIt(
        "ignores all elements and publishes an exception",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofNotifications([
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.complete(Some(Division_by_zero)),
            ])
            |> RxObservables.ignoreElements,
        ~expected=[RxNotification.complete(Some(Division_by_zero))],
        (),
      ),
    ],
  );