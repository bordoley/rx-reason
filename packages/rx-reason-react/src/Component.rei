module type RxReasonReactComponentSpec = {
  type props;
  type state;

  let name: string;
  let createStore: RxReason.Observable.t(props) => RxReason.Observable.t(state);
  let render: (~state: state, array(ReasonReact.reactElement)) => ReasonReact.reactElement;

};

module type RxReasonReactComponent = {
  type props;
  type state;
  type action;

  let make: (~props: props, array(ReasonReact.reactElement)) => ReasonReact.componentSpec(
    state,
    state,
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action,
  )
};

module Make: (ComponentSpec: RxReasonReactComponentSpec) => (RxReasonReactComponent with type props := ComponentSpec.props);