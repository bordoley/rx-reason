open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "onNext",
    [
      observableIt(
        "passes through notifications",
        ~nextToString=string_of_int,
        ~source=
          _ =>
            RxObservables.ofList([1, 2])
            |> RxObservables.onNext(RxFunctions.alwaysUnit1),
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.complete(None),
        ],
        (),
      ),
      it("calls the side effect function", () => {
        let sideEffectCount = ref(0);

        RxObservables.ofList([1])
        |> RxObservables.onNext(_ => incr(sideEffectCount))
        |> RxObservable.connect
        |> ignore;

        sideEffectCount^ |> Expect.toBeEqualToInt(1);
      }),
    ],
  );