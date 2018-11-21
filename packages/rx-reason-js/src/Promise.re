[@bs.send]
external promiseThen :
  (Js.Promise.t('a), 'a => unit, Js.Promise.error => unit) =>
  Js.Promise.t('a) =
  "then";

let toObservable = {
  let toObservableSubscribe = (promise, subscriber) => {
    let onSuccessIfNotDisposed = result =>
      if (! RxReason.Subscriber.isDisposed(subscriber)) {
        subscriber |> RxReason.Subscriber.next(result);
        subscriber |> RxReason.Subscriber.complete;
      };

    let onErrorIfNotDisposed = err =>
      if (! RxReason.Subscriber.isDisposed(subscriber)) {
        subscriber
        |> RxReason.Subscriber.complete(~exn=PromiseException.Exn(err));
      };

    promise
    |. promiseThen(onSuccessIfNotDisposed, onErrorIfNotDisposed)
    |> ignore;
  };

  promise => RxReason.Observable.create1(toObservableSubscribe, promise);
};

let fromObservable = {
  let onNext = (resolve, _, a) => resolve(. a);
  let onComplete = (_, reject, exn) =>
    switch (exn) {
    | Some(exn) => reject(. exn)
    | _ => ()
    };

  observable =>
    Js.Promise.make((~resolve, ~reject) => {
      let subscriber =
        RxReason.Subscriber.createAutoDisposing2(
          ~onNext,
          ~onComplete,
          resolve,
          reject,
        );
      observable
      |> RxReason.Observable.lift(RxReason.Operators.last)
      |> RxReason.Observable.subscribeWith(subscriber);
    });
};