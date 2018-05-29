type state('props) = {
  reactElement: ReasonReact.reactElement,
  exn: option(exn),
  propsSubject: Rx.Subject.t('props),
};

type action =
  | SubscriptionError(option(exn))
  | Render(ReasonReact.reactElement);

let (<<) = (f1: 'b => 'a, f2: 'c => 'b) : ('c => 'a) =>
  (c: 'c) => f1(f2(c));

let reducer =(action, state) =>
  switch (action) {
  | SubscriptionError(exn) => ReasonReact.Update({...state, exn})
  | Render(reactElement) => ReasonReact.Update({...state, reactElement})
  };

let make =
    (
      ~name: string,
      ~state: Rx.Observable.t('props) => Rx.Observable.t('state),
      ~render: 'state => ReasonReact.reactElement,
    ) => {
  let component = ReasonReact.reducerComponentWithRetainedProps(name);
  let componentWithLifecyclesAndReducer = {
    ...component,
    reducer,
    didMount: ({send, state: {propsSubject}, onUnmount}) => {
      let propsObservable = propsSubject |> Rx.Subject.toObservable;
      let subscription =
        state(propsObservable)
        |> Rx.Observable.lift(
              Rx.Operators.map(render)
              << Rx.Operators.onNext(next => send(Render(next)))
              << Rx.Operators.onComplete(exn =>
                  switch (exn) {
                  | Some(_) => send(SubscriptionError(exn))
                  | None => ()
                  }
                ),
            )
        |> Rx.Observable.subscribe;
      onUnmount(() => subscription |> Rx.Disposable.dispose);
    },  
    shouldUpdate: ({oldSelf, newSelf}) => {
      let oldState = oldSelf.state;
      let newState = newSelf.state;

      oldState.reactElement !== newState.reactElement
      || oldState.exn !== newState.exn;
    },
    render: ({state: {exn, reactElement}}) =>
      switch (exn) {
      | Some(exn) => raise(exn)
      | None => reactElement
      },
  };

  (~props, children) => {
    ...componentWithLifecyclesAndReducer,
    initialState: () => {
      let propsSubject = Rx.Subject.createWithReplayBuffer(1);
      propsSubject |> Rx.Subject.toObserver |> Rx.Observer.next(props);
      {
        reactElement: Array.length(children) > 0 ? ReasonReact.array(children) : ReasonReact.null,
        exn: None,
        propsSubject,
      }
    },

    willReceiveProps: ({state}) => {
      let {propsSubject} = state;
      propsSubject |> Rx.Subject.toObserver |> Rx.Observer.next(props);
      state;
    },
  };
};
