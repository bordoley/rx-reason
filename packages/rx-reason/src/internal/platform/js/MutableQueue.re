type t('a) = array('a);

let clear = queue =>
  queue
  |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue))
  |> ignore;

let create = () : t('a) => [||];

let dequeue = queue => Js.Array.shift(queue);

let enqueue = (scheduledItem, queue) =>
  queue |> Js.Array.push(scheduledItem) |> ignore;

let forEach = (f, queue) => queue |> Js.Array.forEach(f);

/*
let isEmpty = queue => Js.Array.length(queue) === 0;*/

let peek = queue => queue[0];