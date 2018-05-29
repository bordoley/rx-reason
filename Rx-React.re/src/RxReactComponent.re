type action('state) =
  | None
  | Next('state)
  | Completed(option(exn));

let actionsEqual = (oldProps, newProps) => switch(oldProps, newProps) {
  | (None, None) => false
  | (Next(oldState), Next(newState)) => oldState !== newState
  | (Completed(oldException), Completed(newException)) => oldException !== newException
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
    actionsEqual(oldAction, newAction);
  };

let make =
    (
      ~name: string,
      ~createStore: Rx.Observable.t('props) => Rx.Observable.t('state),
    ) => {
  let component = ReasonReact.reducerComponentWithRetainedProps(name);
  let componentWithLifecyclesAndReducer = {
    ...component,
    reducer,
    didMount: ({send, state: {propsSubject}, onUnmount}) => {
      let subscription = propsSubject
        |> Rx.Subject.toObservable
        |> createStore
        |> Rx.Observable.lift(
              Rx.Operators.observe(
                ~onNext=next => send(Next(next)),
                ~onComplete=exn => send(Completed(exn))
              ),
            )
        |> Rx.Observable.subscribe;
      onUnmount(() => subscription |> Rx.Disposable.dispose);
    },
    shouldUpdate,
  };

  (
   ~render: (~props: 'state, array(ReasonReact.reactElement)) => ReasonReact.reactElement, 
   ~props: 'props, 
   children: array(ReasonReact.reactElement)
  ) => {
    ...componentWithLifecyclesAndReducer,
    initialState: () => {
      let propsSubject = Rx.Subject.createWithReplayBuffer(1);
      propsSubject |> Rx.Subject.toObserver |> Rx.Observer.next(props);
      {
        action: None,
        propsSubject,
      }
    },
    willReceiveProps: ({state}) => {
      let {propsSubject} = state;
      propsSubject |> Rx.Subject.toObserver |> Rx.Observer.next(props);
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
