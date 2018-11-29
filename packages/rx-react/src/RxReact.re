let useObservable = (observable: RxObservable.t('a)) =>
  React.useEffectWithCleanup1(
    observable => {
      let subscription = observable |> RxObservable.subscribe;
      () => subscription |> RxDisposable.dispose;
    },
    observable,
  );

  let onNextWithRef = {
    let onNextCallback = (onNext, reactRef, next) => {
      let ele = reactRef |> React.Ref.currentGet;
      switch (ele) {
      | Some(ele) => ele |> onNext(next)
      | None => ()
      };
    };
  
    (onNext, reactRef, observable) =>
      observable |> RxObservables.onNext2(onNextCallback, onNext, reactRef);
  };

let useOnNextWithRef =
  (onNext, reactRef, observable) => {
    let observable = React.useMemo3(onNextWithRef, onNext, reactRef, observable);
    useObservable(observable);
  };

type state('state) =
  | Null
  | Next('state)
  | Error(exn);

let useObservableState = {
  let make =
      (propsStream, createStateStream, createSideEffectsStream, setState) => {
    let distinctPropsStream =
      propsStream
      |> RxSubject.asObservable
      |> RxObservables.distinctUntilChanged
      |> RxObservables.observeOn(RxJsSchedulers.PriorityScheduler.immediate)
      |> RxObservables.share;

    let stateStream =
      createStateStream(distinctPropsStream)
      |> RxObservables.observeOn(RxJsSchedulers.PriorityScheduler.immediate)
      |> RxObservables.onNext(state => setState(Next(state)))
      |> RxObservables.mapTo();

    RxObservables.merge([
      stateStream,
      createSideEffectsStream(distinctPropsStream),
    ])
    |> RxObservables.observeOn(RxJsSchedulers.PriorityScheduler.immediate)
    |> RxObservables.onExn(exn => setState(Error(exn)));
  };

  (
    createStateStream: RxObservable.t('props) => RxObservable.t('state),
    createSideEffectsStream: RxObservable.t('props) => RxObservable.t(unit),
    props: 'props,
  ) => {
    let propsStream = React.useMemo(RxSubject.create);
    let (state, setState) = React.useState(Null);

    let observable =
      React.useMemo4(
        make,
        propsStream,
        createStateStream,
        createSideEffectsStream,
        setState,
      );

    useObservable(observable);

    React.useEffect2(RxSubject.next, props, propsStream);
    state;
  };
};

let createReactComponent =
    (
      ~name=?,
      ~createSideEffectsStream=_ => RxObservables.empty(),
      ~createStateStream=_ => RxObservables.empty(),
      ~renderNull=() => React.Element.null,
      ~renderExn=raise,
      render: (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    )
    : React.Component.t('props, 'children) =>
  React.Component.create(
    ~name?,
    (~props, ~children) => {
      let state =
        useObservableState(createStateStream, createSideEffectsStream, props);

      switch (state) {
      | Null => renderNull()
      | Error(exn) => renderExn(exn)
      | Next(state) => render(~props=state, children)
      };
    },
  );

let createComponent =
    (
      ~name=?,
      ~createSideEffectsStream=_ => RxObservables.empty(),
      ~createStateStream=_ => RxObservables.empty(),
      ~renderNull=() => React.Element.null,
      ~renderExn=raise,
      render: (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    ) => {
  let statefulComponent =
    createReactComponent(
      ~name?,
      ~createSideEffectsStream,
      ~createStateStream,
      ~renderNull,
      ~renderExn,
      render,
    );
  React.Element.create(statefulComponent);
};