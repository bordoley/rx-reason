let module Rx = RxReason;

module Action = {
  type t('state) =
    | None
    | Next('state)
    | Completed(option(exn));

  let equal = (oldAction, newAction) => switch(oldAction, newAction) {
    | (None, None) => true
    | (Next(oldState), Next(newState)) => oldState === newState
    | (Completed(oldException), Completed(newException)) => oldException === newException
    | _ => false
    };
};

module type RxReasonReactComponentSpec = {
  type props;
  type state;

  let name: string;
  let createStore: Rx.Observable.t(props) => Rx.Observable.t(state);
  let render: (~state: state, array(ReasonReact.reactElement)) => ReasonReact.reactElement;
};

module type RxReasonReactComponent = {
  type props;
  type state;
  type action;

  let make: (~props: props, array(ReasonReact.reactElement)) =>ReasonReact.componentSpec(
    state,
    state,
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action,
  )
};

module Make = (ComponentSpec: RxReasonReactComponentSpec): (RxReasonReactComponent with type props := ComponentSpec.props) => {
  type props = ComponentSpec.props;
  type action = Action.t(ComponentSpec.state);
  type state = {
    action: action,
    propsSubject: Rx.Subject.t(props),
  };

  let component = ReasonReact.reducerComponent(ComponentSpec.name);

  let didMount = (
    {send, state: {propsSubject}, onUnmount}: ReasonReact.self(state, ReasonReact.noRetainedProps, action)
  ) => {
    let subscription = propsSubject
      |> RxReason.Subject.toObservable
      |> RxReason.Observable.lift(observer =>
          RxReason.Operators.distinctUntilChanged
          /* FIXME: In the future React will expose it's scheduler via an api.
           * We should schedule using that instead of our home rolled eventloop.
           * https://github.com/facebook/react/tree/master/packages/react-scheduler
           */
          @@ RxReason.Operators.observeOn(RxReasonJs.JSEventLoop.scheduler)
          @@ observer,
        )
      |> ComponentSpec.createStore
      |> RxReason.Observable.lift(
            RxReason.Operators.observe(
              ~onNext=next => send(Next(next)),
              ~onComplete=exn => send(Completed(exn))
            ),
          )
      |> RxReason.Observable.subscribe;
    onUnmount(() => subscription |> RxReason.Disposable.dispose);
  };

  let reducer = (action, state) => ReasonReact.Update({...state, action});

  let shouldUpdate = (
    {oldSelf, newSelf}: ReasonReact.oldNewSelf(state, ReasonReact.noRetainedProps, action)
  ) => {
      let oldAction = oldSelf.state.action;
      let newAction = newSelf.state.action;
      !Action.equal(oldAction, newAction);
    };

  let make = (~props: props, children) => {
    ...component,
    reducer,
    didMount,
    shouldUpdate,
    initialState: () => {
      let propsSubject = RxReason.Subject.createWithReplayBuffer(1);
      propsSubject |> RxReason.Subject.toObserver |> RxReason.Observer.next(props);
      {
        action: None,
        propsSubject,
      }
    },
    willReceiveProps: ({state}) => {
      let {propsSubject} = state;
      propsSubject |> RxReason.Subject.toObserver |> RxReason.Observer.next(props);
      state;
    },
    render: ({state: { action }}) =>
      switch (action) {
      | Next(state) => ComponentSpec.render(~state, children)
      | Completed(Some(exn)) => raise(exn)
      | _ => ReasonReact.null
      },
  };
};
