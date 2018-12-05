open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxEvent",
    [
      describe(
        "asObservable",
        [
          it("disposed instance emits no values", () => {
            let event = RxEvent.disposed;
            let observedValue = ref(0);

            event
            |> RxEvent.asObservable
            |> RxObservable.lift(
                 RxSubscriber.decorate(
                   ~onNext=(_, next) => observedValue := next,
                   ~onComplete=(_, _) => (),
                 ),
               )
            |> RxObservable.connect
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToTrue;

            event |> RxEvent.dispatch(1);
            observedValue^ |> Expect.toBeEqualToInt(0);
          }),
          it("subscribe after dispose returns a disposed Disposable", () => {
            let event = RxEvent.create();
            event |> RxEvent.dispose;

            event
            |> RxEvent.asObservable
            |> RxObservable.connect
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it("emits no values after being disposed", () => {
            let event = RxEvent.create();
            let observedValue = ref(0);

            event |> RxEvent.dispose;

            event
            |> RxEvent.asObservable
            |> RxObservable.lift(
                 RxSubscriber.decorate(
                   ~onNext=(_, next) => observedValue := next,
                   ~onComplete=(_, _) => (),
                 ),
               )
            |> RxObservable.connect
            |> ignore;

            event |> RxEvent.dispatch(1); 
            observedValue^ |> Expect.toBeEqualToInt(0);
          }),
          it(
            "no longer emits notifications when the subscription is disposed",
            () => {
            let event = RxEvent.create();

            let observedValue1 = ref(0);
            let subscription1 =
            event
              |> RxEvent.asObservable
              |> RxObservable.lift(
                   RxSubscriber.decorate(
                     ~onNext=(_, next) => observedValue1 := next,
                     ~onComplete=(_, _) => (),
                   ),
                 )
              |> RxObservable.connect;

            let observedValue2 = ref(0);
            let subscription2 =
            event
              |> RxEvent.asObservable
              |> RxObservable.lift(
                   RxSubscriber.decorate(
                     ~onNext=(_, next) => observedValue2 := next,
                     ~onComplete=(_, _) => (),
                   ),
                 )
              |> RxObservable.connect;

            observedValue1^ |> Expect.toBeEqualToInt(0);
            observedValue2^ |> Expect.toBeEqualToInt(0);

            event |> RxEvent.dispatch(1);
            observedValue1^ |> Expect.toBeEqualToInt(1);
            observedValue2^ |> Expect.toBeEqualToInt(1);

            subscription1 |> RxDisposable.dispose;
            event |> RxEvent.dispatch(2);
            observedValue1^ |> Expect.toBeEqualToInt(1);
            observedValue2^ |> Expect.toBeEqualToInt(2);

            subscription2 |> RxDisposable.dispose;
            event |> RxEvent.dispatch(3);
            observedValue1^ |> Expect.toBeEqualToInt(1);
            observedValue2^ |> Expect.toBeEqualToInt(2);
          }),
        ],
      ),
    ],
  );