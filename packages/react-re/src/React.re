module Component = {
  type t('props);
};

module Element = {
  type t;
};

[@bs.val] [@bs.module "react"]
external reactCreateElement : (Component.t('props), Js.t({..})) => Element.t =
  "createElement";
let createElement =
    (component: Component.t('props), props: 'props)
    : Element.t =>
  reactCreateElement(component, {"reasonProps": props});

[@bs.val] [@bs.module "react"]
external reactCreateElementWithChild :
  (Component.t('props), Js.t({..}), Element.t) => Element.t =
  "createElement";
let createElementWithChild =
    (component: Component.t('props), props: 'props, child: Element.t)
    : Element.t =>
  reactCreateElementWithChild(component, {"reasonProps": props}, child);

[@bs.val] [@bs.module "react"]
external reactCreateElementWithChildren :
  (Component.t('props), Js.t({..}), array(Element.t)) => Element.t =
  "createElement";
let createElementWithChildren =
    (
      component: Component.t('props),
      props: 'props,
      children: array(Element.t),
    )
    : Element.t => {
  let childrenLength = Js.Array.length(children);

  switch (childrenLength) {
  | 0 => createElement(component, props)
  | 1 => createElementWithChild(component, props, children[0])
  | _ =>
    reactCreateElementWithChildren(
      component,
      {"reasonProps": props},
      children,
    )
  };
};

let defaultShouldUpdate =
  (. a: Js.t({..}), b: Js.t({..})) => a##reasonProps === b##reasonProps;

[@bs.val] [@bs.module "react"]
external reactMemo :
  (Js.t({..}) => Element.t, (. Js.t({..}), Js.t({..})) => bool) =>
  Component.t('props) =
  "memo";
let memo = (f: 'props => Element.t) : Component.t('props) =>
  reactMemo(
    props => {
      let props = props##reasonProps;
      f(props);
    },
    defaultShouldUpdate,
  );

[@bs.val] external null : Element.t = "null";

type dispose = unit => unit;

[@bs.val] [@bs.module "react"]
external reactUseEffect : ((. unit) => dispose) => unit = "useEffect";
let useEffect = (generator: (. unit) => dispose) =>
  reactUseEffect(generator);

[@bs.val] [@bs.module "react"]
external reactUseEffect1 : ((. unit) => dispose, array('key)) => unit =
  "useEffect";
let useEffect1 = (generator: (. unit) => dispose, key) =>
  reactUseEffect1(generator, [|key|]);

[@bs.val] [@bs.module "react"]
external reactUseEffect2 : ((. unit) => dispose, ('k0, 'k1)) => unit =
  "useEffect";
let useEffect2 = (generator: (. unit) => dispose, k0, k1) =>
  reactUseEffect2(generator, (k0, k1));

[@bs.val] [@bs.module "react"]
external reactUseMemo : (unit => 't, array('key)) => 't = "useMemo";
let useMemo = (generator: unit => 't, key: 'key) =>
  reactUseMemo(generator, [|key|]);

[@bs.val] [@bs.module "react"]
external reactUseState : 'state => ('state, 'state => unit) = "useState";
let useState = reactUseState;

external string : string => Element.t = "%identity";