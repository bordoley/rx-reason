type state('state) =
  | Null
  | Next('state)
  | Error(exn);

let useRxState =
    (
      createStateStream:
        RxReason.Observable.t('props) => RxReason.Observable.t('state),
      props: 'props,
    )
    : state('state) => {
  let (state, setState) = React.useState(Null);

  let propsStream = React.useMemo(RxReason.Subject.create);

  React.useEffect1(
    (.) => {
      let distinctPropsStream =
        propsStream
        |> RxReason.Subject.asObservable
        |> RxReason.Observable.lift(RxReason.Operators.distinctUntilChanged);

      let stateStream = createStateStream(distinctPropsStream);

      let subscription =
        stateStream
        |> RxReason.Observable.pipe2(
             RxReason.Operators.distinctUntilChanged,
             RxReason.Operators.observe(
               ~onNext=state => setState(Next(state)),
               ~onComplete=
                 exn =>
                   switch (exn) {
                   | None => ()
                   | Some(exn) => setState(Error(exn))
                   },
             ),
           )
        |> RxReason.Observable.subscribe;

      () => subscription |> RxReason.Disposable.dispose;
    },
    (),
  );

  React.useEffect1(
    (.) => {
      propsStream |> RxReason.Subject.next(props);
      () => ();
    },
    props,
  );

  state;
};

let useRxSideEffects =
    (
      createSideEffectsStream:
        RxReason.Observable.t('props) => RxReason.Observable.t(unit),
      props: 'props,
    )
    : option(exn) => {
  let (state, setState) = React.useState(None);

  let propsStream = React.useMemo(RxReason.Subject.create);

  React.useEffect1(
    (.) => {
      let distinctPropsStream =
        propsStream
        |> RxReason.Subject.asObservable
        |> RxReason.Observable.lift(RxReason.Operators.distinctUntilChanged);

      let stateStream = createSideEffectsStream(distinctPropsStream);

      let subscription =
        stateStream
        |> RxReason.Observable.pipe2(
             RxReason.Operators.distinctUntilChanged,
             RxReason.Operators.onComplete(exn =>
               switch (exn) {
               | None => ()
               | Some(_) => setState(exn)
               }
             ),
           )
        |> RxReason.Observable.subscribe;

      () => subscription |> RxReason.Disposable.dispose;
    },
    (),
  );

  React.useEffect1(
    (.) => {
      propsStream |> RxReason.Subject.next(props);
      () => ();
    },
    props,
  );

  state;
};

let createComponent =
    (
      ~name=?,
      ~createSideEffectsStream=_ => RxReason.Observables.empty(),
      ~createStateStream=_ => RxReason.Observables.empty(),
      render: (~key: string=?, ~props: 'state, unit) => React.Element.t,
    )
    : React.Component.t('props) =>
  React.Component.create(
    ~name?,
    props => {
      let state = useRxState(createStateStream, props);
      let sideEffects = useRxSideEffects(createSideEffectsStream, props);

      switch (sideEffects) {
      | Some(exn) => raise(exn)
      | _ => ()
      };

      switch (state) {
      | Null => React.Element.null
      | Error(exn) => raise(exn)
      | Next(state) => render(~props=state, ())
      };
    },
  );

let createElement =
    (
      ~name=?,
      ~createSideEffectsStream=_ => RxReason.Observables.empty(),
      ~createStateStream=_ => RxReason.Observables.empty(),
      render: (~key: string=?, ~props: 'state, unit) => React.Element.t,
    ) => {
  let statefulComponent =
    createComponent(
      ~name?,
      ~createSideEffectsStream,
      ~createStateStream,
      render,
    );
  React.Element.create(statefulComponent);
};