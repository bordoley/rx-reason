type action('state);
type state('props, 'state);

let module Rx = RxReason;

let make:
  (
    ~name: string,
    ~createStore: Rx.Observable.t('props) => Rx.Observable.t('state),
    ~render: (~props: 'state, array(ReasonReact.reactElement)) => ReasonReact.reactElement,
    ~props: 'props,
    array(ReasonReact.reactElement),
  ) => ReasonReact.componentSpec(
    state('props, 'state),
    state('props, 'state),
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action('state),
  );
