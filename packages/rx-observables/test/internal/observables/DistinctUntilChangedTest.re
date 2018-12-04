open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "distinctUntilChanged",
    [
      observableIt(
        "removes duplicates",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofList([1, 1, 1, 3, 5, 3, 3, 1])
            |> RxObservables.distinctUntilChanged(~equals=(===)),
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(3),
          RxNotification.next(5),
          RxNotification.next(3),
          RxNotification.next(1),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );