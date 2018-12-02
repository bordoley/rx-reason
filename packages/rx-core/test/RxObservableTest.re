open ReUnit;
open ReUnit.Test;

let testSourceThrows = observable =>
  it("notifies complete with synchronous exceptions", () => {
    let exceptionCaught = ref(false);

    let operator =
      RxSubscriber.decorate(
        ~onNext=(_, _) => (),
        ~onComplete=
          _ =>
            fun
            | Some(_) => {
                exceptionCaught := true;
                ();
              }
            | _ => (),
      );

    observable
    |> RxObservable.lift(operator)
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
        "never",
        [
          it("subscribe returns a disposed disposable", () =>
            RxObservable.never
            |> RxObservable.subscribe
            |> RxDisposable.isDisposed
            |> Expect.toBeEqualToTrue
          ),
        ],
      ),
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