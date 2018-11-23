let div = ReactDomElements.div;
let button = ReactDomElements.button;
let span = ReactDomElements.span;
let string = ReactDom.string;

let render = ({count, greeting, incrementCount, show, toggle}: Store.props) =>
  div([|
    button(
      ~props=ReactDomProps.t(~onClick=incrementCount, ()),
      [|
        string(
          "You've clicked this " ++ string_of_int(count) ++ " times(s)",
        ),
      |],
    ),
    button(
      ~props=ReactDomProps.t(~onClick=toggle, ()),
      [|string("Toggle greeting")|],
    ),
    span([|show ? string(greeting) : React.Element.null|]),
  |]);

let component = React.Component.create(~name="Greeting", render);

let createElement = React.Element.create(component);