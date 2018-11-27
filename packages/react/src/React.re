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

[@bs.val] [@bs.module "react"]
external reactUseEffect : (unit => Js.null(unit)) => unit = "useEffect";
let useEffect = generator =>
  reactUseEffect(() => {
    generator();
    Js.Null.empty;
  });

[@bs.val] [@bs.module "react"]
external reactUseEffect1 : (unit => Js.null(unit), array('key)) => unit =
  "useEffect";
let useEffect1 = (generator, key) =>
  reactUseEffect1(
    () => {
      generator(key);
      Js.Null.empty;
    },
    [|key|],
  );

[@bs.val] [@bs.module "react"]
external reactUseEffect2 : (unit => Js.null(unit), ('k0, 'k1)) => unit =
  "useEffect";
let useEffect2 = (generator, k0, k1) =>
  reactUseEffect2(
    () => {
      generator(k0, k1);
      Js.Null.empty;
    },
    (k0, k1),
  );

[@bs.val] [@bs.module "react"]
external reactUseEffect3 : (unit => Js.null(unit), ('k0, 'k1, 'k2)) => unit =
  "useEffect";
let useEffect3 = (generator, k0, k1, k2) =>
  reactUseEffect3(
    () => {
      generator(k0, k1, k2);
      Js.Null.empty;
    },
    (k0, k1, k2),
  );

type cleanup = unit => unit;

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup : (unit => cleanup) => unit = "useEffect";
let useEffectWithCleanup = generator => reactUseEffectWithCleanup(generator);

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup1 : (unit => cleanup, array('key)) => unit =
  "useEffect";
let useEffectWithCleanup1 = (generator, key) =>
  reactUseEffectWithCleanup1(() => generator(key), [|key|]);

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup2 : (unit => cleanup, ('k0, 'k1)) => unit =
  "useEffect";
let useEffectWithCleanup2 = (generator, k0, k1) =>
  reactUseEffectWithCleanup2(() => generator(k0, k1), (k0, k1));

[@bs.val] [@bs.module "react"]
external reactUseEffectWithCleanup3 :
  (unit => cleanup, ('k0, 'k1, 'k2)) => unit =
  "useEffect";
let useEffectWithCleanup3 = (generator, k0, k1, k2) =>
  reactUseEffectWithCleanup3(() => generator(k0, k1, k2), (k0, k1, k2));

[@bs.val] [@bs.module "react"]
external reactUseMemo : (unit => 't) => 't = "useMemo";
let useMemo = (generator: unit => 't) => reactUseMemo(generator);

[@bs.val] [@bs.module "react"]
external reactUseMemo1 : (unit => 't, array('a)) => 't = "useMemo";
let useMemo1 = (generator: 'a => 't, ctx0: 'a) =>
  reactUseMemo1(() => generator(ctx0), [|ctx0|]);

[@bs.val] [@bs.module "react"]
external reactUseMemo2 : (unit => 't, ('a, 'b)) => 't = "useMemo";
let useMemo2 = (generator: ('a, 'b) => 't, ctx0: 'a, ctx1: 'b) =>
  reactUseMemo2(() => generator(ctx0, ctx1), (ctx0, ctx1));

[@bs.val] [@bs.module "react"]
external reactUseMemo3 : (unit => 't, ('a, 'b, 'c)) => 't = "useMemo";
let useMemo3 = (generator: ('a, 'b, 'c) => 't, ctx0: 'a, ctx1: 'b, ctx2: 'c) =>
  reactUseMemo3(() => generator(ctx0, ctx1, ctx2), (ctx0, ctx1, ctx2));

[@bs.val] [@bs.module "react"]
external reactUseState : 'state => ('state, 'state => unit) = "useState";
let useState = reactUseState;