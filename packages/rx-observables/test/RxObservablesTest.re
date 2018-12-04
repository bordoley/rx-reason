open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "RxObservables",
    [
      ConcatListTest.test,
      DebounceTest.test,
      DefaultIfEmptyTest.test,
      DeferTest.test,
      DistinctUntilChangedTest.test,
      describe("empty", []),
      EveryTest.test,
      ExhaustTest.test,
      FindTest.test,
      FirstTest.test,
      FirstOrNoneTest.test,
      describe(
        "ignoreElements",
        [
          observableIt(
            "ignores all elements and publishes an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  RxNotification.next(1),
                  RxNotification.next(2),
                  RxNotification.next(3),
                  RxNotification.complete(Some(Division_by_zero)),
                ])
                |> RxObservables.ignoreElements,
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
        ],
      ),
      describe(
        "isEmpty",
        [
          observableIt(
            "return false if not empty",
            ~nextToString=string_of_bool,
            ~source=_ => RxObservables.ofValue(1) |> RxObservables.isEmpty,
            ~expected=[
              RxNotification.next(false),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "return true if empty",
            ~nextToString=string_of_bool,
            ~source=_ => RxObservables.empty() |> RxObservables.isEmpty,
            ~expected=[
              RxNotification.next(true),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "keep",
        [
          observableIt(
            "completes the subscriber when the predicate throws an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofValue(1)
                |> RxObservables.keep(_ => raise(Division_by_zero)),
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes the subscriber when the keep subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ => RxObservables.ofValue(1) |> RxObservables.keep(_ => true),
            ~expected=[
              RxNotification.next(1),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "last",
        [
          observableIt(
            "publishes the last observed value and disposes",
            ~nextToString=string_of_int,
            ~source=
              _ => RxObservables.ofList([1, 2, 3]) |> RxObservables.last,
            ~expected=[
              RxNotification.next(3),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  RxNotification.next(1),
                  RxNotification.next(2),
                  RxNotification.next(3),
                  RxNotification.complete(Some(Division_by_zero)),
                ])
                |> RxObservables.last,
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=_ => RxObservables.empty() |> RxObservables.last,
            ~expected=[RxNotification.complete(Some(RxEmptyException.Exn))],
            (),
          ),
        ],
      ),
      describe(
        "lastOrNone",
        [
          observableIt(
            "publishes the Some of the last observed value and completes",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ => RxObservables.ofList([2, 3]) |> RxObservables.lastOrNone,
            ~expected=[
              RxNotification.next(Some(3)),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  RxNotification.next(1),
                  RxNotification.next(2),
                  RxNotification.complete(Some(Division_by_zero)),
                ])
                |> RxObservables.lastOrNone,
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.first
                |> RxObservables.lastOrNone,
            ~expected=[
              RxNotification.next(None),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "map",
        [
          observableIt(
            "completes the subscriber when the mapper throws an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservables.map(_ => raise(Division_by_zero)),
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "completes the subscriber when the mapping subscriber is completed",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3])
                |> RxObservables.map(i => i + 1),
            ~expected=[
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.next(4),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "mapTo",
        [
          observableIt(
            "maps any input to value",
            ~nextToString=RxFunctions.identity,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3]) |> RxObservables.mapTo("a"),
            ~expected=[
              RxNotification.next("a"),
              RxNotification.next("a"),
              RxNotification.next("a"),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "maybeFirst",
        [
          observableIt(
            "publishes the first observed value",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2, 3]) |> RxObservables.maybeFirst,
            ~expected=[
              RxNotification.next(1),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.raise(Division_by_zero)
                |> RxObservables.maybeFirst,
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.first
                |> RxObservables.maybeFirst,
            ~expected=[RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "maybeLast",
        [
          observableIt(
            "publishes the last observed value",
            ~nextToString=string_of_int,
            ~source=
              _ => RxObservables.ofList([1, 2, 3]) |> RxObservables.maybeLast,
            ~expected=[
              RxNotification.next(3),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofNotifications([
                  RxNotification.next(1),
                  RxNotification.next(2),
                  RxNotification.complete(Some(Division_by_zero)),
                ])
                |> RxObservables.maybeLast,
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.empty()
                |> RxObservables.first
                |> RxObservables.maybeLast,
            ~expected=[RxNotification.complete(None)],
            (),
          ),
        ],
      ),
      describe(
        "mergeList",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.mergeList([
                  ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, RxNotification.next(7)),
                      (3.0, RxNotification.next(9)),
                      (4.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([1, 2, 3]),
                  ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (2.0, RxNotification.next(8)),
                      (4.0, RxNotification.next(10)),
                      (5.0, RxNotification.complete(None)),
                    ],
                  ),
                  RxObservables.ofList([4, 5, 6]),
                ]),
            ~expected=[
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.next(4),
              RxNotification.next(5),
              RxNotification.next(6),
              RxNotification.next(7),
              RxNotification.next(8),
              RxNotification.next(9),
              RxNotification.next(10),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe("none", []),
      describe("observeOn", []),
      describe("ofList", []),
      describe("ofValue", []),
      describe(
        "onComplete",
        [
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2])
                |> RxObservables.onComplete(RxFunctions.alwaysUnit1),
            ~expected=[
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.complete(None),
            ],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            RxObservables.ofList([1])
            |> RxObservables.onComplete(_ => incr(sideEffectCount))
            |> RxObservable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe(
        "onNext",
        [
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([1, 2])
                |> RxObservables.onNext(RxFunctions.alwaysUnit1),
            ~expected=[
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.complete(None),
            ],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            RxObservables.ofList([1])
            |> RxObservables.onNext(_ => incr(sideEffectCount))
            |> RxObservable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe("raise", []),
      RetryTest.test,
      describe(
        "scan",
        [
          observableIt(
            "publishes all intermediate values, including the initial accumulator value",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.ofList([2, 3, 4])
                |> RxObservables.scan((acc, next) => acc + next, 0),
            ~expected=[
              RxNotification.next(0),
              RxNotification.next(2),
              RxNotification.next(5),
              RxNotification.next(9),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
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
                  ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, RxNotification.next(1)),
                      (2.0, RxNotification.next(2)),
                      (4.0, RxNotification.next(3)),
                      (6.0, RxNotification.next(4)),
                      (9.0, RxNotification.complete(None)),
                    ],
                  )
                  |> RxObservables.shareWithReplayBuffer(2);

                RxObservables.mergeList([
                  source,
                  source |> RxObservables.subscribeOn(~delay=5.0, scheduler),
                ]);
              },
            ~expected=[
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.next(4),
              RxNotification.next(4),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "some",
        [
          observableIt(
            "returns false for an subscriber that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=
              _ => RxObservables.empty() |> RxObservables.some(i => i > 10),
            ~expected=[
              RxNotification.next(false),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "returns false for an subscriber for which no value passes the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                RxObservables.ofList([5, 6, 7])
                |> RxObservables.some(i => i > 10),
            ~expected=[
              RxNotification.next(false),
              RxNotification.complete(None),
            ],
            (),
          ),
          observableIt(
            "completes with true on the first observed value that passed the predicate",
            ~nextToString=string_of_int,
            ~source=
              ({now} as scheduler) =>
                ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, RxNotification.next(8)),
                    (2.0, RxNotification.next(11)),
                    (3.0, RxNotification.next(14)),
                    (4.0, RxNotification.next(6)),
                    (5.0, RxNotification.next(5)),
                    (6.0, RxNotification.complete(None)),
                  ],
                )
                |> RxObservables.some(i => i > 10)
                |> RxObservables.map(_ => now() |> int_of_float),
            ~expected=[
              RxNotification.next(2),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "startWithlist",
        [
          observableIt(
            "prepends the values",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                RxObservables.startWithList(
                  [1, 2, 3],
                  ofRelativeTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, RxNotification.next(4)),
                      (2.0, RxNotification.next(5)),
                      (3.0, RxNotification.complete(None)),
                    ],
                  ),
                ),
            ~expected=[
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.next(4),
              RxNotification.next(5),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      describe(
        "startWithValue",
        [
          observableIt(
            "prepends the values",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservables.startWithValue(
                  1,
                  RxObservables.ofList([2, 3]),
                ),
            ~expected=[
              RxNotification.next(1),
              RxNotification.next(2),
              RxNotification.next(3),
              RxNotification.complete(None),
            ],
            (),
          ),
        ],
      ),
      SwitchTest.test,
      describe("subscribeOn", []),
      TakeUntilTest.test,
      TimeoutTest.test,
      WithLatestFromTest.test,
    ],
  );