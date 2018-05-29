type state('props);
type action;

type t('props) =
  'props =>
  ReasonReact.componentSpec(
    state('props),
    state('props),
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action,
  );

let create:
  (
    ~name: string,
    ~state: Rx.Observable.t('props) => Rx.Observable.t('state),
    ~render: 'state => ReasonReact.reactElement,
  ) =>
  t('props);