open Functions.Operators;

type t('a) = Observable.t('a);

let subscribeObserver = Observable.subscribeObserver;

let subscribe = Observable.subscribe;

let defer = Observable.defer;

let ofValue = Observable.ofValue;

let first = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.first);

let last = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.last);

let liftFirst = (operator: Operator.t('a, 'b), single: t('a)) : t('b) =>
  single |> Observable.lift(Operators.first << operator);

let liftLast = (operator: Operator.t('a, 'b), single: t('a)) : t('b) =>
  single |> Observable.lift(Operators.last << operator);

let create = subscribe => Observable.create(subscribe) |> first;

let reduce =
    (
      reducer: ('acc, 'a) => 'acc,
      initialValue: 'acc,
      observable: Observable.t('a),
    )
    : t('acc) =>
  observable |> liftLast(Operators.scan(reducer, initialValue));

let toObservable = (single: t('a)) : Observable.t('a) => single;