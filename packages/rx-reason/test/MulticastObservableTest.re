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
                (0.0, Next(1)), 
                (2.0, Next(2)), 
                (4.0, Next(3)),
                (10.0, Complete),
              ])
            |> MulticastObservable.shareWithReplayBuffer(1)
            |> MulticastObservable.toObservable;

          source |> Observable.subscribe |> ignore;

          Observable.merge([
            source 
            |> Observable.subscribeOn(scheduleWithDelay(~delay=6.0)),
            source 
            |> Observable.subscribeOn(scheduleWithDelay(~delay=3.0))
          ]);
        },
        ~expected=[Next(2), Next(3), Next(3), Complete],
        (),
      ),
    ]),
  ]);