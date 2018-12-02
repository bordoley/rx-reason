open ReUnit;
open ReUnit.Test;


let observableToList = {
  let toListAccumulator = (acc, next) => [next, ...acc];

  observable =>
    observable |> RxObservables.scan(toListAccumulator, []) |> RxObservables.last |> RxObservables.map(List.rev);
};

let rxNotificationEquals =
    (
      ~exnEquals=RxFunctions.referenceEquality,
      ~nextEquals=RxFunctions.referenceEquality,
      a,
      b,
    ) =>
  switch (a, b) {
  | (RxNotification.Next(a), RxNotification.Next(b)) => nextEquals(a, b)
  | (RxNotification.Complete(Some(a)), RxNotification.Complete(Some(b))) => exnEquals(a, b)
  | (RxNotification.Complete(None), RxNotification.Complete(None)) => true
  | _ => false
  };

let rxNotificationToString = (~exnToString=_ => "exn", ~nextToString, notif) =>
  switch (notif) {
  | RxNotification.Next(v) => "Next(" ++ nextToString(v) ++ ")"
  | RxNotification.Complete(Some(exn)) =>
    "CompleteWithException(" ++ exnToString(exn) ++ ")"
  | RxNotification.Complete(None) => "Complete"
  };

let expectObservableToProduce =
    (~nextEquals=(===), ~nextToString, expected, observable) =>
  observable
  |> RxObservables.materialize
  |> observableToList
  |> RxObservables.onNext(
       Expect.toBeEqualToListWith(
         ~equals=rxNotificationEquals(~nextEquals),
         ~toString=rxNotificationToString(~nextToString),
         expected,
       ),
     )
  |> RxObservables.first
  |> RxObservables.onComplete(
       fun
       | Some(exn) => raise(exn)
       | _ => (),
     )
  |> RxObservable.subscribe;

let observableIt =
    (
      name,
      ~nextEquals=(===),
      ~nextToString,
      ~source: RxScheduler.t => RxObservable.t('a),
      ~expected: list(RxNotification.t('b)),
      (),
    ) =>
  it(
    name,
    () => {
      let vts = RxVirtualTimeScheduler.create();
      let scheduler = vts |> RxVirtualTimeScheduler.asScheduler;

      let subscription =
        source(scheduler)
        |> expectObservableToProduce(~nextEquals, ~nextToString, expected);

      vts |> RxVirtualTimeScheduler.run;
      subscription |> RxDisposable.dispose;
    },
  );