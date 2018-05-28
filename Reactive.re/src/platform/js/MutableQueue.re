type t('a) = array('a);

[@bs.val] external shiftUnsafe: (array('a)) => 'a = "";
[%%bs.raw{|
function shiftUnsafe(array) {
  return array.shift();
}|}];

let create= (): t('a) => [||];

let dequeue = queue =>
  shiftUnsafe(queue);

let enqueue = (scheduledItem, queue) =>
  queue |> Js.Array.push(scheduledItem) |> ignore;

let isEmpty = queue => Js.Array.length(queue) === 0;