open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "startWithValue",
    [
      observableIt(
        "prepends the values",
        ~nextToString=string_of_int,
        ~source=
          _ => RxObservables.startWithValue(1, RxObservables.ofList([2, 3])),
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