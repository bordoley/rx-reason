type t('a) = array('a);

[@bs.val] external shiftUnsafe : array('a) => 'a = "";
%bs.raw
{|
function shiftUnsafe(array) {
  return array.shift();
}|};

let clear = queue =>
  queue
  |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue))
  |> ignore;

let create = () : t('a) => [||];

let dequeue = queue => shiftUnsafe(queue);

let enqueue = (scheduledItem, queue) =>
  queue |> Js.Array.push(scheduledItem) |> ignore;

let forEach = (f, queue) => queue |> Js.Array.forEach(f);

let isEmpty = queue => Js.Array.length(queue) === 0;

let peek = queue => queue[0];