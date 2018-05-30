module InnerComponent = {
  let component = ReasonReact.statelessComponent("InnerComponent");
  let make = (~count, ~greeting, ~incrementCount, ~show, ~toggle, _) => {
    ...component,
    render: _ => {
      let message =
        "You've clicked this " ++ string_of_int(count) ++ " times(s)";
      <div>
        <button onClick=incrementCount>
          (ReasonReact.string(message))
        </button>
        <button onClick=toggle>
          (ReasonReact.string("Toggle greeting"))
        </button>
        (show ? ReasonReact.string(greeting) : ReasonReact.null)
      </div>;
    },
  };
};

type props = string;

type state = {
  count: int,
  greeting: string,
  incrementCount: ReactEventRe.Mouse.t => unit,
  show: bool,
  toggle: ReactEventRe.Mouse.t => unit,
};

type actions =
  | Click
  | Toggle
  | SetTitle(string);

let reducer = (state, action) =>
  switch (action) {
  | Click => {...state, count: state.count + 1}
  | Toggle => {...state, show: ! state.show}
  | SetTitle(greeting) => {...state, greeting}
  };

let render = (~props as {count, greeting, incrementCount, show, toggle}: state, _) =>
  <InnerComponent count greeting incrementCount show toggle />;

let createStore = (props: RxReason.Observable.t(props)) : RxReason.Observable.t(state) => {
  let subject = RxReason.Subject.create();

  let actions = subject |> RxReason.Subject.toObservable;
  let propsActions =
    props
    |> RxReason.Observable.lift(RxReason.Operators.map(greeting => SetTitle(greeting)));

  let dispatch = (action, _) =>
    subject |> RxReason.Subject.toObserver |> RxReason.Observer.next(action);

  let initialState: state = {
    count: 0,
    greeting: "",
    incrementCount: dispatch(Click),
    show: false,
    toggle: dispatch(Toggle),
  };

  RxReason.Observable.merge([actions, propsActions])
  |> RxReason.Observable.lift(
      RxReason.Operators.scan(reducer, initialState),
    );
};

let component = RxReasonReact.Component.make(~createStore, ~render);
let make = (~greeting as props: string) => component(~props);
