open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxValue",
    [
      describe(
        "asObservable",
        [
          it("disposed instance emits no values", () => {
            let value = RxValue.disposed;
            let observedValue = ref(0);

            value |> RxValue.dispose;

            value
            |> RxValue.asObservable
            |> RxObservable.lift(
                 RxSubscriber.decorate(
                   ~onNext=(_, next) => observedValue := next,
                   ~onComplete=(_, _) => (),
                 ),
               )
            |> RxObservable.subscribe
            |> ignore;

            value |> RxValue.update(old => old + 1);
            observedValue^ |> Expect.toBeEqualToInt(0);
          }),
          it("emits no values after being disposed", () => {
            let value = RxValue.create(1);
            let observedValue = ref(0);

            value |> RxValue.dispose;

            value
            |> RxValue.asObservable
            |> RxObservable.lift(
                 RxSubscriber.decorate(
                   ~onNext=(_, next) => observedValue := next,
                   ~onComplete=(_, _) => (),
                 ),
               )
            |> RxObservable.subscribe
            |> ignore;

            observedValue^ |> Expect.toBeEqualToInt(0);
          }),
          it("publishes the current value on subscribe", () => {
            let value = RxValue.create(1);
            let observedValue = ref(0);

            value
            |> RxValue.asObservable
            |> RxObservable.lift(
                 RxSubscriber.decorate(
                   ~onNext=(_, next) => observedValue := next,
                   ~onComplete=(_, _) => (),
                 ),
               )
            |> RxObservable.subscribe
            |> ignore;

            observedValue^ |> Expect.toBeEqualToInt(1);
          }),
          it(
            "no longer emits notifications when the subscription is disposed",
            () => {
            let value = RxValue.create(0);

            let observedValue1 = ref(0);
            let subscription1 =
              value
              |> RxValue.asObservable
              |> RxObservable.lift(
                   RxSubscriber.decorate(
                     ~onNext=(_, next) => observedValue1 := next,
                     ~onComplete=(_, _) => (),
                   ),
                 )
              |> RxObservable.subscribe;

            let observedValue2 = ref(0);
            let subscription2 =
              value
              |> RxValue.asObservable
              |> RxObservable.lift(
                   RxSubscriber.decorate(
                     ~onNext=(_, next) => observedValue2 := next,
                     ~onComplete=(_, _) => (),
                   ),
                 )
              |> RxObservable.subscribe;

            observedValue1^ |> Expect.toBeEqualToInt(0);
            observedValue2^ |> Expect.toBeEqualToInt(0);

            value |> RxValue.update(old => old + 1);
            observedValue1^ |> Expect.toBeEqualToInt(1);
            observedValue2^ |> Expect.toBeEqualToInt(1);

            subscription1 |> RxDisposable.dispose;
            value |> RxValue.update(old => old + 1);
            observedValue1^ |> Expect.toBeEqualToInt(1);
            observedValue2^ |> Expect.toBeEqualToInt(2);

            subscription2 |> RxDisposable.dispose;
            value |> RxValue.update(old => old + 1);
            observedValue1^ |> Expect.toBeEqualToInt(1);
            observedValue2^ |> Expect.toBeEqualToInt(2);
          }),
        ],
      ),
    ],
  );