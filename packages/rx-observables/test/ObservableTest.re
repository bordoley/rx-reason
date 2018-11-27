open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "Observable",
    [
      describe(
        "create",
        [
          observableIt(
            "onNext and onComplete function calls delegate to the subscriber",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                RxObservable.create(subscriber => {
                  subscriber |> RxSubscriber.next(10);
                  subscriber |> RxSubscriber.next(20);
                  subscriber |> RxSubscriber.complete;
                }),
            ~expected=[Next(10), Next(20), RxNotification.complete(None)],
            (),
          ),
          observableIt(
            "onSubscribe throws synchronously before completing",
            ~nextToString=string_of_int,
            ~source=_ => RxObservable.create(_ => raise(Division_by_zero)),
            ~expected=[RxNotification.complete(Some(Division_by_zero))],
            (),
          ),
          it(
            "throws when onSubscribe complete synchronously and then throws an exception",
            () => {
            let observable =
              RxObservable.create(subscriber => {
                subscriber |> RxSubscriber.complete;
                raise(Division_by_zero);
              });

            (() => observable |> RxObservable.subscribe) |> Expect.shouldRaise;
          }),
        ],
      ),
      describe("never", []),
    ],
  );