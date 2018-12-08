let div = BsReactDom.div;
let button = BsReactDom.button;
let span = BsReactDom.span;
let string = BsReactDom.string;
let textarea = BsReactDom.textarea;

let optional =
  BsReact.createComponent(
    ~name="Optional", (~props: bool, ~children: array(BsReact.Element.t)) =>
    props ? BsReact.Element.array(children) : BsReact.Element.null
  );

let context = BsReact.Context.create("book");
let contextComponent =
  BsReact.createComponentWithDefaultProps(
    ~name="contextComponent",
    ~defaultProps=(),
    (~props as _: unit, ~children as _: unit) => {
      let contextValue = BsReact.useContext(context);

      div([|string(contextValue)|]);
    },
  );

let refTest =
  BsReact.createComponentWithDefaultProps(
    ~name="refTest",
    ~defaultProps=(),
    (~props as _: unit, ~children as _: unit) => {
      let ref = BsReact.useRef(None);
      let onClick = _ => Js.log(ref |> BsReact.Ref.currentGet);

      button(
        ~ref,
        ~props=BsReactDomProps.t(~onClick, ()),
        [|string("ref button")|],
      );
    },
  );

let focusableTextArea =
  textarea
  |> BsRxReact.useImperativeMethods((ele, ()) =>
       switch (Webapi.Dom.HtmlElement.ofElement(ele)) {
       | Some(ele) => ele |> Webapi.Dom.HtmlElement.focus
       | _ => ()
       }
     );

let rxRefTest =
  BsReact.createComponentWithDefaultProps(
    ~name="RxRefTest",
    ~defaultProps=(),
    (~props as _: unit, ~children as _: unit) => {
      let event = BsReact.useMemo(RxEvent.create);
      let onClick =
        BsReact.useMemo1((event, _) => event |> RxEvent.dispatch(), event);

      div([|
        button(
          ~props=BsReactDomProps.t(~onClick, ()),
          [|string("focus clicker")|],
        ),
        focusableTextArea(~commands=RxEvent.asObservable(event), [||]),
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
  BsReact.createComponent(
    ~name="Greeting",
    (
      ~props as {count, greeting, incrementCount, show, toggle}: greetingProps,
      ~children as _: unit,
    ) => {
    let incrementCount =
      BsReact.useMemo1(
        (incrementCount, _) => incrementCount(),
        incrementCount,
      );

    let toggle = BsReact.useMemo1((toggle, _) => toggle(), toggle);

    div([|
      refTest(),
      contextComponent(),
      button(
        ~props=BsReactDomProps.t(~onClick=incrementCount, ()),
        [|
          string(
            "You've clicked this " ++ string_of_int(count) ++ " times(s)",
          ),
        |],
      ),
      button(
        ~props=BsReactDomProps.t(~onClick=toggle, ()),
        [|string("Toggle greeting")|],
      ),
      optional(~props=show, [|string(greeting)|]),
      rxRefTest(),
    |]);
  });