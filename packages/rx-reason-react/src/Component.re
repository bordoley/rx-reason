let module Rx = RxReason;

type action('state) =
  | None
  | Next('state)
  | Completed(option(exn));

let actionsEqual = (oldProps, newProps) => switch(oldProps, newProps) {
  | (None, None) => true
  | (Next(oldState), Next(newState)) => oldState === newState
  | (Completed(oldException), Completed(newException)) => oldException === newException
  | _ => false
  };

type state('props, 'state) = {
  action: action('state),
  propsSubject: Rx.Subject.t('props),
};

let reducer = (action, state) => ReasonReact.Update({...state, action});

let shouldUpdate = (
  {oldSelf, newSelf}: ReasonReact.oldNewSelf(state('props, 'state), ReasonReact.noRetainedProps, action('state))
) => {
    let oldAction = oldSelf.state.action;
    let newAction = newSelf.state.action;
    !actionsEqual(oldAction, newAction);
  };

let make =
    (
      ~name: string,
      ~createStore: Rx.Observable.t('props) => Rx.Observable.t('state),
    ) => {
  let component = ReasonReact.reducerComponentWithRetainedProps(name);
  let didMount = (
    {send, state: {propsSubject}, onUnmount}: ReasonReact.self(state('props, 'state), ReasonReact.noRetainedProps, action('state))
  ) => {
    let subscription = propsSubject
      |> RxReason.Subject.toObservable
      |> createStore
      |> RxReason.Observable.lift(
            RxReason.Operators.observe(
              ~onNext=next => send(Next(next)),
              ~onComplete=exn => send(Completed(exn))
            ),
          )
      |> RxReason.Observable.subscribe;
    onUnmount(() => subscription |> RxReason.Disposable.dispose);
  };

  (
   ~render: (~props: 'state, array(ReasonReact.reactElement)) => ReasonReact.reactElement, 
   ~props: 'props, 
   children: array(ReasonReact.reactElement)
  ) => {
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
      | Next(props) => render(~props, children)
      | Completed(Some(exn)) => raise(exn)
      | _ => ReasonReact.null
      },
  };
};
