let module StatefulComponent = RxReasonReact.Component.Make({
  type props = string;
  type state = Store.State.t;

  let name = "RxReasonReactExampleContainer";
  let createStore = Store.create; 
  let render = (~state as {count, greeting, incrementCount, show, toggle}: Store.State.t, _) =>
    <GreetingComponent count greeting incrementCount show toggle />;
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
)|> ignore;