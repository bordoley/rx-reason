module Props = {
  type t('state, 'action, 'dispatcher) = {
    stateStream: RxObservable.t('state),
    dispatch: ('action, 'dispatcher) => unit,
    dispatcher: 'dispatcher,
  };
};

module RenderProps = {
  type t('state, 'action, 'dispatcher) = {
    state: 'state,
    dispatch: ('action, 'dispatcher) => unit,
    dispatcher: 'dispatcher,
  };
};

let createReactComponent =
    (~name=?, ~renderDefault=?, ~renderExn=?, ~render)
    : React.Component.t(Props.t('state, 'action, 'dispatcher), 'children) => {
  let f =
      (
        {stateStream, dispatch, dispatcher}:
          Props.t('state, 'action, 'dispatcher),
      ) =>
    stateStream
    |> RxObservables.map(state =>
         (
           {state, dispatch, dispatcher}:
             RenderProps.t('state, 'action, 'dispatch)
         )
       );
  let propsToState = RxObservables.switchMap(f);

  RxReact.createReactComponent(
    ~name?,
    ~propsToState,
    ~renderDefault?,
    ~renderExn?,
    ~render,
  );
};

let create = (~name=?, ~renderDefault=?, ~renderExn=?, ~render, ()) => {
  let statefulComponent =
    createReactComponent(~name?, ~renderDefault?, ~renderExn?, ~render);
  React.Element.create(statefulComponent);
};