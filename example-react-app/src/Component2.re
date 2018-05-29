module InnerComponent = {
  let component = ReasonReact.statelessComponent("InnerComponent");

  let make = (~count, ~greeting, ~incrementCount, ~show, ~toggle, _children) => {
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

type action =
  | Click
  | Toggle
  | SetTitle(string);

type state = {
  count: int,
  greeting: string,
  incrementCount: ReactEventRe.Mouse.t => unit,
  show: bool,
  toggle: ReactEventRe.Mouse.t => unit,
};

type props = action;

let (<<) = (f1: 'b => 'a, f2: 'c => 'b) : ('c => 'a) =>
  (c: 'c) => f1(f2(c));

let stateTransform =
    (obs: Rx.Observable.t(props))
    : Rx.Observable.t(ReasonReact.reactElement) => {
  let subject = Rx.Subject.create();

  let initialState: state = {
    count: 0,
    greeting: "",
    incrementCount: _ =>
      subject |> Rx.Subject.toObserver |> Rx.Observer.next(Click),
    show: false,
    toggle: _ => subject |> Rx.Subject.toObserver |> Rx.Observer.next(Toggle),
  };

  Rx.Observable.merge([subject |> Rx.Subject.toObservable, obs])
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
       << Rx.Operators.map(({count, greeting, incrementCount, show, toggle}) =>
            <InnerComponent count greeting incrementCount show toggle />
          ),
     );
};

let component = RxReactComponent.create("Example", stateTransform);
let make = (~greeting: string, _children) => component(SetTitle(greeting));