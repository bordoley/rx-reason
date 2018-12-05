open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "combineLatest2",
    [
      observableIt(
        "combines latest values from each observable",
        ~nextToString=string_of_int,
        ~source=
          scheduler =>
            RxObservables.combineLatest2(
              ~selector=(a, b) => a + b,
              RxReUnit.ofAbsoluteTimeNotifications(
                ~scheduler,
                [
                  (1.0, RxNotification.next(0)),
                  (3.0, RxNotification.next(2)),
                  (5.0, RxNotification.next(4)),
                  (6.0, RxNotification.complete(None)),
                ],
              ),
              RxReUnit.ofAbsoluteTimeNotifications(
                ~scheduler,
                [
                  (0.0, RxNotification.next(0)),
                  (2.0, RxNotification.next(2)),
                  (4.0, RxNotification.next(4)),
                  (5.0, RxNotification.complete(None)),
                ],
              ),
            ),
        ~expected=[
          RxNotification.next(0),
          RxNotification.next(2),
          RxNotification.next(4),
          RxNotification.next(6),
          RxNotification.next(8),
          RxNotification.complete(None),
        ],
        (),
      ),
      observableIt(
        "completes when either observable completes with an exception",
        ~nextToString=string_of_int,
        ~source=
          scheduler =>
            RxObservables.combineLatest2(
              ~selector=(a, b) => a + b,
              RxObservables.raise(Division_by_zero)
              |> RxObservables.delay(~scheduler, 2.0),
              RxReUnit.ofAbsoluteTimeNotifications(
                ~scheduler,
                [
                  (1.0, RxNotification.next(0)),
                  (3.0, RxNotification.next(2)),
                  (5.0, RxNotification.next(4)),
                  (6.0, RxNotification.complete(None)),
                ],
              ),
            ),
        ~expected=[RxNotification.complete(Some(Division_by_zero))],
        (),
      ),
    ],
  );