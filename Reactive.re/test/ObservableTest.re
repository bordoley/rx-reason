open ReUnit;

open ReUnit.Test;

let test =
  describe(
    "Observable",
    [
      describe(
        "createWithObserver",
        [
          it("the observer instance passed to the onSubscribe function is returned as the subscription", () => {
            let observerInstance = MutableOption.empty();
            let observable =
              Observable.createWithObserver(observer => {
                MutableOption.set(observer, observerInstance);
                Disposable.disposed;
              });
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=Functions.alwaysUnit,
                 );
            let observer =
              observerInstance
              |> MutableOption.firstOrRaise
              |> Observer.toDisposable;
            subscription === observer |> Expect.toBeEqualToTrue;
          }),
          it("disposing the subscription disposes the returned observable", () => {
            let returnedDisposable = Disposable.empty();
            let observable =
              Observable.createWithObserver(_ => {
                returnedDisposable;
              });
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=Functions.alwaysUnit,
                 );
            
            Disposable.dispose(subscription);
            returnedDisposable |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it("onSubscribe raising an exception is caught and completes the observer", () => {
            let observable =
              Observable.createWithObserver(_ => {
                raise(Division_by_zero);
                Disposable.disposed;
              });

            let completed = ref(None);
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=exn=>completed := exn,
                 );
            switch(completed^) {
            | Some(Division_by_zero) => ()
            | _ => failwith("expected exception to be thrown")
            }
          }),
          it("subscribe rethrows exceptions from onSubscribe if the observer is already completed", () => {
            let observable =
              Observable.createWithObserver(observer => {
                observer |> Observer.complete(~exn=None);
                raise(Division_by_zero);
                Disposable.disposed;
              });
            Expect.shouldRaise(() => {
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=Functions.alwaysUnit,
                 );
            });
          }),
        ],
      ),
    ],
  );