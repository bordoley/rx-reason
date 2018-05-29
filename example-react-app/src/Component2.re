module InnerComponent = {
  let make = (~count, ~greeting, ~incrementCount, ~show, ~toggle, _children) => {
    ...ReasonReact.statelessComponent("InnerComponent"),
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

let (<<) = (f1: 'b => 'a, f2: 'c => 'b) : ('c => 'a) =>
  (c: 'c) => f1(f2(c));

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

let render = ({count, greeting, incrementCount, show, toggle}: state) =>
  <InnerComponent count greeting incrementCount show toggle />;

let state = (props: Rx.Observable.t(props)) : Rx.Observable.t(state) => {
  let subject = Rx.Subject.create();
  let actions = subject |> Rx.Subject.toObservable;
  let dispatch = (action, _) =>
    subject |> Rx.Subject.toObserver |> Rx.Observer.next(action);

  let initialState: state = {
    count: 0,
    greeting: "",
    incrementCount: dispatch(Click),
    show: false,
    toggle: dispatch(Toggle),
  };

  Rx.Observable.merge([
    actions,
    props
    |> Rx.Observable.lift(Rx.Operators.map(greeting => SetTitle(greeting))),
  ])
  |> Rx.Observable.lift(
       Rx.Operators.scan(
         (state, action) =>
           switch (action) {
           | Click => {...state, count: state.count + 1}
           | Toggle => {...state, show: ! state.show}
           | SetTitle(greeting) => {...state, greeting}
           },
         initialState,
       )
     );
};

let component = RxReactComponent.create(~name="Example", ~state, ~render);
let make = (~greeting: string, _children) => component(greeting);