open ReUnit.Test;
open RxReUnit;

let test = describe(
  "withLatestFrom",
  [
    observableIt(
      "drops values from the source, if there is no latest value",
      ~nextToString=string_of_int,
      ~source=
        scheduler =>
          ofAbsoluteTimeNotifications(
            ~scheduler,
            [
              (0.0, RxNotification.next(1)),
              (200.0, RxNotification.next(2)),
              (400.0, RxNotification.next(3)),
              (600.0, RxNotification.next(4)),
              (700.0, RxNotification.complete(None)),
            ],
          )
          |> RxObservables.withLatestFrom(
               ~selector=(a, b) => a + b,
               ofAbsoluteTimeNotifications(
                 ~scheduler,
                 [
                   (100.0, RxNotification.next(1)),
                   (250.0, RxNotification.next(2)),
                   (300.0, RxNotification.next(3)),
                   (450.0, RxNotification.next(4)),
                   (500.0, RxNotification.complete(None)),
                 ],
               ),
             ),
      ~expected=[
        RxNotification.next(3),
        RxNotification.next(6),
        RxNotification.next(8),
        RxNotification.complete(None),
      ],
      (),
    ),
  ],
);