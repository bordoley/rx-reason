open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "defer",
    [
      it("calls the observable factory on subscribe", () => {
        let count = ref(0);

        let observable =
          RxObservables.defer(() => {
            incr(count);
            RxObservables.empty();
          });

        observable |> RxObservable.subscribe |> ignore;
        observable |> RxObservable.subscribe |> ignore;
        observable |> RxObservable.subscribe |> ignore;
        count^ |> Expect.toBeEqualToInt(3);
      }),
    ],
  );