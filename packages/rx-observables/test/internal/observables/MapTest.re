open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "map",
    [
      observableIt(
        "completes the subscriber when the mapper throws an exception",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofList([1, 2, 3])
            |> RxObservables.map(_ => raise(Division_by_zero)),
        ~expected=[RxNotification.complete(Some(Division_by_zero))],
        (),
      ),
      observableIt(
        "completes the subscriber when the mapping subscriber is completed",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofList([1, 2, 3]) |> RxObservables.map(i => i + 1),
        ~expected=[
          RxNotification.next(2),
          RxNotification.next(3),
          RxNotification.next(4),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );