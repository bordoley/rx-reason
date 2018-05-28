open Functions.Operators;
open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "Observable",
    [
      describe(
        "combineLatest2",
        [
          it("combines latest values from each subject", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let result = ref([0, 0]);

            Observable.combineLatest2(
              ~selector=(a, b) => [a, b],
              subject0 |> Subject.toObservable,
              subject1 |> Subject.toObservable,
            )
            |> Observable.subscribe(
                 ~onNext=next => result := next,
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;

            observer0 |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([0, 0]);
            observer0 |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([0, 0]);
            observer1 |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([2, 2]);
            observer1 |> Observer.next(3);
            result^ |> Expect.toBeEqualToListOfInt([2, 3]);
          }),
          it("completes only when all observables complete", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest2(
                ~selector=(a, b) => [a, b],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
              )
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit, ~onComplete=_ =>
                   result := true
                 );

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;

            observer0 |> Observer.next(1);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer1 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer0 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes only when the first observables completes with an exception",
            () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest2(
                ~selector=(a, b) => [a, b],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
              )
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit, ~onComplete=_ =>
                   result := true
                 );

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;
            let observer0 = subject0 |> Subject.toObserver;

            observer0 |> Observer.complete(Some(Division_by_zero));
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            result^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe("combineLatest3", []),
      describe("combineLatest4", []),
      describe("combineLatest5", []),
      describe("combineLatest6", []),
      describe("combineLatest7", []),
      describe(
        "concat",
        [
          it("mixing async and sync observables", () => {
            let result = ref([]);
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let subscription =
              Observable.concat([
                Observable.ofList([1, 2, 3]),
                subject0 |> Subject.toObservable,
                Observable.ofList([7, 8, 9]),
                subject1 |> Subject.toObservable,
              ])
              |> Observable.subscribe(
                   ~onNext=x => result := [x, ...result^],
                   ~onComplete=Functions.alwaysUnit,
                 );
            result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);

            subject0 |> Subject.toObserver |> Observer.next(4);
            subject0 |> Subject.toObserver |> Observer.next(5);
            subject0 |> Subject.toObserver |> Observer.next(6);
            subject0 |> Subject.toObserver |> Observer.complete(None);
            subject1 |> Subject.toObserver |> Observer.next(10);
            subject1 |> Subject.toObserver |> Observer.next(11);
            subject1 |> Subject.toObserver |> Observer.complete(None);

            result^
            |> Expect.toBeEqualToListOfInt([
                 11,
                 10,
                 9,
                 8,
                 7,
                 6,
                 5,
                 4,
                 3,
                 2,
                 1,
               ]);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "create",
        [
          it(
            "onNext and onComplete function calls delegate to the subscriber",
            () => {
            let observable =
              Observable.create((~onNext, ~onComplete) => {
                onNext(10);
                onNext(20);
                onComplete(None);
                Disposable.disposed;
              });

            let value = ref(0);
            let complete = ref(false);
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=next => value := next,
                   ~onComplete=_ => complete := true,
                 );

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            value^ |> Expect.toBeEqualToInt(20);
            complete^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "createWithObserver",
        [
          it(
            "the observer instance passed to the onSubscribe function is returned as the subscription",
            () => {
              let observerInstance = MutableOption.create();
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
                observerInstance |> MutableOption.get |> Observer.toDisposable;
              subscription === observer |> Expect.toBeEqualToTrue;
            },
          ),
          it("disposing the subscription disposes the returned observable", () => {
            let returnedDisposable = Disposable.empty();
            let observable =
              Observable.createWithObserver(_ => returnedDisposable);
            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=Functions.alwaysUnit,
                 );

            Disposable.dispose(subscription);
            returnedDisposable
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue;
          }),
          it(
            "onSubscribe raising an exception is caught and completes the observer",
            () => {
            let observable =
              Observable.createWithObserver(_ => raise(Division_by_zero));

            let completed = ref(None);
            observable
            |> Observable.subscribe(
                 ~onNext=Functions.alwaysUnit, ~onComplete=exn =>
                 completed := exn
               )
            |> ignore;
            switch (completed^) {
            | Some(Division_by_zero) => ()
            | _ => failwith("expected exception to be thrown")
            };
          }),
          it(
            "subscribe rethrows exceptions from onSubscribe if the observer is already completed",
            () => {
              let observable =
                Observable.createWithObserver(observer => {
                  observer |> Observer.complete(None);
                  raise(Division_by_zero);
                });
              Expect.shouldRaise(() =>
                observable
                |> Observable.subscribe(
                     ~onNext=Functions.alwaysUnit,
                     ~onComplete=Functions.alwaysUnit,
                   )
              );
            },
          ),
        ],
      ),
      describe(
        "defer",
        [
          it("calls the observable factory on subscribe", () => {
            let count = ref(0);

            let observable =
              Observable.defer(() => {
                count := count^ + 1;
                Observable.empty();
              });

            let subscription =
              observable
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit,
                   ~onComplete=Functions.alwaysUnit,
                 );

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;

            observable
            |> Observable.subscribe(
                 ~onNext=Functions.alwaysUnit,
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            observable
            |> Observable.subscribe(
                 ~onNext=Functions.alwaysUnit,
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe("empty", []),
      describe(
        "lift",
        [
          it("applies the operator", () =>
            Observable.ofList([1, 2, 3])
            |> Observable.lift(
                 Operators.scan((acc, next) => acc + next, 0)
                 << Operators.map(string_of_int)
                 << Operators.last,
               )
            |> Observable.subscribe(
                 ~onNext=Expect.toBeEqualToString("6"),
                 ~onComplete=Functions.alwaysUnit,
               )
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue
          ),
        ],
      ),
      describe(
        "merge",
        [
          it("merges host and cold observables", () => {
            let result = ref([]);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            Observable.merge([
              subject1 |> Subject.toObservable,
              Observable.ofList([1, 2, 3]),
              subject2 |> Subject.toObservable,
              Observable.ofList([4, 5, 6]),
            ])
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer1 |> Observer.next(7);
            observer2 |> Observer.next(8);
            observer1 |> Observer.next(9);
            result^
            |> Expect.toBeEqualToListOfInt([9, 8, 7, 6, 5, 4, 3, 2, 1]);
          }),
          it("merges hot observables", () => {
            let result = ref([]);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            Observable.merge([
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
              subject3 |> Subject.toObservable,
            ])
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

            observer1 |> Observer.next(1);
            observer2 |> Observer.next(2);
            observer1 |> Observer.next(3);
            observer2 |> Observer.next(4);
            observer1 |> Observer.complete(None);
            observer2 |> Observer.next(5);
            observer2 |> Observer.complete(None);
            observer3 |> Observer.next(6);
            observer3 |> Observer.complete(None);

            result^ |> Expect.toBeEqualToListOfInt([6, 5, 4, 3, 2, 1]);
          }),
          it("completes when the last observable completes", () => {
            let result = ref(false);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let subscription =
              Observable.merge([
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              ])
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit, ~onComplete=_ =>
                   result := true
                 );

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

            observer1 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer2 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer3 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes early if any observable completes with an exception", () => {
            let result = ref(false);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let subscription =
              Observable.merge([
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              ])
              |> Observable.subscribe(
                   ~onNext=Functions.alwaysUnit, ~onComplete=_ =>
                   result := true
                 );

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer1 |> Observer.complete(None);
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer2 |> Observer.complete(Some(Division_by_zero));
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe("never", []),
      describe("ofList", []),
      describe("ofValue", []),
      describe(
        "retry",
        [
          it("with cold observable", () => {
            let result = ref([]);
            let retryCount = ref(0);
            let shouldRetry = _ => {
              let retry = retryCount^ < 2;
              if (retry) {
                Interlocked.increment(retryCount) |> ignore;
              };
              retry;
            };

            Observable.create((~onNext, ~onComplete) => {
              onNext(1);
              onNext(2);
              onComplete(Some(Division_by_zero));
              Disposable.disposed;
            })
            |> Observable.retry(shouldRetry)
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;
            result^ |> Expect.toBeEqualToListOfInt([2, 1, 2, 1, 2, 1]);
          }),
          it("with hot observable", () => {
            let result = ref([]);
            let subject = ref(Subject.create());

            let subscription =
              Observable.create((~onNext, ~onComplete) => {
                subject := Subject.create();
                let observable = subject^ |> Subject.toObservable;
                observable |> Observable.subscribe(~onNext, ~onComplete);
              })
              |> Observable.retry(_ => true)
              |> Observable.subscribe(
                   ~onNext=x => result := [x, ...result^],
                   ~onComplete=Functions.alwaysUnit,
                 );

            let observer = subject^ |> Subject.toObserver;
            observer |> Observer.next(1);
            observer |> Observer.next(2);
            observer |> Observer.complete(Some(Division_by_zero));
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            let observer = subject^ |> Subject.toObserver;
            observer |> Observer.next(3);
            observer |> Observer.next(4);
            observer |> Observer.complete(None);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;

            result^ |> Expect.toBeEqualToListOfInt([4, 3, 2, 1]);
          }),
        ],
      ),
      describe(
        "startWithlist",
        [
          it("prepends values", () => {
            let result = ref([]);
            let subject = Subject.create();
            let subscription =
              Observable.startWithList(
                [1, 2, 3],
                subject |> Subject.toObservable,
              )
              |> Observable.subscribe(
                   ~onNext=x => result := [x, ...result^],
                   ~onComplete=Functions.alwaysUnit,
                 );
            result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            let observer = subject |> Subject.toObserver;
            observer |> Observer.next(4);
            observer |> Observer.next(5);

            result^ |> Expect.toBeEqualToListOfInt([5, 4, 3, 2, 1]);
          }),
        ],
      ),
      describe(
        "startWithValue",
        [
          it("prepends the value", () => {
            let result = ref([]);
            Observable.startWithValue(1, Observable.ofList([2, 3]))
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
                 ~onComplete=Functions.alwaysUnit,
               )
            |> ignore;
            result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);
          }),
        ],
      ),
      describe("subscribe", []),
      describe("subscribeObserver", []),
      describe("subscribeOn", []),
    ],
  );