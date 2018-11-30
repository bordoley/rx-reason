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

let greetingStateComponent =
  RxReactOld.createComponent(
    ~name="GreetingStateComponent",
    ~createStateStream=Store.create,
    greeting,
  );

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    ReactDom.renderToElementWithId(
      greetingStateComponent(~props=state^ ? "true" : "false", ()),
      "index2",
    );
    state := state^ |> (!);
  },
  5000,
)
|> ignore;