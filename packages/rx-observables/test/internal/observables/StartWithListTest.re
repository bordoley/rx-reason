open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "startWithlist",
    [
      observableIt(
        "prepends the values",
        ~nextToString=string_of_int,
        ~source=
          scheduler =>
            RxObservables.startWithList(
              [1, 2, 3],
              ofRelativeTimeNotifications(
                ~scheduler,
                [
                  (1.0, RxNotification.next(4)),
                  (2.0, RxNotification.next(5)),
                  (3.0, RxNotification.complete(None)),
                ],
              ),
            ),
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.next(3),
          RxNotification.next(4),
          RxNotification.next(5),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );