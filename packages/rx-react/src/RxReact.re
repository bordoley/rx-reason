type state('state) =
  | Null
  | Next('state)
  | Error(exn);

let useRxState =
    (
      createStateStream:
        RxReason.Observable.t('props) => RxReason.Observable.t('state),
      createSideEffectsStream:
        RxReason.Observable.t('props) => RxReason.Observable.t(unit),
      props: 'props,
    )
    : state('state) => {
  let (state, setState) = React.useState(Null);

  let propsStream = React.useMemo(RxReason.Subject.create);

  React.useEffect1(
    () => {
      let distinctPropsStream =
        propsStream
        |> RxReason.Subject.asObservable
        |> RxReason.Observable.lift(RxReason.Operators.distinctUntilChanged)
        |> RxReason.Observables.share;

      let stateStream =
        createStateStream(distinctPropsStream)
        |> RxReason.Observable.pipe2(
             RxReason.Operators.onNext(state => setState(Next(state))),
             RxReason.Operators.mapTo(),
           );

      let subscription =
        RxReason.Observables.merge([
          stateStream,
          createSideEffectsStream(distinctPropsStream),
        ])
        |> RxReason.Observable.lift(
             RxReason.Operators.onExn(exn => setState(Error(exn))),
           )
        |> RxReason.Observable.subscribe;
      () => subscription |> RxReason.Disposable.dispose;
    },
    (),
  );

  React.useEffect1(
    () => {
      propsStream |> RxReason.Subject.next(props);
      () => ();
    },
    props,
  );

  state;
};

let createReactComponent =
    (
      ~name=?,
      ~createSideEffectsStream=_ => RxReason.Observables.empty(),
      ~createStateStream=_ => RxReason.Observables.empty(),
      render: (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    )
    : React.Component.t('props, 'children) =>
  React.Component.createReactComponent(
    ~name?,
    (~props, ~children) => {
      let state =
        useRxState(createStateStream, createSideEffectsStream, props);

      switch (state) {
      | Null => React.Element.null
      | Error(exn) => raise(exn)
      | Next(state) => render(~props=state, children)
      };
    },
  );

let createElement =
    (
      ~name=?,
      ~createSideEffectsStream=_ => RxReason.Observables.empty(),
      ~createStateStream=_ => RxReason.Observables.empty(),
      render: (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    ) => {
  let statefulComponent =
    createReactComponent(
      ~name?,
      ~createSideEffectsStream,
      ~createStateStream,
      render,
    );
  React.Element.create(statefulComponent);
};