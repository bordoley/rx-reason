open ReUnit;
open ReUnit.Test;

let expectObservableToProduce =
    (~nextEquals=(===), ~nextToString, expected, observable) =>
  observable
  |> RxObservables.materialize
  |> RxObservables.toList
  |> RxObservables.onNext(
       Expect.toBeEqualToListWith(
         ~equals=RxNotification.equals(~nextEquals),
         ~toString=RxNotification.toString(~nextToString),
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