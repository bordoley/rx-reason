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

let lift = (operator: Operator.t('a, 'b), single: t('a)) : t('b) =>
  single |> Observable.lift(operator << Operators.first);

let toObservable = (single: t('a)) : Observable.t('a) => single;