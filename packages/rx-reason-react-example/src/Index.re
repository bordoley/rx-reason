let greeting = Greeting.createElement;

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

let statefulComponent = 
  RxReact.createComponent(~createStateStream=Store.create, greeting);
let createStatefulElement = React.createElement(statefulComponent);

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    ReactDom.renderToElementWithId(
      createStatefulElement(state^ ? "true" : "false"),
      "index2"
    );
    state := state^ |> (!);
  },
  5000,
)
|> ignore;