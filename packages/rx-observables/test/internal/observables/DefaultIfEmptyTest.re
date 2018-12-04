open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "defaultIfEmpty",
    [
      observableIt(
        "returns the default if empty",
        ~nextToString=string_of_int,
        ~source=_ => RxObservables.empty() |> RxObservables.defaultIfEmpty(1),
        ~expected=[RxNotification.next(1), RxNotification.complete(None)],
        (),
      ),
      observableIt(
        "passes through if not empty",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofList([1, 2, 3])
            |> RxObservables.defaultIfEmpty(1),
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.next(3),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );