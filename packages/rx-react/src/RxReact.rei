let useObservable: RxObservable.t('a) => unit;
let useOnNextWithRef:
  (('a, 'b) => unit, React.Ref.t('b), RxObservable.t('a)) => unit;

let createReactComponent:
  (
    ~name: string=?,
    ~propsToState: RxObservable.observable('props) => RxObservable.t('state),
    ~renderDefault: (~key: string=?, ~props: unit, 'children) => React.Element.t=?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => React.Element.t=?,
    ~render: (~key: string=?, ~props: 'state, 'children) => React.Element.t
  ) =>
  React.Component.t('props, 'children);

let createComponent:
  (
    ~name: string=?,
    ~propsToState: RxObservable.observable('props) => RxObservable.t('state),
    ~renderDefault: (~key: string=?, ~props: unit, 'children) => React.Element.t=?,
    ~renderExn: (~key: string=?,  ~props: exn, 'children) => React.Element.t=?,
    ~render: (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    unit,
    ~key: string=?,
    ~props: 'props,
    'children
  ) =>
  React.Element.t;