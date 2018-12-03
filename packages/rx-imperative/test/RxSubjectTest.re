open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "RxSubject",
    [
      describe(
        "create2",
        [
          describe(
            "onNext",
            [
              it("calls onNext delegate", () => {
                let onNextCount = ref(0);

                let subject =
                  RxSubject.create2(
                    ~onNext=(_, onNextCount, _) => incr(onNextCount),
                    ~onComplete=(_, _, _) => (),
                    ~onSubscribe=(_, _, _) => (),
                    ~onDispose=(_, _) => (),
                    (),
                    onNextCount,
                  );

                subject |> RxSubject.next();
                subject |> RxSubject.next();
                subject |> RxSubject.next();

                onNextCount^ |> Expect.toBeEqualToInt(3);
              }),
            ],
          ),
          describe(
            "onComplete",
            [
              it("calls onComplete delegate", () => {
                let onCompleteCount = ref(0);

                let subject =
                  RxSubject.create2(
                    ~onNext=(_, _, _) => (),
                    ~onComplete=
                      (onCompleteCount, _, _) => incr(onCompleteCount),
                    ~onSubscribe=(_, _, _) => (),
                    ~onDispose=(_, _) => (),
                    onCompleteCount,
                    (),
                  );

                subject |> RxSubject.complete;
                subject |> RxSubject.complete;
                subject |> RxSubject.complete;

                onCompleteCount^ |> Expect.toBeEqualToInt(1);
              }),
            ],
          ),
          describe(
            "asObservable",
            [
              it("subscribe calls onSubscribe delegate", () => {
                let onSubscribeCount = ref(0);

                let subject =
                  RxSubject.create2(
                    ~onNext=(_, _, _) => (),
                    ~onComplete=(_, _, _) => (),
                    ~onSubscribe=
                      (onSubscribeCount, _, _) => incr(onSubscribeCount),
                    ~onDispose=(_, _) => (),
                    onSubscribeCount,
                    (),
                  );

                subject
                |> RxSubject.asObservable
                |> RxObservable.subscribe
                |> ignore;
                subject
                |> RxSubject.asObservable
                |> RxObservable.subscribe
                |> ignore;

                onSubscribeCount^ |> Expect.toBeEqualToInt(2);
              }),
              it(
                "subscribe does not call onSubscribe delegate if disposed", () => {
                let onSubscribeCount = ref(0);

                let subject =
                  RxSubject.create2(
                    ~onNext=(_, _, _) => (),
                    ~onComplete=(_, _, _) => (),
                    ~onSubscribe=
                      (onSubscribeCount, _, _) => incr(onSubscribeCount),
                    ~onDispose=(_, _) => (),
                    onSubscribeCount,
                    (),
                  );

                subject
                |> RxSubject.asObservable
                |> RxObservable.subscribe
                |> ignore;
                subject |> RxSubject.dispose;
                subject
                |> RxSubject.asObservable
                |> RxObservable.subscribe
                |> ignore;

                onSubscribeCount^ |> Expect.toBeEqualToInt(1);
              }),
            ],
          ),
        ],
      ),
    ],
  );