type action('state);
type state('props, 'state);

let make:
  (
    ~name: string,
    ~createStore: RxReason.Observable.t('props) => RxReason.Observable.t('state),
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
