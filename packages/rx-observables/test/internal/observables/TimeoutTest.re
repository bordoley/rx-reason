open ReUnit.Test;
open RxReUnit;

let test = describe(
  "timeout",
  [
    observableIt(
      "when timeout does not expire",
      ~nextToString=string_of_int,
      ~source=
        scheduler =>
          ofRelativeTimeNotifications(
            ~scheduler,
            [
              (0.0, RxNotification.next(1)),
              (4.0, RxNotification.next(2)),
              (6.0, RxNotification.next(3)),
              (10.0, RxNotification.next(4)),
              (14.0, RxNotification.complete(None)),
            ],
          )
          |> RxObservables.timeout(~scheduler, 5.0),
      ~expected=[
        RxNotification.next(1),
        RxNotification.next(2),
        RxNotification.next(3),
        RxNotification.next(4),
        RxNotification.complete(None),
      ],
      (),
    ),
    observableIt(
      "when timeout expires",
      ~nextToString=string_of_int,
      ~source=
        scheduler =>
          ofRelativeTimeNotifications(
            ~scheduler,
            [
              (0.0, RxNotification.next(1)),
              (4.0, RxNotification.next(2)),
              (6.0, RxNotification.next(3)),
              (15.0, RxNotification.next(4)),
              (20.0, RxNotification.complete(None)),
            ],
          )
          |> RxObservables.timeout(~scheduler, 5.0),
      ~expected=[
        RxNotification.next(1),
        RxNotification.next(2),
        RxNotification.next(3),
        RxNotification.complete(Some(RxTimeoutException.Exn)),
      ],
      (),
    ),
  ],
);