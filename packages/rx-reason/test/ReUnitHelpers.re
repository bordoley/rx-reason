open ReUnit;
open ReUnit.Test;

let expectObservableToProduce =
    (~nextEquals=(===), ~nextToString, expected, observable) =>
  observable
  |> Observable.lift(
       Operators.(
         materialize
         >> toList
         >> onNext(
              Expect.toBeEqualToListWith(
                ~equals=Notification.equals(~nextEquals),
                ~toString=Notification.toString(~nextToString),
                expected,
              ),
            )
         >> first
       ),
     )
  |> Observable.subscribe(~onComplete=exn =>
       switch (exn) {
       | Some(exn) => raise(exn)
       | _ => ()
       }
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
      subscription |> Disposable.dispose;
    },
  );

let operatorIt =
    (
      name,
      ~nextEquals=(===),
      ~nextToString,
      ~source: ClockScheduler.t => Observable.t('a),
      ~operator: ClockScheduler.t => Operator.t('a, 'b),
      ~expected: list(Notification.t('b)),
      (),
    ) =>
  observableIt(
    name,
    ~nextEquals,
    ~nextToString,
    ~source=
      scheduler =>
        source(scheduler) |> Observable.lift(operator(scheduler)),
    ~expected,
    (),
  );