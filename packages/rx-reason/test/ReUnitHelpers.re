open ReUnit;
open ReUnit.Test;

let expectObservableToProduce =
    (~nextEquals=(===), ~nextToString, expected, observable) =>
  observable
  |> Observable.materialize
  |> Observable.toList
  |> Observable.onNext(
       Expect.toBeEqualToListWith(
         ~equals=Notification.equals(~nextEquals),
         ~toString=Notification.toString(~nextToString),
         expected,
       ),
     )
  |> Observable.first
  |> Observable.subscribe(
       ~onComplete=
         fun
         | Some(exn) => raise(exn)
         | _ => (),
     );

let observableIt =
    (
      name,
      ~nextEquals=(===),
      ~nextToString,
      ~source: ClockScheduler.t => Observable.t('a),
      ~expected: list(Notification.t('b)),
      (),
    ) =>
  it(
    name,
    () => {
      let vts = VirtualTimeScheduler.create();
      let scheduler = vts |> VirtualTimeScheduler.toClockScheduler;

      let subscription =
        source(scheduler)
        |> expectObservableToProduce(~nextEquals, ~nextToString, expected);

      vts |> VirtualTimeScheduler.run;
      subscription |> CompositeDisposable.dispose;
    },
  );