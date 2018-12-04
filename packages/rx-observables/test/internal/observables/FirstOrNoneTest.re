open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "firstOrNone",
    [
      observableIt(
        "publishes Some of the first observed value",
        ~nextEquals=Option.equals,
        ~nextToString=Option.toString(~toString=string_of_int),
        ~source=
          _ => RxObservables.ofList([2, 3]) |> RxObservables.firstOrNone,
        ~expected=[
          RxNotification.next(Some(2)),
          RxNotification.complete(None),
        ],
        (),
      ),
      observableIt(
        "publishes Some of the first observed value",
        ~nextEquals=Option.equals,
        ~nextToString=Option.toString(~toString=string_of_int),
        ~source=
          _ =>
            RxObservables.raise(Division_by_zero) |> RxObservables.firstOrNone,
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
            |> RxObservables.firstOrNone,
        ~expected=[
          RxNotification.next(None),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );