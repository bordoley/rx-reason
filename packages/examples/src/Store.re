type props = {
  count: int,
  greeting: string,
  incrementCount: ReactDomEvent.Mouse.t => unit,
  show: bool,
  toggle: ReactDomEvent.Mouse.t => unit,
};

module Actions = {
  type t =
    | Click
    | Toggle
    | SetTitle(string);
};

let reducer = (state: props, action) =>
  switch (action) {
  | Actions.Click => {...state, count: state.count + 1}
  | Actions.Toggle => {...state, show: ! state.show}
  | Actions.SetTitle(greeting) => {...state, greeting}
  };

let create =
    (props: RxObservable.t(string))
    : RxObservable.t(props) => {
  let subject = RxSubject.create();

  let actions = subject |> RxSubject.asObservable;
  let propsActions =
    props
    |> RxObservable.lift(
         RxOperators.map(greeting => Actions.SetTitle(greeting)),
       );

  let dispatch = (action, _) => subject |> RxSubject.next(action);

  let initialState: props = {
    count: 0,
    greeting: "",
    incrementCount: dispatch(Actions.Click),
    show: false,
    toggle: dispatch(Actions.Toggle),
  };

  RxObservables.merge([actions, propsActions])
  |> RxObservable.lift(RxOperators.scan(reducer, initialState));
};