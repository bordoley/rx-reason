open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "some",
    [
      observableIt(
        "returns false for an subscriber that completes without producing values",
        ~nextToString=string_of_bool,
        ~source=_ => RxObservables.empty() |> RxObservables.some(i => i > 10),
        ~expected=[
          RxNotification.next(false),
          RxNotification.complete(None),
        ],
        (),
      ),
      observableIt(
        "returns false for an subscriber for which no value passes the predicate",
        ~nextToString=string_of_bool,
        ~source=
          _ =>
            RxObservables.ofList([5, 6, 7]) |> RxObservables.some(i => i > 10),
        ~expected=[
          RxNotification.next(false),
          RxNotification.complete(None),
        ],
        (),
      ),
      observableIt(
        "completes with true on the first observed value that passed the predicate",
        ~nextToString=string_of_int,
        ~source=
          ({now} as scheduler) =>
            ofAbsoluteTimeNotifications(
              ~scheduler,
              [
                (1.0, RxNotification.next(8)),
                (2.0, RxNotification.next(11)),
                (3.0, RxNotification.next(14)),
                (4.0, RxNotification.next(6)),
                (5.0, RxNotification.next(5)),
                (6.0, RxNotification.complete(None)),
              ],
            )
            |> RxObservables.some(i => i > 10)
            |> RxObservables.map(_ => now() |> int_of_float),
        ~expected=[RxNotification.next(2), RxNotification.complete(None)],
        (),
      ),
    ],
  );