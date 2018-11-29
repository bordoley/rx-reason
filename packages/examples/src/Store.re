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
    | Toggle;
};

type state = {
  count: int,
  greeting: string,
  show: bool,
};

let reducer = (action, state) =>
  switch (action) {
  | Actions.Click => {...state, count: state.count + 1}
  | Actions.Toggle => {...state, show: ! state.show}
  };

let create = (props: RxObservable.t(string)) : RxObservable.t(props) =>
  props
  |> RxObservables.map(greeting => {
       let state = RxValue.create({count: 0, greeting, show: false});

       let dispatch = (action, _) =>
         state |> RxValue.update1(reducer, action);
       let incrementCount = dispatch(Actions.Click);
       let toggle = dispatch(Actions.Toggle);

       state
       |> RxValue.asObservable
       |> RxObservables.map(({count, greeting, show}) =>
            {count, greeting, incrementCount, show, toggle}
          );
     })
  |> RxObservables.switch_;