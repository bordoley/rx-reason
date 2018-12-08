type t =
  (
    ~key: string=?,
    ~ref: BsReact.Ref.t(Dom.element)=?,
    ~props: BsReactDomProps.t=?,
    array(BsReact.Element.t)
  ) =>
  BsReact.Element.t;

let create = tag =>
  BsReact.wrapJsComponentWithDefaultProps(
    BsReact.JsComponent.cast(tag),
    ~defaultProps=BsReactDomProps.default,
  );