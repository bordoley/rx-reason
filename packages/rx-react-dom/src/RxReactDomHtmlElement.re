let optionDo = f =>
  fun
  | Some(opt) => f(opt)
  | _ => ();

let useBlur = {
  let blur = ((), ele) =>
    ele
    |> Webapi.Dom.HtmlElement.ofElement
    |> optionDo(Webapi.Dom.HtmlElement.blur);
  RxReact.useOnNextWithRef(blur);
};

let useClick = {
  let click = ((), ele) =>
    ele
    |> Webapi.Dom.HtmlElement.ofElement
    |> optionDo(Webapi.Dom.HtmlElement.click);
  RxReact.useOnNextWithRef(click);
};

let useFocus = {
  let focus = ((), ele) =>
    ele
    |> Webapi.Dom.HtmlElement.ofElement
    |> optionDo(Webapi.Dom.HtmlElement.focus);
  RxReact.useOnNextWithRef(focus);
};