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

let peek = queue =>
  if (Js.Array.length(queue) > 0) {
    Some(queue[0]);
  } else {
    None;
  };

let length = Js.Array.length;