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
                    React.Element.t
                      =?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => React.Element.t=?,
    ~render: (
               ~key: string=?,
               ~props: RenderProps.t('state, 'action, 'dispatcher),
               'children
             ) =>
             React.Element.t
  ) =>
  React.Component.t(Props.t('state, 'action, 'dispatcher), 'children);

let create:
  (
    ~name: string=?,
    ~renderDefault: (~key: string=?, ~props: unit, 'children) => React.Element.t=?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => React.Element.t=?,
    ~render: (~key: string=?, ~props: RenderProps.t('state, 'action, 'dispatcher), 'children) =>
             React.Element.t,
    unit,
    ~key: string=?,
    ~props: Props.t('state, 'action, 'dispatcher),
    'children
  ) =>
  React.Element.t;