module Component = {
  type t('props);
};

module Element = {
  type t;
};

[@bs.val] [@bs.module "react"]
external reactCreateElement :
  (Component.t('props), ~props: Js.t({..})=?) => Element.t =
  "createElement";
let createElement =
    (component: Component.t('props), props: 'props)
    : Element.t =>
  reactCreateElement(component, ~props={"reasonProps": props});

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
external reactUseEffect : ((. unit) => dispose, array('key)) => unit =
  "useEffect";
let useEffect = (generator: (. unit) => dispose, key) =>
  reactUseEffect(generator, [|key|]);

[@bs.val] [@bs.module "react"]
external reactUseMemo : (unit => 't, array('key)) => 't = "useMemo";
let useMemo = (generator: unit => 't, key: 'key) =>
  reactUseMemo(generator, [|key|]);

[@bs.val] [@bs.module "react"]
external reactUseState : 'state => ('state, 'state => unit) = "useState";
let useState = reactUseState;

external string : string => Element.t = "%identity";