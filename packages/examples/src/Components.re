let div = ReactDom.div;
let button = ReactDom.button;
let span = ReactDom.span;
let string = ReactDom.string;
let textarea = ReactDom.textarea;

let optional =
  React.createComponent(
    ~name="Optional", (~props: bool, ~children: array(React.Element.t)) =>
    props ? React.Element.array(children) : React.Element.null
  );

let context = React.Context.create("book");
let contextComponent =
  React.createComponentWithDefaultProps(
    ~name="contextComponent",
    ~defaultProps=(),
    (~props as _: unit, ~children as _: unit) => {
      let contextValue = React.useContext(context);

      div([|string(contextValue)|]);
    },
  );

let refTest =
  React.createComponentWithDefaultProps(
    ~name="refTest",
    ~defaultProps=(),
    (~props as _: unit, ~children as _: unit) => {
      let ref = React.useRef(None);
      let onClick = _ => Js.log(ref |> React.Ref.currentGet);

      button(
        ~ref,
        ~props=ReactDomProps.t(~onClick, ()),
        [|string("ref button")|],
      );
    },
  );

let rxRefTest =
  React.createComponentWithDefaultProps(
    ~name="RxRefTest",
    ~defaultProps=(),
    (~props as _: unit, ~children as _: unit) => {
      let event = React.useMemo(RxEvent.create);

      let textAreaRef =
        RxReactDomHtmlElement.useRef(event |> RxEvent.asObservable);

      div([|
        button(
          ~props=
            ReactDomProps.t(
              ~onClick=
                _ =>
                  event |> RxEvent.trigger(RxReactDomHtmlElement.Action.Focus),
              (),
            ),
          [|string("focus clicker")|],
        ),
        textarea(~ref=textAreaRef, [||]),
      |]);
    },
  );

type greetingProps = {
  count: int,
  greeting: string,
  incrementCount: unit => unit,
  show: bool,
  toggle: unit => unit,
};

let greeting =
  React.createComponent(
    ~name="Greeting",
    (
      ~props as {count, greeting, incrementCount, show, toggle}: greetingProps,
      ~children as _: unit,
    ) => {
    let incrementCount =
      React.useMemo1(
        (incrementCount, _) => incrementCount(),
        incrementCount,
      );

    let toggle = React.useMemo1((toggle, _) => toggle(), toggle);

    div([|
      refTest(),
      contextComponent(),
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
      rxRefTest(),
    |]);
  });