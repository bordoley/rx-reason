type t('a) = Observable.t('a);

let asObservable = (single: t('a)) : Observable.t('a) => single;

let create = subscribe =>
  Observable.create(subscriber => {
    let teardown =
      subscribe(
        ~onSuccess=
          result => {
            subscriber |> Subscriber.next(result);
            subscriber |> Subscriber.complete;
          },
        ~onError=exn => subscriber |> Subscriber.complete(~exn),
      );
    subscriber |> Subscriber.addTeardown(teardown) |> ignore;
  });

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
  let teardown = innerSubscription =>
    Interlocked.exchange(CompositeDisposable.disposed, innerSubscription)
    |> CompositeDisposable.dispose;

  (~onSuccess: 'a => unit, ~onError: exn => unit, single) => {
    let innerSubscription = ref(CompositeDisposable.disposed);
    let subscription = Disposable.create1(teardown, innerSubscription);

    innerSubscription :=
      single
      |> Observable.subscribeWith(
           ~onNext=
             next => {
               onSuccess(next);
               subscription |> Disposable.dispose;
             },
           ~onComplete=
             fun
             | Some(exn) => onError(exn)
             | None => onError(EmptyException.Exn),
         );
    subscription;
  };
};

let subscribe =
    (~onSuccess=Functions.alwaysUnit1, ~onError=Functions.alwaysUnit1, single) =>
  subscribeWith(~onSuccess, ~onError, single);