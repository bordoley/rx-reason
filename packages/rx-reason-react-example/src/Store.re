module State = {
  type t = {
    count: int,
    greeting: string,
    incrementCount: ReactEvent.Mouse.t => unit,
    show: bool,
    toggle: ReactEvent.Mouse.t => unit,
  };
};

module Actions = {
  type t =
    | Click
    | Toggle
    | SetTitle(string);
};

let reducer = (state: State.t, action) =>
  switch (action) {
  | Actions.Click => {...state, count: state.count + 1}
  | Actions.Toggle => {...state, show: ! state.show}
  | Actions.SetTitle(greeting) => {...state, greeting}
  };

let create =
    (props: RxReason.Observable.t(string))
    : RxReason.Observable.t(State.t) => {
  let subject = RxReason.Subject.create();

  let actions = subject |> RxReason.Subject.asObservable;
  let propsActions =
    props
    |> RxReason.Observable.lift(
         RxReason.Operators.map(greeting => Actions.SetTitle(greeting)),
       );

  let dispatch = (action, _) => subject |> RxReason.Subject.next(action);

  let initialState: State.t = {
    count: 0,
    greeting: "",
    incrementCount: dispatch(Actions.Click),
    show: false,
    toggle: dispatch(Actions.Toggle),
  };

  RxReason.Observables.merge([actions, propsActions])
  |> RxReason.Observable.lift(RxReason.Operators.scan(reducer, initialState));
};