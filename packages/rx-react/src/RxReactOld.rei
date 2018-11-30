let createComponent:
  (
    ~name: string=?,
    ~createSideEffectsStream: RxObservable.t('props) => RxObservable.t(unit)=?,
    ~createStateStream: RxObservable.t('props) => RxObservable.t('state)=?,
    (~key: string=?, ~props: 'state, 'children) => React.Element.t,
    ~key: string=?,
    ~props: 'props,
    'children
  ) =>
  React.Element.t;