open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "takeUntil",
    [
      observableIt(
        "emits values until notifier produces a value",
        ~nextToString=string_of_int,
        ~source=
          scheduler => {
            let source =
              ofRelativeTimeNotifications(
                ~scheduler,
                [
                  (0.0, RxNotification.next(1)),
                  (4.0, RxNotification.next(2)),
                  (6.0, RxNotification.next(3)),
                  (10.0, RxNotification.next(4)),
                  (14.0, RxNotification.complete(None)),
                ],
              );

            let notifier =
              ofRelativeTimeNotifications(
                ~scheduler,
                [(5.0, RxNotification.next())],
              );
            source |> RxObservables.takeUntil(notifier);
          },
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.complete(None),
        ],
        (),
      ),
      observableIt(
        "notifier emitting an exception completes the observable",
        ~nextToString=string_of_int,
        ~source=
          scheduler => {
            let source =
              ofRelativeTimeNotifications(
                ~scheduler,
                [
                  (0.0, RxNotification.next(1)),
                  (4.0, RxNotification.next(2)),
                  (6.0, RxNotification.next(3)),
                  (10.0, RxNotification.next(4)),
                  (14.0, RxNotification.complete(None)),
                ],
              );

            let notifier =
              ofRelativeTimeNotifications(
                ~scheduler,
                [(5.0, RxNotification.complete(Some(Division_by_zero)))],
              );
            source |> RxObservables.takeUntil(notifier);
          },
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.complete(Some(Division_by_zero)),
        ],
        (),
      ),
    ],
  );