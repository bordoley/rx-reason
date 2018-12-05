open ReUnit;
open ReUnit.Test;

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

        observable |> RxObservable.connect |> ignore;
        observable |> RxObservable.connect |> ignore;
        observable |> RxObservable.connect |> ignore;
        count^ |> Expect.toBeEqualToInt(3);
      }),
    ],
  );