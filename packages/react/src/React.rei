module rec Component: {
  type t('props, 'children);

  let createReactComponent:
    (
      ~name: string=?,
      ~arePropsEqual: ('props, 'props) => bool=?,
      ~areChildrenEqual: ('children, 'children) => bool=?,
      (~props: 'props, ~children: 'children) => Element.t
    ) =>
    Component.t('props, 'children);

  let createReasonComponent:
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

  external array : array(t) => t = "%identity";
  [@bs.val] external null : Element.t = "null";
};

type dispose = unit => unit;

let useEffect: (unit => dispose) => unit;
let useEffect1: (unit => dispose, 'key) => unit;
let useEffect2: (unit => dispose, 'key0, 'key1) => unit;

let useMemo: (unit => 't) => 't;
let useState: 'state => ('state, 'state => unit);