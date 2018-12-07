open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "isEmpty",
    [
      observableIt(
        "return false if not empty",
        ~nextToString=string_of_bool,
        ~source=_ => RxObservables.ofValue(1) |> RxObservables.isEmpty,
        ~expected=[
          RxNotification.next(false),
          RxNotification.complete(None),
        ],
        (),
      ),
      observableIt(
        "return true if empty",
        ~nextToString=string_of_bool,
        ~source=_ => RxObservables.empty() |> RxObservables.isEmpty,
        ~expected=[
          RxNotification.next(true),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );