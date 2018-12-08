[@bs.val] [@bs.module "react-dom"]
external render: (BsReact.Element.t, Dom.element) => unit = "render";

[@bs.val] [@bs.return nullable]
external _getElementById: string => option(Dom.element) =
  "document.getElementById";

let renderToElementWithId = (reactElement, id) =>
  switch (_getElementById(id)) {
  | None =>
    raise(
      Invalid_argument(
        "BsReactDom.renderToElementWithId : no element of id "
        ++ id
        ++ " found in the HTML.",
      ),
    )
  | Some(element) => render(reactElement, element)
  };

external string: string => BsReact.Element.t = "%identity";

let a = BsReactDomComponent.create("a");
let button = BsReactDomComponent.create("button");
let div = BsReactDomComponent.create("div");
let h1 = BsReactDomComponent.create("h1");
let h2 = BsReactDomComponent.create("h2");
let h3 = BsReactDomComponent.create("h3");
let h4 = BsReactDomComponent.create("h4");
let h5 = BsReactDomComponent.create("h5");
let h6 = BsReactDomComponent.create("h6");
let img = BsReactDomComponent.create("img");
let input = BsReactDomComponent.create("input");
let label = BsReactDomComponent.create("label");
let li = BsReactDomComponent.create("li");
let ol = BsReactDomComponent.create("ol");
let span = BsReactDomComponent.create("span");
let table = BsReactDomComponent.create("table");
let td = BsReactDomComponent.create("td");
let textarea = BsReactDomComponent.create("textarea");
let tfoot = BsReactDomComponent.create("tfoot");
let th = BsReactDomComponent.create("th");
let thead = BsReactDomComponent.create("thead");
let ul = BsReactDomComponent.create("ul");