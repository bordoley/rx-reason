module Dispatcher = {
  type t('action) = 'action => unit;
};

module Props = {
  type t('state, 'action) = {
    stateStream: RxObservable.t('state),
    dispatch: Dispatcher.t('action),
  };
};

let createReactComponent =
    (~name=?, ~renderDefault=?, ~renderExn=?, ~render)
    : React.Component.t(Props.t('state, 'action), 'children) => {
  let f = ({stateStream, dispatch}: Props.t('state, 'action)) =>
    stateStream |> RxObservables.map(state => (state, dispatch));
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