[@bs.send]
external promiseThen :
  (Js.Promise.t('a), 'a => unit, Js.Promise.error => unit) =>
  Js.Promise.t('a) =
  "then";

let toObservable = {
  let onSuccessIfNotDisposed = (subscriber, result) =>
    if (! RxSubscriber.isDisposed(subscriber)) {
      subscriber |> RxSubscriber.next(result);
      subscriber |> RxSubscriber.complete;
    };

  let onErrorIfNotDisposed = (subscriber, err) =>
    if (! RxSubscriber.isDisposed(subscriber)) {
      subscriber |> RxSubscriber.complete(~exn=BsRxPromiseException.Exn(err));
    };

  let toObservableSubscribe = (promise, subscriber) =>
    promise
    |. promiseThen(
         onSuccessIfNotDisposed(subscriber),
         onErrorIfNotDisposed(subscriber),
       )
    |> ignore;

  promise => RxObservable.create1(toObservableSubscribe, promise);
};

let fromObservable = {
  let onNext = (last, _, _, _, next) => RxMutableOption.set(next, last);

  let onComplete = (last, resolve, reject, _, exn) =>
    switch (exn) {
    | Some(exn) => reject(. exn)
    | None when ! RxMutableOption.isEmpty(last) =>
      let lastValue = RxMutableOption.get(last);
      resolve(. lastValue);
    | _ => ()
    };

  let operator = (last, resolve, reject) =>
    RxSubscriber.decorate3(~onNext, ~onComplete, last, resolve, reject);

  let promiseCallback = (observable, ~resolve, ~reject) => {
    let last = RxMutableOption.create();

    observable
    |> RxObservable.lift(operator(last, resolve, reject))
    |> RxObservable.connect
    |> ignore;
  };

  observable => Js.Promise.make(promiseCallback(observable));
};