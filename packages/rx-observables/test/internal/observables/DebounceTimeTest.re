open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "debounceTime",
    [
      observableIt(
        "debounces",
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
                (16.0, RxNotification.next(5)),
                (17.0, RxNotification.next(6)),
                (18.0, RxNotification.complete(None)),
              ],
            )
            |> RxObservables.debounceTime(~scheduler, 5.0),
        ~expected=[
          RxNotification.next(3),
          RxNotification.next(6),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );