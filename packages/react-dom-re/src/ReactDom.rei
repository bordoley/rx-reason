let createElement:
  (string, ~key: string=?, ~props: ReactDomProps.t=?, array(React.Element.t)) =>
  React.Element.t;

external string : string => React.Element.t = "%identity";

[@bs.val] [@bs.module "react-dom"]
external render : (React.Element.t, Dom.element) => unit = "render";

let renderToElementWithId: (React.Element.t, string) => unit;