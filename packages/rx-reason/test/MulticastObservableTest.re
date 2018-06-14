open ReUnit.Test;
open ReUnitHelpers;

let test =
  describe("MulticastObservable", [
    describe("share", [
    ]),
    describe("shareWithReplayBuffer", [
      observableIt("replays event on subscribe", 
        ~nextToString=string_of_int,
        ~source=({scheduleWithDelay} as scheduler) => {
          let source =
            Observable.ofAbsoluteTimeNotifications(~scheduler, [
                (Next(1), 0.0), 
                (Next(2), 2.0), 
                (Next(3), 4.0),
              ])
            |> MulticastObservable.shareWithReplayBuffer(1)
            |> MulticastObservable.toObservable;

          source |> Observable.subscribe |> ignore;

          source 
          |> Observable.lift(Operators.first) 
          |> Observable.subscribeOn(scheduleWithDelay(~delay=6.0));
        },
        ~expected=[Next(3), Complete],
        (),
      ),
    ]),
  ]);