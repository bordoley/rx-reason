let component = ReasonReact.statelessComponent("GreetingComponent");
let make = (~count, ~greeting, ~incrementCount, ~show, ~toggle, _) => {
  ...component,
  render: _ => {
    let message =
      "You've clicked this " ++ string_of_int(count) ++ " times(s)";
    <div>
      <button onClick=incrementCount>
        (ReasonReact.string(message))
      </button>
      <button onClick=toggle>
        (ReasonReact.string("Toggle greeting"))
      </button>
      (show ? ReasonReact.string(greeting) : ReasonReact.null)
    </div>;
  },
};

