type history;
[@bs.val] [@bs.scope "window"] external history: history = "history";
[@bs.send.pipe: history]
external pushState: (option(unit), string, string) => unit = "";

type location;
[@bs.val] [@bs.scope "window"] external location: location = "location";
[@bs.get] external href: location => string = "";

[@bs.val] external window: Dom.eventTarget = "window";

type t = unit;

let instance: t = ();

let observable =
  window
  |> RxDomEventTarget.observeEvent(~passive=true, "popstate")
  |> RxObservables.map(_ => location |> href)
  |> RxObservables.startWithValue(location |> href)
  |> RxObservables.shareReplayLast;

let asObservable = _ => observable;

 let notify = (prev, next) => {
   if (prev !== next) {
     history |> pushState(None, "", next);
   };
 }

let update = (f, _) => {
  let value = location |> href;
  let nextValue = f(value);
  notify(value, nextValue);
};

let update1 = (f, ctx0, _) => {
  let value = location |> href;
  let nextValue = f(ctx0, value);
  notify(value, nextValue);
};

let update2 = (f, ctx0, ctx1, _) => {
  let value = location |> href;
  let nextValue = f(ctx0, ctx1, value);
  notify(value, nextValue);
};

let update3 = (f, ctx0, ctx1, ctx2, _) => {
  let value = location |> href;
  let nextValue = f(ctx0, ctx1, ctx2, value);
  notify(value, nextValue);
};

let update4 = (f, ctx0, ctx1, ctx2, ctx3, _) => {
  let value = location |> href;
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, value);
  notify(value, nextValue);
};

let update5 = (f, ctx0, ctx1, ctx2, ctx3, ctx4, _) => {
  let value = location |> href;
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, ctx4, value);
  notify(value, nextValue);
};