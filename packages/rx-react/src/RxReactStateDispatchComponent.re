module Props = {
  type t('state, 'action, 'dispatcher) = {
    stateStream: RxObservable.t('state),
    dispatch: ('action, 'dispatcher) => unit,
    dispatcher: 'dispatcher,
  };
};

let createReactComponent =
    (~name=?, ~renderDefault=?, ~renderExn=?, ~render)
    : React.Component.t(Props.t('state, 'action, 'dispatcher), 'children) => {
  let f = ({stateStream, dispatch, dispatcher}: Props.t('state, 'action, 'dispatcher)) =>
    stateStream |> RxObservables.map(state => (state, dispatch, dispatcher));
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