open ReUnit.Test;
open RxReUnit;

let test = describe(
  "every",
  [
    observableIt(
      "returns true for an subscriber that completes without producing values",
      ~nextToString=string_of_bool,
      ~source=
        _ => RxObservables.empty() |> RxObservables.every(i => i > 10),
      ~expected=[
        RxNotification.next(true),
        RxNotification.complete(None),
      ],
      (),
    ),
    observableIt(
      "completes with false on the first observed value that fails the predicate",
      ~nextToString=string_of_int,
      ~source=
        scheduler =>
          ofAbsoluteTimeNotifications(
            ~scheduler,
            [
              (1.0, RxNotification.next(12)),
              (2.0, RxNotification.next(8)),
              (3.0, RxNotification.next(14)),
              (5.0, RxNotification.next(6)),
              (6.0, RxNotification.complete(None)),
            ],
          )
          |> RxObservables.every(i => i > 10)
          |> RxObservables.map(_ =>
               scheduler |> RxScheduler.now |> int_of_float
             ),
      ~expected=[
        RxNotification.next(2),
        RxNotification.complete(None),
      ],
      (),
    ),
    observableIt(
      "completes with true if all values pass the predicate",
      ~nextToString=string_of_bool,
      ~source=
        _ =>
          RxObservables.ofList([12, 13])
          |> RxObservables.every(i => i > 10),
      ~expected=[
        RxNotification.next(true),
        RxNotification.complete(None),
      ],
      (),
    ),
  ],
);