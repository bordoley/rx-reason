open ReUnit;
open ReUnit.Test;

let expectObservableToProduce =
    (~nextEquals=(===), ~nextToString, expected, observable) =>
  observable
  |> Observable.lift(Operators.materialize)
  |> Observables.toList
  |> Observable.pipe2(
       Operators.onNext(
         Expect.toBeEqualToListWith(
           ~equals=Notification.equals(~nextEquals),
           ~toString=Notification.toString(~nextToString),
           expected,
         ),
       ),
       Operators.first,
     )
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
      ~source: Scheduler.t => Observable.t('a),
      ~expected: list(Notification.t('b)),
      (),
    ) =>
  it(
    name,
    () => {
      let vts = VirtualTimeScheduler.create();
      let scheduler = vts |> VirtualTimeScheduler.asScheduler;

      let subscription =
        source(scheduler)
        |> expectObservableToProduce(~nextEquals, ~nextToString, expected);

      vts |> VirtualTimeScheduler.run;
      subscription |> Disposable.dispose;
    },
  );