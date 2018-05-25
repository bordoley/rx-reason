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
              Observable.create((~onNext, ~onComplete) => {
                onNext(5);
                onNext(10);
                onComplete(None);
                Disposable.disposed;
              });
            let subscription =
              observable |> Observable.subscribe(
                ~onNext=next => Js.log(next),
                ~onComplete=Functions.alwaysUnit,
              );
            ();
          }),
        ],
      ),
    ],
  );