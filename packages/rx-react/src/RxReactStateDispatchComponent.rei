module Dispatcher: {type t('action) = 'action => unit;};

module Props: {
  type t('state, 'action) = {
    stateStream: RxObservable.t('state),
    dispatch: Dispatcher.t('action),
  };
};

let createReactComponent:
  (
    ~name: string=?,
    ~renderDefault: (~key: string=?, ~props: unit, 'children) =>
                    React.Element.t
                      =?,
    ~renderExn: (~key: string=?, ~props: exn, 'children) => React.Element.t=?,
    ~render: (
               ~key: string=?,
               ~props: ('state, Dispatcher.t('action)),
               'children
             ) =>
             React.Element.t
  ) =>
  React.Component.t(Props.t('state, 'action), 'children);

let create:
  (
    ~name: string=?,
    ~renderDefault: (~key: string=?, ~props: unit, 'a) => React.Element.t=?,
    ~renderExn: (~key: string=?, ~props: exn, 'a) => React.Element.t=?,
    ~render: (~key: string=?, ~props: ('b, Dispatcher.t('c)), 'a) =>
             React.Element.t,
    unit,
    ~key: string=?,
    ~props: Props.t('b, 'c),
    'a
  ) =>
  React.Element.t;