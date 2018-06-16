/**
 * Module function(Functor) for creating ReasonReact stateful components
 * which utilize a RxReason observable to maintain state.
 *
 * ```re
 * module BlinkTag =
 *  RxReasonReact.Component.Make({
 *   type props = string;
 *   type state = string;
 *
 *   let name = "BlinkTag";
 *   let scheduler = RxReasonJs.EventLoop.scheduleWithDelay(1000.0);
 *   let operator =
 *     RxReason.Operators.(
 *       map(title =>
 *         RxReason.Observable.ofList(~scheduler, [title, ""])
 *         |> RxReason.Observable.repeat
 *       )
 *       >> switch_
 *     );
 *   let createStore = props => props |> RxReason.Observable.lift(operator);
 *
 *   let render = (~state: string, _) => {
 *     let child = ReasonReact.string(state);
 *     <div> child </div>;
 *   };
 * });
 * ```
 */;

module type RxReasonReactComponentSpec = {
  type props;
  type state;

  let name: string;
  let createStore:
    RxReason.Observable.t(props) => RxReason.Observable.t(state);
  let render:
    (~state: state, array(ReasonReact.reactElement)) =>
    ReasonReact.reactElement;
};

module type RxReasonReactComponent = {
  type props;
  type state;
  type action;

  let make:
    (~props: props, array(ReasonReact.reactElement)) =>
    ReasonReact.componentSpec(
      state,
      state,
      ReasonReact.noRetainedProps,
      ReasonReact.noRetainedProps,
      action,
    );
};

module Make:
  (ComponentSpec: RxReasonReactComponentSpec) =>
  RxReasonReactComponent with type props := ComponentSpec.props;