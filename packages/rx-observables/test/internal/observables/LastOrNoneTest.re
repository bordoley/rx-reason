open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "lastOrNone",
    [
      observableIt(
        "publishes the Some of the last observed value and completes",
        ~nextEquals=Option.equals,
        ~nextToString=Option.toString(~toString=string_of_int),
        ~source=_ => RxObservables.ofList([2, 3]) |> RxObservables.lastOrNone,
        ~expected=[
          RxNotification.next(Some(3)),
          RxNotification.complete(None),
        ],
        (),
      ),
      observableIt(
        "passes through completed exceptions",
        ~nextEquals=Option.equals,
        ~nextToString=Option.toString(~toString=string_of_int),
        ~source=
          _ =>
            RxObservables.ofNotifications([
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.complete(Some(Division_by_zero)),
            ])
            |> RxObservables.lastOrNone,
        ~expected=[RxNotification.complete(Some(Division_by_zero))],
        (),
      ),
      observableIt(
        "ignores EmptyException, publishes None, and completes normally",
        ~nextEquals=Option.equals,
        ~nextToString=Option.toString(~toString=string_of_int),
        ~source=
          _ =>
            RxObservables.empty()
            |> RxObservables.first
            |> RxObservables.lastOrNone,
        ~expected=[
          RxNotification.next(None),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );