open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "concatList",
    [
      observableIt(
        "synchronous and asynchronous observables",
        ~nextToString=string_of_int,
        ~source=
          scheduler =>
            RxObservables.concatList([
              ofRelativeTimeNotifications(
                ~scheduler,
                [
                  (1.0, RxNotification.next(7)),
                  (3.0, RxNotification.next(9)),
                  (4.0, RxNotification.complete(None)),
                ],
              ),
              RxObservables.ofList([1, 2, 3]),
              ofRelativeTimeNotifications(
                ~scheduler,
                [
                  (2.0, RxNotification.next(8)),
                  (4.0, RxNotification.next(10)),
                  (5.0, RxNotification.complete(None)),
                ],
              ),
              RxObservables.ofList([4, 5, 6]),
            ]),
        ~expected=[
          RxNotification.next(7),
          RxNotification.next(9),
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.next(3),
          RxNotification.next(8),
          RxNotification.next(10),
          RxNotification.next(4),
          RxNotification.next(5),
          RxNotification.next(6),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );