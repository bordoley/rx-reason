open ReUnit.Test;
open ReUnitHelpers;

let test =
  describe(
    "MulticastObservable",
    [
      describe("share", []),
      describe(
        "shareWithReplayBuffer",
        [
          observableIt(
            "replays event on subscribe",
            ~nextToString=string_of_int,
            ~source=
              scheduler => {
                let source =
                  Observables.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(1)),
                      (2.0, Next(2)),
                      (4.0, Next(3)),
                      (6.0, Next(4)),
                      (9.0, Complete),
                    ],
                  )
                  |> Observables.shareWithReplayBuffer(2)

                Observables.merge([
                  source,
                  source |> Observables.subscribeOn(~delay=5.0, scheduler),
                ]);
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(2),
              Next(3),
              Next(4),
              Next(4),
              Complete,
            ],
            (),
          ),
        ],
      ),
    ],
  );