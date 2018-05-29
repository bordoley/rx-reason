type state('props, 'state) = {
  props: 'state,
  exn: option(exn),
  propsSubject: Rx.Subject.t('props),
};

type action('state) =
  | Next('state)
  | Complete(option(exn));

let (<<) = (f1: 'b => 'a, f2: 'c => 'b) : ('c => 'a) =>
  (c: 'c) => f1(f2(c));

let reducer =(action, state) =>
  switch (action) {
  | Next(props) => ReasonReact.Update({...state, props})
  | Complete(exn) => ReasonReact.Update({...state, exn})
  };

let shouldUpdate = (
  {oldSelf, newSelf}: ReasonReact.oldNewSelf(state('props, 'state), ReasonReact.noRetainedProps, action('state))
) => {
    let oldState = oldSelf.state;
    let newState = newSelf.state;

    oldState.props !== newState.props
    || oldState.exn !== newState.exn;
  };

let make =
    (
      ~name: string,
      ~state: Rx.Observable.t('props) => Rx.Observable.t('state),
      ~initialState: 'state,
    ) => {
  let component = ReasonReact.reducerComponentWithRetainedProps(name);
  let componentWithLifecyclesAndReducer = {
    ...component,
    reducer,
    didMount: ({send, state: {propsSubject}, onUnmount}) => {
      let subscription = propsSubject
        |> Rx.Subject.toObservable
        |> state
        |> Rx.Observable.lift(
              Rx.Operators.onNext(next => send(Next(next)))
              << Rx.Operators.onComplete(exn =>send(Complete(exn))),
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
        props: initialState,
        exn: None,
        propsSubject,
      }
    },

    willReceiveProps: ({state}) => {
      let {propsSubject} = state;
      propsSubject |> Rx.Subject.toObserver |> Rx.Observer.next(props);
      state;
    },
    render: ({state: {exn, props}}) =>
      switch (exn) {
      | Some(exn) => raise(exn)
      | None => render(~props, children)
      },
  };
};
