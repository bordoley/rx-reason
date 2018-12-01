open ReUnit;
open ReUnit.Test;

let testSourceThrows = observable =>
  it("notifies complete with synchronous exceptions", () => {
    let exceptionCaught = ref(false);

    observable
    |> RxObservable.observe(
         ~onNext=_ => (),
         ~onComplete=
           fun
           | Some(_) => {
               exceptionCaught := true;
               ();
             }
           | _ => (),
       )
    |> RxObservable.subscribe
    |> ignore;

    exceptionCaught^ |> Expect.toBeEqualToTrue;
  });

let testSourceThrowsAfterCompletingSynchronous = observable =>
  it("rethrows synchronous exceptions if subscriber is completed", () =>
    Expect.shouldRaise(() => observable |> RxObservable.subscribe |> ignore)
  );  

let test =
  describe(
    "RxObservable",
    [
      describe(
        "create",
        [
          describe(
            "subscribe",
            [
              testSourceThrows(
                RxObservable.create(_ => raise(Division_by_zero)),
              ),
              testSourceThrowsAfterCompletingSynchronous(
                RxObservable.create(subscriber => {
                  subscriber |> RxSubscriber.complete;
                  raise(Division_by_zero);
                }),
              ),
            ],
          ),
        ],
      ),
      describe(
        "create1",
        [
          describe(
            "subscribe",
            [
              testSourceThrows(
                RxObservable.create1((_, _) => raise(Division_by_zero), ()),
              ),
              testSourceThrowsAfterCompletingSynchronous(
                RxObservable.create1(
                  (_, subscriber) => {
                    subscriber |> RxSubscriber.complete;
                    raise(Division_by_zero);
                  },
                  (),
                ),
              ),
            ],
          ),
        ],
      ),
      describe(
        "create2",
        [
          describe(
            "subscribe",
            [
              testSourceThrows(
                RxObservable.create2(
                  (_, _, _) => raise(Division_by_zero),
                  (),
                  (),
                ),
              ),
              testSourceThrowsAfterCompletingSynchronous(
                RxObservable.create2(
                  (_, _, subscriber) => {
                    subscriber |> RxSubscriber.complete;
                    raise(Division_by_zero);
                  },
                  (),
                  (),
                ),
              ),
            ],
          ),
        ],
      ),
      describe(
        "create3",
        [
          describe(
            "subscribe",
            [
              testSourceThrows(
                RxObservable.create3(
                  (_, _, _, _) => raise(Division_by_zero),
                  (),
                  (),
                  (),
                ),
              ),
              testSourceThrowsAfterCompletingSynchronous(
                RxObservable.create3(
                  (_, _, _, subscriber) => {
                    subscriber |> RxSubscriber.complete;
                    raise(Division_by_zero);
                  },
                  (),
                  (),
                  (),
                ),
              ),
            ],
          ),
        ],
      ),
      describe(
        "create4",
        [
          describe(
            "subscribe",
            [
              testSourceThrows(
                RxObservable.create4(
                  (_, _, _, _, _) => raise(Division_by_zero),
                  (),
                  (),
                  (),
                  (),
                ),
              ),
              testSourceThrowsAfterCompletingSynchronous(
                RxObservable.create4(
                  (_, _, _, _, subscriber) => {
                    subscriber |> RxSubscriber.complete;
                    raise(Division_by_zero);
                  },
                  (),
                  (),
                  (),
                  (),
                ),
              ),
            ],
          ),
        ],
      ),
      describe(
        "create5",
        [
          describe(
            "subscribe",
            [
              testSourceThrows(
                RxObservable.create5(
                  (_, _, _, _, _, _) => raise(Division_by_zero),
                  (),
                  (),
                  (),
                  (),
                  (),
                ),
              ),
              testSourceThrowsAfterCompletingSynchronous(
                RxObservable.create5(
                  (_, _, _, _, _, subscriber) => {
                    subscriber |> RxSubscriber.complete;
                    raise(Division_by_zero);
                  },
                  (),
                  (),
                  (),
                  (),
                  (),
                ),
              ),
            ],
          ),
        ],
      ),
    ],
  );