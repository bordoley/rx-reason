let div = ReactDomElement.div;
let button = ReactDomElement.button;
let span = ReactDomElement.span;
let string = ReactDom.string;

let optional =
  React.Component.createReasonComponent(
    ~name="Optional", (~props: bool, ~children: array(React.Element.t)) =>
    props ? React.Element.array(children) : React.Element.null
  );

let greeting =
  React.Component.createReasonComponent(
    ~name="Greeting",
    (
      ~props as {count, greeting, incrementCount, show, toggle}: Store.props,
      ~children as _: unit,
    ) =>
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
      optional(~props=show, [|string(greeting)|]),
    |])
  );