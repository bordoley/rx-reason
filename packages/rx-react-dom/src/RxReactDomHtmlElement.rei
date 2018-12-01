module Action: {
  type t =
    | Blur
    | Click
    | Focus;
};

let update: (Dom.element, Action.t) => unit;

let useRef: RxObservable.t(Action.t) => React.Ref.t(Dom.element);