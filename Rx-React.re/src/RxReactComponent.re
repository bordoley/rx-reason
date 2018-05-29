type state('props) = {
  reactElement: ReasonReact.reactElement,
  exn: option(exn),
  propsSubject: Rx.Subject.t('props),
};

type action =
  | SubscriptionError(option(exn))
  | Render(ReasonReact.reactElement);

type t('props) =
  'props =>
  ReasonReact.componentSpec(
    state('props),
    state('props),
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action,
  );

let create =
    (
      name: string,
      f:
        Rx.Observable.t('props) => Rx.Observable.t(ReasonReact.reactElement),
    )
    : t('props) => {
  let component = ReasonReact.reducerComponentWithRetainedProps(name);
  let componentWithInitialStateAndReducer = {
    ...component,
    initialState: () => {
      reactElement: ReasonReact.null,
      exn: None,
      propsSubject: Rx.Subject.create(),
    },
    reducer: (action, state) =>
      switch (action) {
      | SubscriptionError(exn) => ReasonReact.Update({...state, exn})
      | Render(reactElement) => ReasonReact.Update({...state, reactElement})
      },
    shouldUpdate: ({oldSelf, newSelf}) => {
      let oldState = oldSelf.state;
      let newState = newSelf.state;

      oldState.reactElement !== newState.reactElement ||
      oldState.exn !== newState.exn;
    },
    render: ({state: {exn, reactElement}}) =>
      switch (exn) {
      | Some(exn) => raise(exn)
      | None => reactElement
      },
  };

  props => {
    ...componentWithInitialStateAndReducer,
    didMount: ({send, state: {propsSubject}, onUnmount}) => {
      let propsObservable = propsSubject |> Rx.Subject.toObservable;
      let subscription =
        f(propsObservable)
        |> Rx.Observable.subscribe(
             ~onNext=next => send(Render(next)),
             ~onComplete=
               exn =>
                 switch (exn) {
                 | Some(_) => send(SubscriptionError(exn))
                 | None => ()
                 },
           );
      onUnmount(() => subscription |> Rx.Disposable.dispose);
      propsSubject |> Rx.Subject.toObserver |> Rx.Observer.next(props);
    },
    willReceiveProps: ({state}) => {
      let {propsSubject} = state;
      propsSubject |> Rx.Subject.toObserver |> Rx.Observer.next(props);
      state;
    },
  };
};