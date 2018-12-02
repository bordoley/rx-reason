let useObservable = {
  let subscribe = observable => {
    let subscription = observable |> RxObservable.subscribe;
    () => subscription |> RxDisposable.dispose;
  };

  observable => React.useEffectWithCleanup1(subscribe, observable);
};

let useRef = {
  let doUpdate = (update, ref, action) => {
    let current = ref |> React.Ref.currentGet;
    switch (current) {
    | Some(ele) => update(ele, action)
    | None => ()
    };
  };

  (update, observable) => {
    let ref = React.useRef(None);

    let doUpdateMemoized = React.useMemo2(doUpdate, update, ref);

    let memoizedObservable =
      React.useMemo2(RxObservables.onNext, doUpdateMemoized, observable);

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
    |> RxSubject.asObservable
    |> propsToState
    |> RxObservables.observe1(~onNext, ~onComplete, setState);

  (propsToState, props) => {
    let propsStream = React.useMemo(RxSubjects.createMulticast);

    let (state, setState) = React.useState(Null);

    propsStream
    |> React.useMemo3(makeStateStream, propsToState, setState)
    |> useObservable;

    propsStream |> React.useEffect2(RxSubject.next, props);
    state;
  };
};

let createReactComponent =
    (
      ~name: option(string)=?,
      ~propsToState:
         RxObservable.observable('props) => RxObservable.t('state),
      ~renderDefault:
         (~key: string=?, ~props: unit, 'children) => React.Element.t=React.null,
      ~renderExn: (~key: string=?, ~props: exn, 'children) => React.Element.t=React.raise,
      ~render: (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    ) =>
  React.Component.create(
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
  React.Element.create(statefulComponent);
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
    : React.Element.t => {
  let ref = useRef(mutate, commands);
  let ref = Some(ref);
  component(~key?, ~ref?, ~props?, children);
};