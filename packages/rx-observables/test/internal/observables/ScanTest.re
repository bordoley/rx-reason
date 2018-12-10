open ReUnit.Test;
open RxReUnit;

let test =      describe(
        "scan",
        [
          observableIt(
            "publishes all intermediate values, including the initial accumulator value",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([2, 3, 4])
                |> RxObservables.scan((. acc, next) => acc + next, 0),
            ~expected=[
              RxNotification.next(0),
              RxNotification.next(2),
              RxNotification.next(5),
              RxNotification.next(9),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      );