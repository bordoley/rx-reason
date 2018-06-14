type t('a) = Observable.t('a);

let create = subscribe =>
  Observable.create((~onNext, ~onComplete) =>
    subscribe(
      ~onSuccess=
        result => {
          onNext(result);
          onComplete(None);
        },
      ~onError=exn => onComplete(Some(exn)),
    )
  );

let defer = Observable.defer;

let ofValue = Observable.ofValue;

let first = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.first);

let flatMap = (mapper: 'a => t('b), single: t('a)) : t('b) =>
  single |> Observable.lift(Operators.(map(mapper) >> exhaust));

let last = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.last);

let liftFirst =
    (operator: Operator.t('a, 'b), observable: Observable.t('a))
    : t('b) =>
  observable |> Observable.lift(Operators.(operator >> first));

let liftLast =
    (operator: Operator.t('a, 'b), observable: Observable.t('a))
    : t('b) =>
  observable |> Observable.lift(Operators.(operator >> last));

let every = (predicate: 'a => bool, observable: Observable.t('a)) : t(bool) =>
  observable |> liftFirst(Operators.every(predicate));

let find = (predicate: 'a => bool, observable: Observable.t('a)) : t('a) =>
  observable |> liftFirst(Operators.find(predicate));

let map = (mapper, single) =>
  single |> Observable.lift(Operators.map(mapper));

let none = (predicate: 'a => bool, observable: Observable.t('a)) : t(bool) =>
  observable |> liftFirst(Operators.none(predicate));

let raise = Observable.raise;

let reduce =
    (
      reducer: ('acc, 'a) => 'acc,
      initialValue: 'acc,
      observable: Observable.t('a),
    )
    : t('acc) =>
  observable |> liftLast(Operators.scan(reducer, initialValue));

let some = (predicate: 'a => bool, observable: Observable.t('a)) : t(bool) =>
  observable |> liftFirst(Operators.some(predicate));

let subscribeWithCallbacks =
    (~onSuccess: 'a => unit, ~onError: exn => unit, single)
    : Disposable.t => {
  let innerSubscription = ref(Disposable.disposed);
  let subscription =
    Disposable.create(() =>
      Interlocked.exchange(Disposable.disposed, innerSubscription)
      |> Disposable.dispose
    );
  innerSubscription :=
    single
    |> Observable.subscribeWithCallbacks(
         ~onNext=
           next => {
             onSuccess(next);
             subscription |> Disposable.dispose;
           },
         ~onComplete=
           exn =>
             switch (exn) {
             | Some(exn) => onError(exn)
             | None =>
               /* This case should never happen due to how the constructors of Single
                * instances  protect against it.
                */
               onError(Operators.EmptyException)
             },
       );
  subscription;
};

let subscribe = (~onSuccess=Functions.alwaysUnit, ~onError=Functions.alwaysUnit, single) =>
  subscribeWithCallbacks(~onSuccess, ~onError, single);

let toObservable = (single: t('a)) : Observable.t('a) => single;