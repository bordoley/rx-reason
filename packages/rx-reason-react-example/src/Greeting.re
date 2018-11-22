let div = ReactDomElements.div;
let button = ReactDomElements.button;
let string = React.string;

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
    show ? string(greeting) : React.null,
  |]);

let component = React.memo(render);

let createElement = React.createElement(component);