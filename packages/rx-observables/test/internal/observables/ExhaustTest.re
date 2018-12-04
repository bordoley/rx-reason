open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "exhaust",
    [
      observableIt(
        "exhausts",
        ~nextToString=string_of_int,
        ~source=
          scheduler => {
            let childObservableA =
              ofRelativeTimeNotifications(
                ~scheduler,
                [
                  (0.0, RxNotification.next(1)),
                  (10.0, RxNotification.next(2)),
                  (20.0, RxNotification.next(3)),
                  (30.0, RxNotification.next(4)),
                  (40.0, RxNotification.complete(None)),
                ],
              );

            let childObservableB =
              ofRelativeTimeNotifications(
                ~scheduler,
                [
                  (0.0, RxNotification.next(5)),
                  (10.0, RxNotification.next(6)),
                  (19.0, RxNotification.next(7)),
                  (30.0, RxNotification.next(8)),
                  (40.0, RxNotification.complete(None)),
                ],
              );

            ofAbsoluteTimeNotifications(
              ~scheduler,
              [
                (0.0, RxNotification.next(childObservableA)),
                (15.0, RxNotification.next(childObservableB)),
                (35.0, RxNotification.next(childObservableA)),
                (60.0, RxNotification.next(childObservableB)),
                (75.0, RxNotification.complete(None)),
              ],
            )
            |> RxObservables.exhaust;
          },
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.next(3),
          RxNotification.next(4),
          RxNotification.next(5),
          RxNotification.next(6),
          RxNotification.next(7),
          RxNotification.next(8),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );