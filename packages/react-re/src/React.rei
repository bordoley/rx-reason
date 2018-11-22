module Component: {type t('props);};

module Element: {type t;};

let createElement: (Component.t('props), 'props) => Element.t;
let createElementWithChild:
  (Component.t('props), 'props, Element.t) => Element.t;
let createElementWithChildren:
  (Component.t('props), 'props, array(Element.t)) => Element.t;

let memo: ('props => Element.t) => Component.t('props);
[@bs.val] external null : Element.t = "null";

type dispose = unit => unit;
external string : string => Element.t = "%identity";

let useEffect: ((. unit) => dispose) => unit;
let useEffect1: ((. unit) => dispose, 'key) => unit;
let useMemo: (unit => 't, 'key) => 't;
let useState: 'state => ('state, 'state => unit);