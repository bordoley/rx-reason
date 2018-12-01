module State = {
  type t = {
    count: int,
    greeting: string,
    show: bool,
  };
};

module Action = {
  type t =
    | Click
    | SetGreeting(string)
    | Toggle;
};

let greetingStateComponent = {
  let useMemoCb = React.useMemo2((dispatch, action, ()) => dispatch(action));

  let name = "GreetingStateComponent";

  let render = (~key as _=?, ~props, ()) => {
    let ({count, greeting, show}: State.t, dispatch) = props;

    let incrementCount = useMemoCb(dispatch, Action.Click);
    let toggle = useMemoCb(dispatch, Action.Toggle);

    Components.greeting(
      ~props={count, greeting, show, incrementCount, toggle},
      (),
    );
  };

  RxReactStateDispatchComponent.create(~name, ~render, ());
};

let actions: RxEvent.t(Action.t) = RxEvent.create();

let stateStore =
  RxValue.create(
    {count: 0, greeting: "Initial Greeting", show: false}: State.t,
  );

let sideEffectsSubscription = {
  let reducer = (action, state) =>
    switch (action) {
    | Action.Click => {...state, State.count: state.State.count + 1}
    | Action.SetGreeting(greeting) => {...state, State.greeting}
    | Action.Toggle => {...state, State.show: ! state.State.show}
    };

  let onNext = (stateStore, action) =>
    stateStore |> RxValue.update1(reducer, action);

  let onComplete = (stateStore, _) => stateStore |> RxValue.dispose;

  actions
  |> RxEvent.asObservable
  |> RxObservable.observe1(~onNext, ~onComplete, stateStore)
  |> RxObservable.subscribe;
};

ReactDom.renderToElementWithId(
  greetingStateComponent(
    ~props={
      dispatch: action => actions |> RxEvent.trigger(action),
      stateStream: stateStore |> RxValue.asObservable,
    },
    (),
  ),
  "index2",
);

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    state := ! state^;
    actions |> RxEvent.trigger(Action.SetGreeting(state^ ? "true" : "false"));
  },
  5000,
)
|> ignore;

let windowPopStateSubscription =
  Webapi.Dom.window
  |> Webapi.Dom.Window.asEventTarget
  |> RxDomEventTarget.observeEvent("popstate")
  |> RxObservables.onNext(Js.log)
  |> RxObservable.subscribe;

Webapi.Dom.(
  History.(
    pushState(History.state(history), "My title", "?next=rock", history)
  )
);

let promise: Js.Promise.t(int) = Js.Promise.resolve(1);
let observable = promise |> RxPromise.toObservable;
observable
|> RxObservables.onNext(Js.log)
|> RxObservables.onComplete(Js.log)
|> RxObservable.subscribe;

let promise =
  observable
  |> RxObservables.mapTo("b")
  |> RxPromise.fromObservable
  |> Js.Promise.then_(a => {
       Js.log(a);
       Js.Promise.resolve();
     });