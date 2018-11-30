let greeting = Components.greeting;

let windowPopStateSubscription =
  Webapi.Dom.window
  |> Webapi.Dom.Window.asEventTarget
  |> RxDomEventTarget.observeEvent("popstate")
  |> RxObservables.onNext(Js.log)
  |> RxObservable.subscribe;

Webapi.Dom.(
  History.(pushState(state(history), "My title", "?next=rock", history))
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

type state = {
  count: int,
  greeting: string,
  show: bool,
};

module Actions = {
  type t =
    | Click
    | SetGreeting(string)
    | Toggle;
};

let actions: RxEvent.t(Actions.t) = RxEvent.create();

let stateStore =
  RxValue.create({count: 0, greeting: "Initial Greeting", show: false});

let mapStateToRenderProps =
    (~state: RxObservable.t('state), ~dispatch)
    : RxObservable.t(Components.greetingProps) => {
  let incrementCount = () => dispatch(Actions.Click);
  let toggle = () => dispatch(Actions.Toggle);

  state
  |> RxObservables.map(state =>
       (
         {
           count: state.count,
           greeting: state.greeting,
           show: state.show,
           incrementCount,
           toggle,
         }: Components.greetingProps
       )
     );
};

let setupSideEffects = (~actions, ~stateStore) =>
  actions
  |> RxObservables.onNext(action =>
       switch (action) {
       | Actions.Click =>
         stateStore
         |> RxValue.update(state => {...state, count: state.count + 1})
       | Actions.SetGreeting(greeting) =>
         stateStore |> RxValue.update(state => {...state, greeting})
       | Actions.Toggle =>
         stateStore |> RxValue.update(state => {...state, show: ! state.show})
       }
     )
  |> RxObservables.ignoreElements;

let greetingStateComponent =
  RxReactActionStateComponent.create(
    ~name="GreetingStateComponent",
    ~mapStateToRenderProps,
    ~setupSideEffects,
    Components.greeting,
  );
let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    state := ! state^;
    actions
    |> RxEvent.trigger(Actions.SetGreeting(state^ ? "true" : "false"));
  },
  5000,
)
|> ignore;

ReactDom.renderToElementWithId(
  greetingStateComponent(~props={actions, stateStore}, ()),
  "index2",
);