let useObservable = {
  let subscribe = observable => {
    let subscription = observable |> RxObservable.subscribe;
    () => subscription |> RxDisposable.dispose;
  };

  (observable) =>
    React.useEffectWithCleanup1(subscribe, observable);
};

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

let useOnNextWithRef = (onNext, reactRef, observable) => {
  let observable =
    React.useMemo3(onNextWithRef, onNext, reactRef, observable);
  useObservable(observable);
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

  let makeStateStream = (propsToState, setState, propsStream) => {
    propsStream
    |> RxSubject.asObservable
    |> propsToState
    |> RxObservable.observe1(~onNext, ~onComplete, setState);
  };

  (propsToState, props) => {
    let propsStream = React.useMemo(RxSubjects.createMulticast);

    let (state, setState) = React.useState(Null);

    propsStream
    |> React.useMemo3(makeStateStream, propsToState, setState)
    |> useObservable;

    React.useEffect2(RxSubject.next, props, propsStream);
    state;
  };
};

let defaultRenderNull = (~key as _=?, _, _) => React.Element.null;
let defaultRenderExn = (~key as _=?, exn, _) => raise(exn);

let createReactComponent =
    (
      ~name: option(string)=?,
      ~propsToState: RxObservable.observable('props) => RxObservable.t('state),
      ~renderNull: (~key: string=?, unit, 'children) => React.Element.t=defaultRenderNull,
      ~renderExn: (~key: string=?, exn, 'children) => React.Element.t=defaultRenderExn,
      ~render: (~key: string=?, ~props: 'state, 'children) => React.Element.t
    ) =>
  React.Component.create(
    ~name?,
    (~props, ~children) => {
      let state = useObservableState(propsToState, props);

      switch (state) {
      | Null => renderNull((), children)
      | Error(exn) => renderExn(exn, children)
      | Next(props) => render(~props, children)
      };
    },
  );

let createComponent =
    (
      ~name=?,
      ~propsToState,
      ~renderNull=?,
      ~renderExn=?,
      ~render,
      (),
    ) => {
  let statefulComponent =
    createReactComponent(
      ~name?,
      ~propsToState,
      ~renderNull?,
      ~renderExn?,
      ~render,
    );
  React.Element.create(statefulComponent);
};