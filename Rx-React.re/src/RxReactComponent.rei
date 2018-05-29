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
    string,
    Rx.Observable.t('props) => Rx.Observable.t(ReasonReact.reactElement)
  ) =>
  t('props);