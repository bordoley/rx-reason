let useObservable: RxObservable.t('a) => unit;
let useOnNextWithRef:
  (('a, 'b) => unit, React.Ref.t('b), RxObservable.t('a)) => unit;

let createReactComponent:
  (
    ~name: string=?,
    ~createSideEffectsStream: RxObservable.t('props) => RxObservable.t(unit)
                                =?,
    ~createStateStream: RxObservable.t('props) => RxObservable.t('state)=?,
    ~renderNull: unit => React.Element.t=?,
    ~renderExn: exn => React.Element.t=?,
    (~key: string=?, ~props: 'state, 'children) => React.Element.t
  ) =>
  React.Component.t('props, 'children);

let createComponent:
  (
    ~name: string=?,
    ~createSideEffectsStream: RxObservable.t('a) => RxObservable.t(unit)=?,
    ~createStateStream: RxObservable.t('a) => RxObservable.t('state)=?,
    ~renderNull: unit => React.Element.t=?,
    ~renderExn: exn => React.Element.t=?,
    (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    ~key: string=?,
    ~props: 'a,
    'children
  ) =>
  React.Element.t;