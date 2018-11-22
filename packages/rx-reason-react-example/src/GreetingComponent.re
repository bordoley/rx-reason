

let render =
  ({count, greeting, incrementCount, show, toggle}: Store.props) => {
    let message =
      React.string(
        "You've clicked this " ++ string_of_int(count) ++ " times(s)",
      );

    ReactDom.div(
      ~children=[|
        ReactDom.button(
          ~props=ReactDom.props(~onClick=incrementCount, ()),
          ~children=[|message|],
          (),
        ),
        ReactDom.button(
          ~props=ReactDom.props(~onClick=toggle, ()),
          ~children=[|React.string("Toggle greeting")|],
          (),
        ),
        show ? React.string(greeting) : React.null,
      |],
      (),
    );
  };

  let component = RxReact.createComponent(
    ~createStateStream=Store.create,
    render,
  );