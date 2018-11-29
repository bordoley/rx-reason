type props = {
  count: int,
  greeting: string,
  incrementCount: unit => unit,
  show: bool,
  toggle: unit => unit,
};

module Actions = {
  type t =
    | Click
    | Toggle;
};

type state = {
  count: int,
  show: bool,
};

let reducer = (action, state) =>
  switch (action) {
  | Actions.Click => {...state, count: state.count + 1}
  | Actions.Toggle => {...state, show: ! state.show}
  };

let create = (props: RxObservable.t(string)) : RxObservable.t(props) => {
  let state = RxValue.create({count: 0, show: false});

  let dispatch = (action, ()) => state |> RxValue.update1(reducer, action);
  let incrementCount = dispatch(Actions.Click);
  let toggle = dispatch(Actions.Toggle);

  RxObservables.combineLatest2(
    ~selector=
      (greeting, {count, show}) => {
        count,
        greeting,
        incrementCount,
        show,
        toggle,
      },
    props,
    state |> RxValue.asObservable,
  );
};