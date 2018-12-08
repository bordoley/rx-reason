module Props: {
  type t('state, 'action, 'dispatcher) = {
    stateStream: RxObservable.t('state),
    dispatch: ('action, 'dispatcher) => unit,
    dispatcher: 'dispatcher,
  };
};

module RenderProps: {
  type t('state, 'action, 'dispatcher) = {
    state: 'state,
    dispatch: ('action, 'dispatcher) => unit,
    dispatcher: 'dispatcher,
  };
};

let createReactComponent:
  (
    ~name: string=?,
    ~renderDefault: (~key: string=?, ~props: unit, 'children) =>
                    BsReact.Element.t
                      =?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => BsReact.Element.t=?,
    ~render: (
               ~key: string=?,
               ~props: RenderProps.t('state, 'action, 'dispatcher),
               'children
             ) =>
             BsReact.Element.t
  ) =>
  BsReact.Component.t(Props.t('state, 'action, 'dispatcher), 'children);

let create:
  (
    ~name: string=?,
    ~renderDefault: (~key: string=?, ~props: unit, 'children) => BsReact.Element.t=?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => BsReact.Element.t=?,
    ~render: (~key: string=?, ~props: RenderProps.t('state, 'action, 'dispatcher), 'children) =>
             BsReact.Element.t,
    unit,
    ~key: string=?,
    ~props: Props.t('state, 'action, 'dispatcher),
    'children
  ) =>
  BsReact.Element.t;