type t('a) = Observable.t('a);

let asObservable = (single: t('a)) : Observable.t('a) => single;

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

let subscribeWith =
    (~onSuccess: 'a => unit, ~onError: exn => unit, single)
    : Disposable.t => {
  let innerSubscription = ref(CompositeDisposable.disposed);
  let subscription =
    Disposable.create(() =>
      Interlocked.exchange(CompositeDisposable.disposed, innerSubscription)
      |> CompositeDisposable.dispose
    );
  innerSubscription :=
    single
    |> Observable.subscribeWith(
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
               onError(EmptyException.Exn)
             },
       );
  subscription;
};

let subscribe =
    (~onSuccess=Functions.alwaysUnit, ~onError=Functions.alwaysUnit, single) =>
  subscribeWith(~onSuccess, ~onError, single);