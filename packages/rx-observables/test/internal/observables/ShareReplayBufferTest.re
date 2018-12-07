open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "shareReplayBuffer",
    [
      observableIt(
        "replays event on subscribe",
        ~nextToString=string_of_int,
        ~source=
          scheduler => {
            let source =
              ofAbsoluteTimeNotifications(
                ~scheduler,
                [
                  (0.0, RxNotification.next(1)),
                  (2.0, RxNotification.next(2)),
                  (4.0, RxNotification.next(3)),
                  (6.0, RxNotification.next(4)),
                  (9.0, RxNotification.complete(None)),
                ],
              )
              |> RxObservables.shareReplayBuffer(2);

            RxObservables.mergeList([
              source,
              source |> RxObservables.subscribeOn(~delay=5.0, scheduler),
            ]);
          },
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.next(3),
          RxNotification.next(2),
          RxNotification.next(3),
          RxNotification.next(4),
          RxNotification.next(4),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );