type component('props, 'children);
type element;

module Element = {
  type t = element;

  let makeReactProps =
      (key: option(string), props: 'props, children: 'children)
      : Js.t({..}) => {
    "key": key,
    "reasonProps": props,
    "reasonChildren": children,
  };

  [@bs.val] [@bs.module "react"]
  external reactCreateElement :
    (component('props, 'children), Js.t({..})) => t =
    "createElement";

  let create =
      (
        component: component('props, 'children),
        ~key: option(string)=?,
        ~props: 'props,
        children,
      )
      : t =>
    reactCreateElement(component, makeReactProps(key, props, children));

  external array : array(t) => t = "%identity";
  [@bs.val] external null : t = "null";
};

module Component = {
  type t('props, 'children) = component('props, 'children);
  [@bs.val]
  external defineProperty : ('a, string, Js.t({..})) => unit =
    "Object.defineProperty";

  external reactCreateComponent :
    (Js.t({..}) => element) => t('props, 'children) =
    "%identity";

  let createComponent =
      (
        ~name: option(string)=?,
        f: (~props: 'props, ~children: 'children) => element,
      )
      : t('props, 'children) => {
    let component = reactProps => {
      let props = reactProps##reasonProps;
      let children = reactProps##reasonChildren;
      f(~props, ~children);
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
    (t('props, 'children), (Js.t({..}), Js.t({..})) => bool) =>
    t('props, 'children) =
    "memo";

  let reasonPropsAndChildrenAreReferenceEqual =
      (a: Js.t({..}), b: Js.t({..})) =>
    a##reasonProps === b##reasonProps
    && a##reasonChildren === b##reasonChildren;

  let referenceEquality = (a, b) => a === b;

  let createReactComponent =
      (
        ~name: option(string)=?,
        ~arePropsEqual: ('props, 'props) => bool=referenceEquality,
        ~areChildrenEqual: ('children, 'children) => bool=referenceEquality,
        f: (~props: 'props, ~children: 'children) => element,
      )
      : t('props, 'children) => {
    let areEqual =
      arePropsEqual === referenceEquality
      && areChildrenEqual === referenceEquality ?
        reasonPropsAndChildrenAreReferenceEqual :
        (
          (a: Js.t({..}), b: Js.t({..})) =>
            arePropsEqual(
              Obj.magic(a##reasonProps),
              Obj.magic(b##reasonProps),
            )
            && areChildrenEqual(
                 Obj.magic(a##reasonChildren),
                 Obj.magic(b##reasonChildren),
               )
        );
    let component = createComponent(~name?, f);
    reactMemo(component, areEqual);
  };

  let createReasonComponent =
      (
        ~name: option(string)=?,
        ~arePropsEqual: ('props, 'props) => bool=referenceEquality,
        ~areChildrenEqual: ('children, 'children) => bool=referenceEquality,
        f: (~props: 'props, ~children: 'children) => element,
      ) => {
    let component =
      createReactComponent(~name?, ~arePropsEqual, ~areChildrenEqual, f);
    Element.create(component);
  };
};

type dispose = unit => unit;

[@bs.val] [@bs.module "react"]
external reactUseEffect : (unit => dispose) => unit = "useEffect";
let useEffect = (generator: unit => dispose) => reactUseEffect(generator);

[@bs.val] [@bs.module "react"]
external reactUseEffect1 : (unit => dispose, array('key)) => unit =
  "useEffect";
let useEffect1 = (generator: unit => dispose, key) =>
  reactUseEffect1(generator, [|key|]);

[@bs.val] [@bs.module "react"]
external reactUseEffect2 : (unit => dispose, ('k0, 'k1)) => unit =
  "useEffect";
let useEffect2 = (generator: unit => dispose, k0, k1) =>
  reactUseEffect2(generator, (k0, k1));

[@bs.val] [@bs.module "react"]
external reactUseMemo : (unit => 't) => 't = "useMemo";
let useMemo = (generator: unit => 't) => reactUseMemo(generator);

[@bs.val] [@bs.module "react"]
external reactUseState : 'state => ('state, 'state => unit) = "useState";
let useState = reactUseState;