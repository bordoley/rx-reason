type state('state) =
  | Null
  | Next('state)
  | Error(exn);

let useRxState =
    (
      createStateStream: RxObservable.t('props) => RxObservable.t('state),
      createSideEffectsStream:
        RxObservable.t('props) => RxObservable.t(unit),
      props: 'props,
    )
    : state('state) => {
  let (state, setState) = React.useState(Null);

  let propsStream = React.useMemo(RxSubject.create);

  React.useEffectWithCleanup3(
    (propsStream, createStateStream, createSideEffectsStream) => {
      let distinctPropsStream =
        propsStream
        |> RxSubject.asObservable
        |> RxObservables.pipe2(
             RxOperators.distinctUntilChanged,
             RxOperators.observeOn(
               RxJsSchedulers.PriorityScheduler.immediate,
             ),
           )
        |> RxObservables.share;

      let stateStream =
        createStateStream(distinctPropsStream)
        |> RxObservables.pipe3(
             RxOperators.observeOn(
               RxJsSchedulers.PriorityScheduler.immediate,
             ),
             RxOperators.onNext(state => setState(Next(state))),
             RxOperators.mapTo(),
           );

      let subscription =
        RxObservables.merge([
          stateStream,
          createSideEffectsStream(distinctPropsStream),
        ])
        |> RxObservables.pipe2(
             RxOperators.observeOn(
               RxJsSchedulers.PriorityScheduler.immediate,
             ),
             RxOperators.onExn(exn => setState(Error(exn))),
           )
        |> RxObservable.subscribe;
      () => subscription |> RxDisposable.dispose;
    },
    propsStream,
    createStateStream,
    createSideEffectsStream,
  );

  React.useEffect2(RxSubject.next, props, propsStream);
  state;
};

let createReactComponent =
    (
      ~name=?,
      ~createSideEffectsStream=_ => RxObservables.empty(),
      ~createStateStream=_ => RxObservables.empty(),
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
      ~createSideEffectsStream=_ => RxObservables.empty(),
      ~createStateStream=_ => RxObservables.empty(),
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