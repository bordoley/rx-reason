open ReUnit.Test;
open RxReUnit;

let test = describe(
  "find",
  [
    observableIt(
      "finds the first matching element and completes",
      ~nextToString=string_of_int,
      ~source=
        _ =>
          RxObservables.ofList([1, 3, 10, 6, 8])
          |> RxObservables.find((. x) => x mod 2 === 0),
      ~expected=[
        RxNotification.next(10),
        RxNotification.complete(None),
      ],
      (),
    ),
  ],
);