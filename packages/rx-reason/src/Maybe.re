open Functions.Operators;

type t('a) = Observable.t('a);

let subscribe = Observable.subscribe;

let subscribeObserver = Observable.subscribeObserver;

let subscribeWithCallbacks = Observable.subscribeWithCallbacks;

let defer = Observable.defer;

let empty = Observable.empty;

let ofValue = Observable.ofValue;

let first = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.maybeFirst);

let last = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.maybeLast);

let liftFirst = (operator: Operator.t('a, 'b), observable: Observable.t('a)) : t('b) =>
  observable |> Observable.lift(Operators.maybeFirst << operator);

let liftLast = (operator: Operator.t('a, 'b), observable: Observable.t('a)) : t('b) =>
  observable |> Observable.lift(Operators.maybeLast << operator);

let create = subscribe => Observable.create(subscribe) |> first;

let every = (predicate: 'a => bool, observable: Observable.t('a)) : t(bool) =>
  observable |> liftFirst(Operators.every(predicate));

let find = (predicate: 'a => bool, observable: Observable.t('a)) : t('a) =>
  observable |> liftFirst(Operators.find(predicate));

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

let toObservable = (maybe: t('a)) : Observable.t('a) => maybe;

let toSingle = (maybe: t('a)) : Single.t(option('a)) =>
  maybe |> Observable.lift(Operators.firstOrNone) |> Single.first;