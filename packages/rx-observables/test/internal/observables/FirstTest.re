open ReUnit.Test;
open RxReUnit;

let test = describe(
  "first",
  [
    observableIt(
      "publishes the first observed value",
      ~nextToString=string_of_int,
      ~source=_ => RxObservables.ofList([2, 3]) |> RxObservables.first,
      ~expected=[
        RxNotification.next(2),
        RxNotification.complete(None),
      ],
      (),
    ),
    observableIt(
      "passes through completed exceptions",
      ~nextToString=string_of_int,
      ~source=
        _ =>
          RxObservables.raise(Division_by_zero) |> RxObservables.first,
      ~expected=[RxNotification.complete(Some(Division_by_zero))],
      (),
    ),
    observableIt(
      "completes with exception if no values are produced",
      ~nextToString=string_of_int,
      ~source=_ => RxObservables.empty() |> RxObservables.first,
      ~expected=[RxNotification.complete(Some(RxEmptyException.Exn))],
      (),
    ),
  ],
);