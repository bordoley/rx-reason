let useObservable: RxObservable.t('a) => unit;

let useRef:
  (('a, 'action) => unit, RxObservable.t('action)) => BsReact.Ref.t('a);

let createReactComponent:
  (
    ~name: string=?,
    ~propsToState: RxObservable.observable('props) => RxObservable.t('state),
    ~renderDefault: (~key: string=?, ~props: unit, 'children) =>
                    BsReact.Element.t
                      =?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => BsReact.Element.t=?,
    ~render: (~key: string=?, ~props: 'state, 'children) => BsReact.Element.t
  ) =>
  BsReact.Component.t('props, 'children);

let createComponent:
  (
    ~name: string=?,
    ~propsToState: RxObservable.observable('props) => RxObservable.t('state),
    ~renderDefault: (~key: string=?, ~props: unit, 'children) =>
                    BsReact.Element.t
                      =?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => BsReact.Element.t=?,
    ~render: (~key: string=?, ~props: 'state, 'children) => BsReact.Element.t,
    unit,
    ~key: string=?,
    ~props: 'props,
    'children
  ) =>
  BsReact.Element.t;

let useImperativeMethods:
  (
    ('a, 'command) => unit,
    (~key: string=?, ~ref: BsReact.Ref.t('a)=?, ~props: 'props=?, 'children) =>
    BsReact.Element.t,
    ~key: string=?,
    ~commands: RxObservable.t('command),
    ~props: 'props=?,
    'children
  ) =>
  BsReact.Element.t;