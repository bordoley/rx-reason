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

let createReactComponent = {
  let stateToRenderProps =
      (dispatch, dispatcher, state)
      : RenderProps.t('state, 'action, 'dispatch) => {
    state,
    dispatch,
    dispatcher,
  };

  let f =
      (
        {stateStream, dispatch, dispatcher}:
          Props.t('state, 'action, 'dispatcher),
      ) =>
    stateStream
    |> RxObservables.map2(stateToRenderProps, dispatch, dispatcher);

  (~name=?, ~renderDefault=?, ~renderExn=?, ~render) => {
    let propsToState = RxObservables.switchMap(f);

    RxReact.createReactComponent(
      ~name?,
      ~propsToState,
      ~renderDefault?,
      ~renderExn?,
      ~render,
    );
  };
};

let create = (~name=?, ~renderDefault=?, ~renderExn=?, ~render, ()) => {
  let statefulComponent =
    createReactComponent(~name?, ~renderDefault?, ~renderExn?, ~render);
  React.Element.create(statefulComponent);
};