type synthetic('a);

module Synthetic: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : synthetic('a) => bool = "";
  [@bs.get] external cancelable : synthetic('a) => bool = "";
  [@bs.get] external currentTarget : synthetic('a) => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : synthetic('a) => bool = "";
  [@bs.get] external eventPhase : synthetic('a) => int = "";
  [@bs.get] external isTrusted : synthetic('a) => bool = "";
  [@bs.get] external nativeEvent : synthetic('a) => Js.t({..}) = "";
  [@bs.send] external preventDefault : synthetic('a) => unit = "";
  [@bs.send] external isDefaultPrevented : synthetic('a) => bool = "";
  [@bs.send] external stopPropagation : synthetic('a) => unit = "";
  [@bs.send] external isPropagationStopped : synthetic('a) => bool = "";
  [@bs.get] external target : synthetic('a) => Js.t({..}) = "";
  [@bs.get] external timeStamp : synthetic('a) => float = "";
  [@bs.get] external type_ : synthetic('a) => string = "type";
  [@bs.send] external persist : synthetic('a) => unit = "";
};

/* Cast any event type to the general synthetic type. This is safe, since synthetic is more general */
external toSyntheticEvent : synthetic('a) => Synthetic.t = "%identity";

module Clipboard: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external clipboardData : t => Js.t({..}) = ""; /* Should return Dom.dataTransfer */
};

module Composition: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external data : t => string = "";
};

module Keyboard: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external altKey : t => bool = "";
  [@bs.get] external charCode : t => int = "";
  [@bs.get] external ctrlKey : t => bool = "";
  [@bs.send] external getModifierState : (t, string) => bool = "";
  [@bs.get] external key : t => string = "";
  [@bs.get] external keyCode : t => int = "";
  [@bs.get] external locale : t => string = "";
  [@bs.get] external location : t => int = "";
  [@bs.get] external metaKey : t => bool = "";
  [@bs.get] external repeat : t => bool = "";
  [@bs.get] external shiftKey : t => bool = "";
  [@bs.get] external which : t => int = "";
};

module Focus: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external relatedTarget : t => Js.t({..}) = ""; /* Should return Dom.eventTarget */
};

module Form: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
};

module Mouse: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external altKey : t => bool = "";
  [@bs.get] external button : t => int = "";
  [@bs.get] external buttons : t => int = "";
  [@bs.get] external clientX : t => int = "";
  [@bs.get] external clientY : t => int = "";
  [@bs.get] external ctrlKey : t => bool = "";
  [@bs.send] external getModifierState : (t, string) => bool = "";
  [@bs.get] external metaKey : t => bool = "";
  [@bs.get] external pageX : t => int = "";
  [@bs.get] external pageY : t => int = "";
  [@bs.get] external relatedTarget : t => Js.t({..}) = ""; /* Should return Dom.eventTarget */
  [@bs.get] external screenX : t => int = "";
  [@bs.get] external screenY : t => int = "";
  [@bs.get] external shiftKey : t => bool = "";
};

module Selection: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
};

module Touch: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external altKey : t => bool = "";
  [@bs.get] external changedTouches : t => Js.t({..}) = ""; /* Should return Dom.touchList */
  [@bs.get] external ctrlKey : t => bool = "";
  [@bs.send] external getModifierState : (t, string) => bool = "";
  [@bs.get] external metaKey : t => bool = "";
  [@bs.get] external shiftKey : t => bool = "";
  [@bs.get] external targetTouches : t => Js.t({..}) = ""; /* Should return Dom.touchList */
  [@bs.get] external touches : t => Js.t({..}) = ""; /* Should return Dom.touchList */
};

module UI: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external detail : t => int = "";
  [@bs.get] external view : t => Dom.window = ""; /* Should return DOMAbstractView/WindowProxy */
};

module Wheel: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external deltaMode : t => int = "";
  [@bs.get] external deltaX : t => float = "";
  [@bs.get] external deltaY : t => float = "";
  [@bs.get] external deltaZ : t => float = "";
};

module Media: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
};

module Image: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
};

module Animation: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external animationName : t => string = "";
  [@bs.get] external pseudoElement : t => string = "";
  [@bs.get] external elapsedTime : t => float = "";
};

module Transition: {
  type tag;
  type t = synthetic(tag);
  [@bs.get] external bubbles : t => bool = "";
  [@bs.get] external cancelable : t => bool = "";
  [@bs.get] external currentTarget : t => Js.t({..}) = "";
  [@bs.get] external defaultPrevented : t => bool = "";
  [@bs.get] external eventPhase : t => int = "";
  [@bs.get] external isTrusted : t => bool = "";
  [@bs.get] external nativeEvent : t => Js.t({..}) = "";
  [@bs.send] external preventDefault : t => unit = "";
  [@bs.send] external isDefaultPrevented : t => bool = "";
  [@bs.send] external stopPropagation : t => unit = "";
  [@bs.send] external isPropagationStopped : t => bool = "";
  [@bs.get] external target : t => Js.t({..}) = "";
  [@bs.get] external timeStamp : t => float = "";
  [@bs.get] external type_ : t => string = "type";
  [@bs.send] external persist : t => unit = "";
  [@bs.get] external propertyName : t => string = "";
  [@bs.get] external pseudoElement : t => string = "";
  [@bs.get] external elapsedTime : t => float = "";
};