type component('props);
type element;

module Component = {
  type t('props) = component('props);
  [@bs.val]
  external defineProperty :
    (Js.t({..}) => element, string, Js.t({..})) => unit =
    "Object.defineProperty";

  external reactCreateComponent : (Js.t({..}) => element) => t('props) =
    "%identity";

  let createComponent =
      (~name: option(string)=?, f: 'props => element)
      : t('props) => {
    let component = props => {
      let props = props##reasonProps;
      f(props);
    };

    switch (name) {
    | Some(name) =>
      defineProperty(component, "name", {"value": name, "writable": false})
    | _ => ()
    };

    reactCreateComponent(component);
  };

  [@bs.val] [@bs.module "react"]
  external reactMemo :
    (t('props), (. Js.t({..}), Js.t({..})) => bool) => t('props) =
    "memo";

  let referenceEquality = (a, b) => a === b;
  let reasonPropsAreReferenceEqual =
    (. a: Js.t({..}), b: Js.t({..})) => a##reasonProps === b##reasonProps;

  let create =
      (
        ~name: option(string)=?,
        ~areEqual: ('props, 'props) => bool=referenceEquality,
        f: 'props => element,
      )
      : t('props) => {
    let areEqual =
      areEqual === referenceEquality ?
        reasonPropsAreReferenceEqual :
        (
          (. a: Js.t({..}), b: Js.t({..})) =>
            areEqual(Obj.magic(a##reasonProps), Obj.magic(b##reasonProps))
        );
    let component = createComponent(~name?, f);
    reactMemo(component, areEqual);
  };
};

module Element = {
  type t = element;

  let makeReactProps = (key: option(string), props: 'props) : Js.t({..}) => {
    "key": key,
    "reasonProps": props,
  };

  [@bs.val] [@bs.module "react"]
  external reactCreateElement : (Component.t('props), Js.t({..})) => t =
    "createElement";
  let create =
      (
        component: Component.t('props),
        ~key: option(string)=?,
        ~props: 'props,
        (),
      )
      : t =>
    reactCreateElement(component, makeReactProps(key, props));

  [@bs.val] external null : t = "null";
};

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
external reactUseMemo : (unit => 't) => 't = "useMemo";
let useMemo = (generator: unit => 't) => reactUseMemo(generator);

[@bs.val] [@bs.module "react"]
external reactUseState : 'state => ('state, 'state => unit) = "useState";
let useState = reactUseState;