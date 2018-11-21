module StatefulComponent =
  RxReasonReact.Component.Make({
    type props = string;
    type state = Store.State.t;

    let name = "RxReasonReactExampleContainer";
    let createStore = Store.create;
    let render =
        (
          ~state as
            {count, greeting, incrementCount, show, toggle}: Store.State.t,
          _,
        ) =>
      <GreetingComponent count greeting incrementCount show toggle />;
  });

let promise: Js.Promise.t(int) = Js.Promise.resolve(1);
let observable = promise |> RxReasonJs.Promise.toObservable;
observable
|> RxReason.Observable.pipe2(
     RxReason.Operators.onNext(Js.log),
     RxReason.Operators.onComplete(Js.log),
   )
|> ignore;

let promise =
  observable
  |> RxReason.Observable.lift(RxReason.Operators.mapTo("b"))
  |> RxReasonJs.Promise.fromObservable
  |> Js.Promise.then_(a => {
       Js.log(a);
       Js.Promise.resolve();
     });

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    ReactDOMRe.renderToElementWithId(
      <StatefulComponent props=(state^ ? "true" : "false") />,
      "index2",
    );
    state := state^ |> (!);
  },
  5000,
)
|> ignore;