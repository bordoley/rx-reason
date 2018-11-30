module Props = {
  type t('action, 'state) = {
    actions: RxEvent.t('action),
    stateStore: RxValue.t('state),
  };
};

let propsToStateMapper =
    (
      mapStateToRenderProps,
      setupSideEffects,
      {actions, stateStore}: Props.t('action, 'state),
    ) => {
  let dispatch = action => actions |> RxEvent.trigger(action);
  let actions = actions |> RxEvent.asObservable;
  let state = stateStore |> RxValue.asObservable;

  let renderPropsStream = mapStateToRenderProps(~state, ~dispatch);

  let sideEffectsStream =
    setupSideEffects(~actions, ~stateStore) |> RxObservables.ignoreElements;

  RxObservables.merge([renderPropsStream, sideEffectsStream]);
};

let propsToState = (mapStateToRenderProps, setupSideEffects, propsStream) =>
  propsStream
  |> RxObservables.map(
       propsToStateMapper(mapStateToRenderProps, setupSideEffects),
     )
  |> RxObservables.switch_;

let createReactComponent =
    (
      ~name: string,
      ~mapStateToRenderProps:
         (
           ~state: RxObservable.observable('state),
           ~dispatch: 'action => unit
         ) =>
         RxObservable.t('renderProps),
      ~setupSideEffects:
         (
           ~actions: RxObservable.observable('action),
           ~stateStore: RxValue.t('state)
         ) =>
         RxObservable.t(unit),
      ~renderNull:
         option((~key: string=?, unit, 'children) => React.Element.t)=?,
      ~renderExn: option((~key: string=?, exn, 'children) => React.Element.t)=?,
      ~render:
         (~key: string=?, ~props: 'renderProps, 'children) => React.Element.t,
    ) =>
  RxReact.createReactComponent(
    ~name,
    ~propsToState=propsToState(mapStateToRenderProps, setupSideEffects),
    ~renderNull?,
    ~renderExn?,
    ~render,
  );

let create =
    (
      ~name,
      ~mapStateToRenderProps,
      ~setupSideEffects,
      ~renderNull=?,
      ~renderExn=?,
      ~render,
      (),
    ) =>
  RxReact.createComponent(
    ~name,
    ~propsToState=propsToState(mapStateToRenderProps, setupSideEffects),
    ~renderNull?,
    ~renderExn?,
    ~render,
    (),
  );