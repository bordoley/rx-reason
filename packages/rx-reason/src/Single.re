type t('a) = Observable.t('a);

let asObservable = (single: t('a)) : Observable.t('a) => single;

let create = {
  let source = (subscribe, subscriber) =>
    subscribe(
      ~onSuccess=
        result => {
          subscriber |> Subscriber.next(result);
          subscriber |> Subscriber.complete;
        },
      ~onError=exn => subscriber |> Subscriber.complete(~exn),
      ~cancellationToken=subscriber |> Subscriber.asCompositeDisposable,
    );

  subscribe => Observable.create1(source, subscribe);
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

let subscribeWith = {
  let onNext = (subscription, onSuccess, _, next) => {
    onSuccess(next);
    subscription^ |> CompositeDisposable.dispose;
  };

  let onComplete = (subscription, _, onError, exn) => {
    switch (exn) {
    | Some(exn) => onError(exn)
    | None => onError(EmptyException.Exn)
    };
    subscription^ |> CompositeDisposable.dispose;
  };

  (~onSuccess, ~onError, single) => {
    let subscription = ref(CompositeDisposable.disposed);

    subscription :=
      single
      |> Observable.subscribeWith3(
           ~onNext,
           ~onComplete,
           subscription,
           onSuccess,
           onError,
         );
    subscription^;
  };
};

let subscribeWith1 = {
  let onNext = (ctx0, subscription, onSuccess, _, next) => {
    onSuccess(ctx0, next);
    subscription^ |> CompositeDisposable.dispose;
  };

  let onComplete = (ctx0, subscription, _, onError, exn) => {
    switch (exn) {
    | Some(exn) => onError(ctx0, exn)
    | None => onError(ctx0, EmptyException.Exn)
    };
    subscription^ |> CompositeDisposable.dispose;
  };

  (~onSuccess, ~onError, ctx0, single) => {
    let subscription = ref(CompositeDisposable.disposed);

    subscription :=
      single
      |> Observable.subscribeWith4(
           ~onNext,
           ~onComplete,
           ctx0,
           subscription,
           onSuccess,
           onError,
         );
    subscription^;
  };
};

let subscribeWith2 = {
  let onNext = (ctx0, ctx1, subscription, onSuccess, _, next) => {
    onSuccess(ctx0, ctx1, next);
    subscription^ |> CompositeDisposable.dispose;
  };

  let onComplete = (ctx0, ctx1, subscription, _, onError, exn) => {
    switch (exn) {
    | Some(exn) => onError(ctx0, ctx1, exn)
    | None => onError(ctx0, ctx1, EmptyException.Exn)
    };
    subscription^ |> CompositeDisposable.dispose;
  };

  (~onSuccess, ~onError, ctx0, ctx1, single) => {
    let subscription = ref(CompositeDisposable.disposed);

    subscription :=
      single
      |> Observable.subscribeWith5(
           ~onNext,
           ~onComplete,
           ctx0,
           ctx1,
           subscription,
           onSuccess,
           onError,
         );
    subscription^;
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