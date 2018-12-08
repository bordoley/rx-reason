module rec Component: {
  type t('props, 'children);

  let create:
    (
      ~name: string=?,
      ~arePropsEqual: ('props, 'props) => bool=?,
      ~areChildrenEqual: ('children, 'children) => bool=?,
      (~props: 'props, ~children: 'children) => Element.t
    ) =>
    Component.t('props, 'children);
}
and Element: {
  type t;

  let create:
    (
      Component.t('props, 'children),
      ~key: string=?,
      ~props: 'props,
      'children
    ) =>
    Element.t;

  external array: array(t) => t = "%identity";
  [@bs.val] external null: Element.t = "null";
};

module JsComponent: {
  type t('props);

  external cast: 'a => t('props) = "%identity";
};

module Ref: {
  [@bs.deriving abstract]
  type t('a) = {current: option('a)};
};

let createComponent:
  (
    ~name: string=?,
    ~arePropsEqual: ('props, 'props) => bool=?,
    ~areChildrenEqual: ('children, 'children) => bool=?,
    (~props: 'props, ~children: 'children) => Element.t,
    ~key: string=?,
    ~props: 'props,
    'children
  ) =>
  Element.t;

let createComponentWithDefaultProps:
  (
    ~name: string=?,
    ~arePropsEqual: ('props, 'props) => bool=?,
    ~areChildrenEqual: ('children, 'children) => bool=?,
    ~defaultProps: 'props,
    (~props: 'props, ~children: 'children) => Element.t,
    ~key: string=?,
    ~props: 'props=?,
    'children
  ) =>
  Element.t;

let wrapJsComponent:
  (
    JsComponent.t('props),
    ~key: string=?,
    ~ref: Ref.t('a)=?,
    ~props: 'props,
    array(Element.t)
  ) =>
  Element.t;

let wrapJsComponentWithDefaultProps:
  (
    JsComponent.t('props),
    ~defaultProps: 'props,
    ~key: string=?,
    ~ref: Ref.t('a)=?,
    ~props: 'props=?,
    array(Element.t)
  ) =>
  Element.t;

module Context: {
  type t('a);

  [@bs.val] [@bs.module "react"]
  external create: 'a => t('a) = "createContext";
};

let null: (~key: 'a=?, ~props: 'b, 'c) => Element.t;
let raise: (~key: 'a=?, ~props: exn, 'c) => Element.t;

[@bs.val] [@bs.module "react"]
external useContext: Context.t('a) => 'a = "useContext";

let useEffect: (unit => unit) => unit;
let useEffect1: ('key => unit, 'key) => unit;
let useEffect2: (('key0, 'key1) => unit, 'key0, 'key1) => unit;
let useEffect3: (('key0, 'key1, 'key2) => unit, 'key0, 'key1, 'key2) => unit;

type cleanup = unit => unit;
let useEffectWithCleanup: (unit => cleanup) => unit;
let useEffectWithCleanup1: ('key => cleanup, 'key) => unit;
let useEffectWithCleanup2: (('key0, 'key1) => cleanup, 'key0, 'key1) => unit;
let useEffectWithCleanup3:
  (('key0, 'key1, 'key2) => cleanup, 'key0, 'key1, 'key2) => unit;
let useEffectWithCleanup4:
  (('key0, 'key1, 'key2, 'key3) => cleanup, 'key0, 'key1, 'key2, 'key3) => unit;

let useMemo: (unit => 't) => 't;
let useMemo1: ('a => 't, 'a) => 't;
let useMemo2: (('a, 'b) => 't, 'a, 'b) => 't;
let useMemo3: (('a, 'b, 'c) => 't, 'a, 'b, 'c) => 't;
let useMemo4: (('a, 'b, 'c, 'd) => 't, 'a, 'b, 'c, 'd) => 't;

[@bs.val] [@bs.module "react"]
external useRef: option('a) => Ref.t('a) = "useRef";

let useState: 'state => ('state, 'state => unit);