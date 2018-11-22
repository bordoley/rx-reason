let buildDomFunction = (tag, ~props=ReactDomProps.default, children) =>
  React.createElementWithChildren(ReactDom.component(tag), props, children);

let button = buildDomFunction("button");
let div = buildDomFunction("div");
let span = buildDomFunction("span");