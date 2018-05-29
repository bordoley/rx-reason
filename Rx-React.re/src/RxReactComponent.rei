type state('props);
type action;

let make:
  (
    ~name: string,
    ~state: Rx.Observable.t('props) => Rx.Observable.t('state),
    ~render: 'state => ReasonReact.reactElement,
    ~props: 'props,
    array(ReasonReact.reactElement),
  ) => ReasonReact.componentSpec(
    state('props),
    state('props),
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action,
  );
