module rec Component: {
  type t('props);

  let create:
    (
      ~name: string=?,
      ~areEqual: ('props, 'props) => bool=?,
      'props => Element.t
    ) =>
    Component.t('props);
}
and Element: {
  type t;

  let create:
    (Component.t('props), ~key: string=?, ~props: 'props, unit) => Element.t;

  [@bs.val] external null : Element.t = "null";
};

type dispose = unit => unit;

let useEffect: ((. unit) => dispose) => unit;
let useEffect1: ((. unit) => dispose, 'key) => unit;
let useEffect2: ((. unit) => dispose, 'key0, 'key1) => unit;

let useMemo: (unit => 't) => 't;
let useState: 'state => ('state, 'state => unit);