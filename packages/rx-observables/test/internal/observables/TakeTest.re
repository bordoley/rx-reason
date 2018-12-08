open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "take",
    [
      observableIt(
        "emits the number of requested values ",
        ~nextToString=string_of_int,
        ~source=
          _ => {
            let source =
              RxObservables.ofNotifications([
                RxNotification.next(1),
                RxNotification.next(2),
                RxNotification.next(3),
                RxNotification.next(4),
                RxNotification.complete(None),
              ]);

            source |> RxObservables.take(2);
          },
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.complete(None),
        ],
        (),
      ),
    ],
  );