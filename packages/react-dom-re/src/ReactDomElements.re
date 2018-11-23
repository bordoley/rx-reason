let buildDomFunction = (tag, ~key=?, ~props=ReactDomProps.default, children) =>
  ReactDom.createElement(tag, ~key?, ~props, children);

let a = buildDomFunction("a");
let button = buildDomFunction("button");
let div = buildDomFunction("div");
let h1 = buildDomFunction("h1");
let h2 = buildDomFunction("h2");
let h3 = buildDomFunction("h3");
let h4 = buildDomFunction("h4");
let h5 = buildDomFunction("h5");
let h6 = buildDomFunction("h6");
let img = buildDomFunction("img");
let input = buildDomFunction("input");
let label = buildDomFunction("label");
let li = buildDomFunction("li");
let ol = buildDomFunction("ol");
let span = buildDomFunction("span");
let table = buildDomFunction("table");
let td = buildDomFunction("td");
let tfoot = buildDomFunction("tfoot");
let th = buildDomFunction("th");
let thead = buildDomFunction("thead");
let ul = buildDomFunction("ul");