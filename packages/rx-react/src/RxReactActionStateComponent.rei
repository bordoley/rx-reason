module Props: {
  type t('action, 'state) = {
    actions: RxEvent.t('action),
    stateStore: RxValue.t('state),
  };
};

let createReactComponent:
  (
    ~name: string,
    ~mapStateToRenderProps: (
                              ~state: RxObservable.observable('state),
                              ~dispatch: 'action => unit
                            ) =>
                            RxObservable.t('renderProps),
    ~setupSideEffects: (
                         ~actions: RxObservable.observable('action),
                         ~stateStore: RxValue.t('state)
                       ) =>
                       RxObservable.t(unit),
    ~renderNull: (~key: string=?, unit, 'children) => React.Element.t=?,
    ~renderExn: (~key: string=?, exn, 'children) => React.Element.t=?,
    ~render: (~key: string=?, ~props: 'renderProps, 'children) =>
             React.Element.t
  ) =>
  React.Component.t(Props.t('action, 'state), 'children);

let create:
  (
    ~name: string,
    ~mapStateToRenderProps: (
                              ~state: RxObservable.observable('state),
                              ~dispatch: 'action => unit
                            ) =>
                            RxObservable.t('renderProps),
    ~setupSideEffects: (
                         ~actions: RxObservable.observable('action),
                         ~stateStore: RxValue.t('state)
                       ) =>
                       RxObservable.t(unit),
    ~renderNull: (~key: string=?, unit, 'children) => React.Element.t=?,
    ~renderExn: (~key: string=?, exn, 'children) => React.Element.t=?,
    ~render: (~key: string=?, ~props: 'renderProps, 'children) =>
             React.Element.t,
    unit,
    ~key: string=?,
    ~props: Props.t('action, 'state),
    'children
  ) =>
  React.Element.t;