let div = ReactDom.div;
let button = ReactDom.button;
let span = ReactDom.span;
let string = ReactDom.string;

let optional =
  React.Component.createReasonComponent(
    ~name="Optional", (~props: bool, ~children: array(React.Element.t)) =>
    props ? React.Element.array(children) : React.Element.null
  );

let context = React.Context.create("book");
let contextComponent =
  React.Component.createReasonComponent(
    ~name="contextComponent",
    (~props as _: unit, ~children as _: array(unit)) => {
    let contextValue = React.useContext(context);

    div([|string(contextValue)|]);
  });

let refTest =
  React.Component.createReasonComponent(
    ~name="refTest", (~props as _: unit, ~children as _: array(unit)) => {
    let ref = React.useRef(None);
    let onClick = _ => Js.log(ref |> React.Ref.currentGet);

    button(
      ~ref,
      ~props=ReactDomProps.t(~onClick, ()),
      [|string("ref button")|],
    );
  });

let greeting =
  React.Component.createReasonComponent(
    ~name="Greeting",
    (
      ~props as {count, greeting, incrementCount, show, toggle}: Store.props,
      ~children as _: unit,
    ) =>
    div([|
      refTest(~props=(), [||]),
      contextComponent(~props=(), [||]),
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