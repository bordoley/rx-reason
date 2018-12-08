[@bs.val] [@bs.module "react-dom"]
external render : (BsReact.Element.t, Dom.element) => unit = "render";

[@bs.val] [@bs.return nullable]
external _getElementById : string => option(Dom.element) =
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

let makeReactProps = (key, ref, props: BsReactDomProps.t) => {
  let keyObj = {"key": key, "ref": ref};
  Js.Obj.assign(Js.Obj.assign(Js.Obj.empty(), Obj.magic(props)), keyObj);
};

[@bs.val] [@bs.module "react"]
external reactCreateElementWithChildren :
  (string, ~props: Js.t({..}), array(BsReact.Element.t)) => BsReact.Element.t =
  "createElement";

[@bs.val] [@bs.module "react"]
external reactCreateElementWithChildrenVariadic : 'a = "createElement";

let createElement =
    (
      tag: string,
      ~key: option(string)=?,
      ~ref: option(BsReact.Ref.t(Dom.element))=?,
      ~props=BsReactDomProps.default,
      children: array(BsReact.Element.t),
    )
    : BsReact.Element.t => {
  let childrenLength = Js.Array.length(children);
  let jsProps = makeReactProps(key, ref, props);

  switch (childrenLength) {
  | _ when childrenLength <= 10 =>
    /* Suppress missing key warnings in the common case. */
    let vararg =
      [|Obj.magic(tag), Obj.magic(jsProps)|] |> Js.Array.concat(children);
    Obj.magic(reactCreateElementWithChildrenVariadic)##apply(
      Js.Nullable.null,
      vararg,
    );
  | _ =>
    reactCreateElementWithChildren(
      tag,
      ~props=jsProps,
      children,
    )
  };
};

external string : string => BsReact.Element.t = "%identity";

let a = createElement("a");
let button = createElement("button");
let div = createElement("div");
let h1 = createElement("h1");
let h2 = createElement("h2");
let h3 = createElement("h3");
let h4 = createElement("h4");
let h5 = createElement("h5");
let h6 = createElement("h6");
let img = createElement("img");
let input = createElement("input");
let label = createElement("label");
let li = createElement("li");
let ol = createElement("ol");
let span = createElement("span");
let table = createElement("table");
let td = createElement("td");
let textarea = createElement("textarea");
let tfoot = createElement("tfoot");
let th = createElement("th");
let thead = createElement("thead");
let ul = createElement("ul");