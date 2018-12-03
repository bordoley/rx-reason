open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxScheduler",
    [
      describe(
        "Result",
        [
          describe(
            "continueAfter",
            [
              it("raises with delay less than 0.0", () =>
                Expect.shouldRaise(() =>
                  RxScheduler.Result.continueAfter(
                    ~delay=-1.0, (~now as _, ~shouldYield as _) =>
                    RxScheduler.Result.complete
                  )
                  |> ignore
                )
              ),
            ],
          ),
        ],
      ),
    ],
  );