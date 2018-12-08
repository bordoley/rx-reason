type t =
  (
    ~key: string=?,
    ~ref: BsReact.Ref.t(Dom.element)=?,
    ~props: BsReactDomProps.t=?,
    array(BsReact.Element.t)
  ) =>
  BsReact.Element.t;

let create:
  (
    string,
    ~key: string=?,
    ~ref: BsReact.Ref.t(Dom.element)=?,
    ~props: BsReactDomProps.t=?,
    array(BsReact.Element.t)
  ) =>
  BsReact.Element.t;