open ReUnit.Test;
open ReUnitHelpers;

let test =
  describe("MulticastObservable", [
    describe("share", [
    ]),
    describe("shareWithReplayBuffer", [
      observableIt("replays event on subscribe", 
        ~nextToString=string_of_int,
        ~source=(scheduler) => {
          let source =
            Observable.ofAbsoluteTimeNotifications(~scheduler, [
                (0.0, Next(1)), 
                (2.0, Next(2)), 
                (4.0, Next(3)),
                (10.0, Complete),
              ])
            |> MulticastObservable.shareWithReplayBuffer(1)
            |> MulticastObservable.asObservable;

          source |> Observable.subscribe |> ignore;

          Observable.merge([
            source 
            |> Observable.subscribeOn(~delay=6.0, scheduler),
            source 
            |> Observable.subscribeOn(~delay=3.0, scheduler)
          ]);
        },
        ~expected=[Next(1), Next(2), Next(3), Next(3), Complete],
        (),
      ),
    ]),
  ]);