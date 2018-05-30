type t('a) = array('a);

[@bs.val] external shiftUnsafe : array('a) => 'a = "";
%bs.raw
{|
function shiftUnsafe(array) {
  return array.shift();
}|};

let create = () : t('a) => [||];

let dequeue = queue => shiftUnsafe(queue);

let enqueue = (scheduledItem, queue) =>
  queue |> Js.Array.push(scheduledItem) |> ignore;

let isEmpty = queue => Js.Array.length(queue) === 0;

let peek = queue => queue[0];

let size = queue => Js.Array.length(queue);

let reset = queue =>
  queue
  |> Js.Array.spliceInPlace(~pos=0, ~remove=size(queue), ~add=[||])
  |> ignore;

let forEach = (cb: 'a => unit, queue) => Js.Array.forEach(cb, queue);