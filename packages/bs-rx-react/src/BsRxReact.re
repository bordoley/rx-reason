let useObservable = {
  let subscribe = observable => {
    let subscription = observable |> RxObservable.connect;
    () => subscription |> RxDisposable.dispose;
  };

  observable => BsReact.useEffectWithCleanup1(subscribe, observable);
};

let useRef = {
  let doUpdate = (update, ref, action) => {
    let current = ref |> BsReact.Ref.currentGet;
    switch (current) {
    | Some(ele) => update(ele, action)
    | None => ()
    };
  };

  (update, observable) => {
    let ref = BsReact.useRef(None);

    let doUpdateMemoized = BsReact.useMemo2(doUpdate, update, ref);

    let memoizedObservable =
      BsReact.useMemo2(RxObservables.onNext, doUpdateMemoized, observable);

    useObservable(memoizedObservable);

    ref;
  };
};

type state('state) =
  | Null
  | Next('state)
  | Error(exn);

let useObservableState = {
  let onNext = (setState, next) => setState(Next(next));

  let onComplete = (setState, exn) =>
    switch (exn) {
    | Some(exn) => setState(Error(exn))
    | _ => setState(Null)
    };

  let makeStateStream = (propsToState, setState, propsStream) =>
    propsStream
    |> RxEvent.asObservable
    |> propsToState
    |> RxObservables.observeOn(BsRxPriorityScheduler.immediate)
    |> RxObservables.observe1(~onNext, ~onComplete, setState);

  (propsToState, props) => {
    let propsStream = BsReact.useMemo(RxEvent.create);

    let (state, setState) = BsReact.useState(Null);

    propsStream
    |> BsReact.useMemo3(makeStateStream, propsToState, setState)
    |> useObservable;

    propsStream |> BsReact.useEffect2(RxEvent.dispatch, props);
    state;
  };
};

let createReactComponent =
    (
      ~name: option(string)=?,
      ~propsToState:
         RxObservable.observable('props) => RxObservable.t('state),
      ~renderDefault:
         (~key: string=?, ~props: unit, 'children) => BsReact.Element.t=BsReact.null,
      ~renderExn: (~key: string=?, ~props: exn, 'children) => BsReact.Element.t=BsReact.raise,
      ~render: (~key: string=?, ~props: 'state, 'children) => BsReact.Element.t,
    ) =>
  BsReact.Component.create(
    ~name?,
    (~props, ~children) => {
      let state = useObservableState(propsToState, props);

      switch (state) {
      | Null => renderDefault(~props=(), children)
      | Error(exn) => renderExn(~props=exn, children)
      | Next(props) => render(~props, children)
      };
    },
  );

let createComponent =
    (~name=?, ~propsToState, ~renderDefault=?, ~renderExn=?, ~render, ()) => {
  let statefulComponent =
    createReactComponent(
      ~name?,
      ~propsToState,
      ~renderDefault?,
      ~renderExn?,
      ~render,
    );
  BsReact.Element.create(statefulComponent);
};

let useImperativeMethods =
    (
      mutate,
      component,
      ~key: option(string)=?,
      ~commands,
      ~props=?,
      children,
    )
    : BsReact.Element.t => {
  let ref = useRef(mutate, commands);
  let ref = Some(ref);
  component(~key?, ~ref?, ~props?, children);
};