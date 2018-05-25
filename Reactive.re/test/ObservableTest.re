open ReUnit;

open ReUnit.Test;

let test =
  describe(
    "Observable",
    [
      describe(
        "create",
        [
          it("", () => {
            let observable =
              Observable.create((~next, ~complete) => {
                next(5);
                next(10);
                complete(None);
                Disposable.disposed;
              });
            let subscription =
              observable |> Observable.subscribe(~onNext=next => Js.log(next));
            ();
          }),
        ],
      ),
    ],
  );