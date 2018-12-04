open ReUnit;
open ReUnit.Test;
open RxReUnit;

let test =
  describe(
    "retry",
    [
      observableIt(
        "with cold observable",
        ~nextToString=string_of_int,
        ~source=
          _ => {
            let retryCount = ref(0);
            let predicate = _ => {
              let retry = retryCount^ < 2;
              if (retry) {
                incr(retryCount);
              };
              retry;
            };

            RxObservable.create(subscriber => {
              subscriber |> RxSubscriber.next(1);
              subscriber |> RxSubscriber.next(2);
              subscriber |> RxSubscriber.complete(~exn=Division_by_zero);
            })
            |> RxObservables.retry(~predicate);
          },
        ~expected=[
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.next(1),
          RxNotification.next(2),
          RxNotification.complete(Some(Division_by_zero)),
        ],
        (),
      ),
      it("with hot observable", () => {
        let result = ref([]);
        let subject = ref(RxSubject.createMulticast());

        let subscription =
          RxObservable.create(subscriber => {
            subject := RxSubject.createMulticast();
            let observable = subject^ |> RxSubject.asObservable;
            let subscription =
              observable
              |> RxObservables.observe(
                   ~onNext=next => subscriber |> RxSubscriber.next(next),
                   ~onComplete=
                     exn => subscriber |> RxSubscriber.complete(~exn?),
                 )
              |> RxObservable.subscribe;

            subscriber |> RxSubscriber.addDisposable(subscription) |> ignore;
          })
          |> RxObservables.retry
          |> RxObservables.onNext(x => result := [x, ...result^])
          |> RxObservable.subscribe;

        let subscriber = subject^;
        subscriber |> RxSubject.next(1);
        subscriber |> RxSubject.next(2);
        subscriber |> RxSubject.complete(~exn=Division_by_zero);
        subscription |> RxDisposable.isDisposed |> Expect.toBeEqualToFalse;

        let subscriber = subject^;
        subscriber |> RxSubject.next(3);
        subscriber |> RxSubject.next(4);
        subscriber |> RxSubject.complete;
        subscription |> RxDisposable.isDisposed |> Expect.toBeEqualToTrue;

        result^ |> Expect.toBeEqualToListOfInt([4, 3, 2, 1]);
      }),
      it("doesn't retry if unsubscribed in shouldRetry callback", () => {
        let vts = RxVirtualTimeScheduler.create();
        let scheduler = vts |> RxVirtualTimeScheduler.asScheduler;

        let subscription = ref(RxDisposable.disposed);
        let thrownException = ref(None);
        subscription :=
          ofAbsoluteTimeNotifications(
            ~scheduler,
            [
              (1.0, RxNotification.next(5)),
              (2.0, RxNotification.complete(Some(Division_by_zero))),
            ],
          )
          |> RxObservables.retry(~predicate=_ => {
               subscription^ |> RxDisposable.dispose;
               true;
             })
          |> expectObservableToProduce(
               ~nextToString=string_of_int,
               [RxNotification.next(5), RxNotification.complete(None)],
             )
          |> RxObservables.onComplete(exn => thrownException := exn)
          |> RxObservable.subscribe;

        vts |> RxVirtualTimeScheduler.run;
        subscription^ |> RxDisposable.dispose;

        switch (thrownException^) {
        | Some(exn) => raise(exn)
        | _ => ()
        };
      }),
    ],
  );