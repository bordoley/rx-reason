open ReUnit;
open ReUnit.Test;
open ReUnitHelpers;

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
                Observable.create(subscriber => {
                  subscriber |> Subscriber.next(10);
                  subscriber |> Subscriber.next(20);
                  subscriber |> Subscriber.complete;
                }),
            ~expected=[Next(10), Next(20), RxReason.Notification.complete(None)],
            (),
          ),
          observableIt(
            "onSubscribe throws synchronously before completing",
            ~nextToString=string_of_int,
            ~source=_ => Observable.create(_ => raise(Division_by_zero)),
            ~expected=[RxReason.Notification.complete(Some(Division_by_zero))],
            (),
          ),
          it(
            "throws when onSubscribe complete synchronously and then throws an exception",
            () => {
            let observable =
              Observable.create(subscriber => {
                subscriber |> Subscriber.complete;
                raise(Division_by_zero);
              });

            (() => observable |> Observable.subscribe) |> Expect.shouldRaise;
          }),
        ],
      ),
      describe("never", []),
    ],
  );