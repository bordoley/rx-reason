open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "mapTo",
    [
      observableIt(
        "maps any input to value",
        ~nextToString=RxFunctions.identity,
        ~source=
          _ => RxObservables.ofList([1, 2, 3]) |> RxObservables.mapTo("a"),
        ~expected=[
          RxNotification.next("a"),
          RxNotification.next("a"),
          RxNotification.next("a"),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );