let module StatefulComponent = RxReasonReact.Component.Make({
  type props = string;
  type state = Store.State.t;

  let name = "RxReasonReactExampleContainer";
  let createStore = Store.create; 
  let render = (~state as {count, greeting, incrementCount, show, toggle}: Store.State.t, _) =>
    <GreetingComponent count greeting incrementCount show toggle />;
});

ReactDOMRe.renderToElementWithId(<StatefulComponent props="Hello!" />, "index2");
