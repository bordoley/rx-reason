[@bs.send]
external addEventListener :
  (
    Dom.eventTarget,
    string,
    Dom.event => unit,
    {
      .
      "capture": bool,
      "passive": bool,
    }
  ) =>
  unit =
  "addEventListener"; /* not widely supported */

[@bs.send]
external removeEventListener :
  (
    Dom.eventTarget,
    string,
    Dom.event => unit,
    {
      .
      "capture": bool,
      "passive": bool,
    }
  ) =>
  unit =
  "removeEventListener"; /* not widely supported */

let removeEventListenerTeardown = (eventName, eventHandler, options, node) =>
  node |. removeEventListener(eventName, eventHandler, options);

let subscribe = (capture, passive, eventName, node, subscriber) => {
  let eventHandler = ev => subscriber |> RxSubscriber.next(ev);
  let options = {"capture": capture, "passive": passive};
  node |. addEventListener(eventName, eventHandler, options);

  let disposable = RxDisposable.create4(
    removeEventListenerTeardown,
    eventName,
    eventHandler,
    options,
    node,
  );

  subscriber
  |> RxSubscriber.addDisposable(disposable)
  |> ignore;
};

let observeEvent =
    (~capture=false, ~passive=true, eventName: string, node: Dom.eventTarget)
    : RxObservable.t(Dom.event) =>
  RxObservable.create4(subscribe, capture, passive, eventName, node);