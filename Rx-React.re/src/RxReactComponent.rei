type state('props, 'state);
type action('state);

let make:
  (
    ~name: string,
    ~state: Rx.Observable.t('props) => Rx.Observable.t('state),
    ~initialState: 'state,
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
