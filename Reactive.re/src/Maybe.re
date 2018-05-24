open Functions.Operators;

type t('a) = Observable.t('a);

let subscribeObserver = Observable.subscribeObserver;

let subscribe = Observable.subscribe;

let create = (subscribe: Observer.t('a) => Disposable.t) : t('a) =>
  Observable.create(observer => observer |> Operators.maybeFirst |> subscribe);

let defer = Observable.defer;

let empty = Observable.empty;

let ofValue = Observable.ofValue;

let first = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.maybeFirst);

let last = (observable: Observable.t('a)) : t('a) =>
  observable |> Observable.lift(Operators.maybeLast);

let lift = (operator: Operator.t('a, 'b), maybe: t('a)) : t('b) =>
  maybe |> Observable.lift(operator << Operators.maybeFirst);

let reduce =
    (
      reducer: ('acc, 'a) => 'acc,
      initialValue: 'acc,
      observable: Observable.t('a),
    )
    : t('acc) =>
  observable |> lift(Operators.scan(reducer, initialValue));

let toObservable = (maybe: t('a)) : Observable.t('a) => maybe;

let toSingle = (maybe: t('a)) : Single.t(option('a)) =>
  maybe |> Observable.lift(Operators.firstOrNone) |> Single.first;