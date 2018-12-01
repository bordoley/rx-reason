module Action = {
  type t =
    | Blur
    | Click
    | Focus;
};

let update = (ele, action) => {
  let ele = ele |> Webapi.Dom.HtmlElement.ofElement;
  switch (ele, action) {
  | (Some(ele), Action.Blur) => ele |> Webapi.Dom.HtmlElement.blur
  | (Some(ele), Action.Click) => ele |> Webapi.Dom.HtmlElement.click
  | (Some(ele), Action.Focus) => ele |> Webapi.Dom.HtmlElement.focus
  | _ => ()
  };
};

let useRef = observable => RxReact.useRef(update, observable);