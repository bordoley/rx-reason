open ReUnit;
open ReUnit.Test;

let observableToList = {
  let toListAccumulator = (acc, next) => [next, ...acc];

  observable =>
    observable
    |> RxObservables.scan(toListAccumulator, [])
    |> RxObservables.last
    |> RxObservables.map(List.rev);
};

let rxNotificationEquals =
    (
      ~exnEquals=RxFunctions.referenceEquality,
      ~nextEquals=RxFunctions.referenceEquality,
      a,
      b,
    ) =>
  RxNotification.map(
    ~onNext=
      vA =>
        RxNotification.map(
          ~onNext=vB => nextEquals(vA, vB),
          ~onComplete=_ => false,
          b,
        ),
    ~onComplete=
      exnA =>
        RxNotification.map(
          ~onNext=_ => false,
          ~onComplete=
            exnB =>
              switch (exnA, exnB) {
              | (Some(a), Some(b)) => exnEquals(a, b)
              | (None, None) => true
              | _ => false
              },
          b,
        ),
    a,
  );

let rxNotificationToString = (~exnToString=_ => "exn", ~nextToString, notif) =>
  notif
  |> RxNotification.map(
       ~onNext=v => "Next(" ++ nextToString(v) ++ ")",
       ~onComplete=
         fun
         | Some(exn) => "CompleteWithException(" ++ exnToString(exn) ++ ")"
         | None => "Complete",
     );

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
  |> RxObservables.first;

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

      let thrownException = ref(None);
      let subscription =
        source(scheduler)
        |> expectObservableToProduce(~nextEquals, ~nextToString, expected)
        |> RxObservables.onComplete(exn => thrownException := exn)
        |> RxObservable.subscribe;

      vts |> RxVirtualTimeScheduler.run;
      subscription |> RxDisposable.dispose;

      switch (thrownException^) {
      | Some(exn) => raise(exn)
      | _ => ()
      };
    },
  );