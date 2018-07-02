type t('a) = Observable.t('a);

let asObservable = (single: t('a)) : Observable.t('a) => single;

let create = {
  let source = (subscribe, subscriber) => {
    let singleSubscriber = SingleSubscriber.decorateSubscriber(subscriber);
    subscribe(singleSubscriber);
  };

  subscribe => Observable.create1(source, subscribe);
};

let create1 = {
  let source = (subscribe, ctx0, subscriber) => {
    let singleSubscriber = SingleSubscriber.decorateSubscriber(subscriber);
    subscribe(ctx0, singleSubscriber);
  };

  (subscribe, ctx) => Observable.create2(source, subscribe, ctx);
};

let defer = Observable.defer;

let every = Observable.every;

let find = (predicate: 'a => bool, observable: Observable.t('a)) : t('a) =>
  observable |> Observable.find(predicate) |> Observable.first;

let first = Observable.first;

let flatMap = (mapper: 'a => t('b), single: t('a)) : t('b) =>
  single |> Observable.map(mapper) |> Observable.exhaust;

let last = Observable.last;

let liftFirst =
    (operator: Operator.t('a, 'b), observable: Observable.t('a))
    : t('b) =>
  observable |> Observable.lift(operator) |> Observable.first;

let liftLast =
    (operator: Operator.t('a, 'b), observable: Observable.t('a))
    : t('b) =>
  observable |> Observable.lift(operator) |> Observable.last;

let map = Observable.map;
let mapTo = Observable.mapTo;
let none = Observable.none;
let ofValue = Observable.ofValue;
let raise = Observable.raise;

let reduce =
    (
      reducer: ('acc, 'a) => 'acc,
      initialValue: 'acc,
      observable: Observable.t('a),
    )
    : t('acc) =>
  observable |> Observable.scan(reducer, initialValue);

let some = Observable.some;

let subscribeSubscriber = (subscriber, single) => {
  let subscriber = subscriber |> SingleSubscriber.asSubscriber;
  single |> Observable.subscribeSubscriber(subscriber);
};

let subscribeWith = {
  let onNext = (subscriber, onSuccess, _, next) => {
    onSuccess(next);
    subscriber^ |> Subscriber.dispose;
  };

  let onComplete = (subscriber, _, onError, exn) => {
    switch (exn) {
    | Some(exn) => onError(exn)
    | None => onError(EmptyException.Exn)
    };
    subscriber^ |> Subscriber.dispose;
  };

  (~onSuccess, ~onError, single) => {
    let subscriber = ref(Subscriber.disposed);

    subscriber :=
      Subscriber.createAutoDisposing3(
        ~onNext,
        ~onComplete,
        subscriber,
        onSuccess,
        onError,
      );

    single |> Observable.subscribeSubscriber(subscriber^);
    subscriber^ |> Subscriber.asDisposable;
  };
};

let subscribeWith1 = {
  let onNext = (ctx0, subscriber, onSuccess, _, next) => {
    onSuccess(ctx0, next);
    subscriber^ |> Subscriber.dispose;
  };

  let onComplete = (ctx0, subscriber, _, onError, exn) => {
    switch (exn) {
    | Some(exn) => onError(ctx0, exn)
    | None => onError(ctx0, EmptyException.Exn)
    };
    subscriber^ |> Subscriber.dispose;
  };

  (~onSuccess, ~onError, ctx0, single) => {
    let subscriber = ref(Subscriber.disposed);

    subscriber :=
      Subscriber.createAutoDisposing4(
        ~onNext,
        ~onComplete,
        ctx0,
        subscriber,
        onSuccess,
        onError,
      );

    single |> Observable.subscribeSubscriber(subscriber^);
    subscriber^ |> Subscriber.asDisposable;
  };
};

let subscribeWith2 = {
  let onNext = (ctx0, ctx1, subscriber, onSuccess, _, next) => {
    onSuccess(ctx0, ctx1, next);
    subscriber^ |> Subscriber.dispose;
  };

  let onComplete = (ctx0, ctx1, subscriber, _, onError, exn) => {
    switch (exn) {
    | Some(exn) => onError(ctx0, ctx1, exn)
    | None => onError(ctx0, ctx1, EmptyException.Exn)
    };
    subscriber^ |> Subscriber.dispose;
  };

  (~onSuccess, ~onError, ctx0, ctx1, single) => {
    let subscriber = ref(Subscriber.disposed);

    subscriber :=
      Subscriber.createAutoDisposing5(
        ~onNext,
        ~onComplete,
        ctx0,
        ctx1,
        subscriber,
        onSuccess,
        onError,
      );

    single |> Observable.subscribeSubscriber(subscriber^);
    subscriber^ |> Subscriber.asDisposable;
  };
};

let subscribe =
    (~onSuccess=Functions.alwaysUnit1, ~onError=Functions.alwaysUnit1, single) =>
  subscribeWith(~onSuccess, ~onError, single);

let subscribe1 =
    (
      ~onSuccess=Functions.alwaysUnit2,
      ~onError=Functions.alwaysUnit2,
      ctx0,
      single,
    ) =>
  subscribeWith1(~onSuccess, ~onError, ctx0, single);

let subscribe2 =
    (
      ~onSuccess=Functions.alwaysUnit3,
      ~onError=Functions.alwaysUnit3,
      ctx0,
      ctx1,
      single,
    ) =>
  subscribeWith2(~onSuccess, ~onError, ctx0, ctx1, single);

