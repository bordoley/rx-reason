let make = (
  propsStream: RxObservable.t('props), 
  createStateStream: RxObservable.t('props) => RxObservable.t('state), 
  createSideEffectsStream:RxObservable.t('props) => RxObservable.t(unit),
) => {
  let distinctPropsStream =
    propsStream
    |> RxObservables.share;

  RxObservables.merge([
    createStateStream(distinctPropsStream),
    createSideEffectsStream(distinctPropsStream)|> RxObservables.ignoreElements,
  ]);
};

let createComponent =
    (
      ~name=?,
      ~createSideEffectsStream: RxObservable.t('props) => RxObservable.t(unit)=_ => RxObservables.empty(),
      ~createStateStream: RxObservable.t('props) => RxObservable.t('state)=_ => RxObservables.empty(),
      render: (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    ) => {
  let propsToState = propsStream =>
    make(propsStream, createStateStream, createSideEffectsStream);

  RxReact.createComponent(~name?, ~propsToState, render);
};