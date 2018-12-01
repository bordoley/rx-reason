type t('a) = array('a);

let add = (value, list) => list |> Js.Array.push(value) |> ignore;

let clear = queue =>
  queue
  |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue))
  |> ignore;

let create = () : t('a) => [||];

let forEachReversed = (f, list) => {
  let length = Js.Array.length(list);
  for (i in 0 to length - 1) {
    let idx = length - 1 - i;
    f(list[idx]);
  };
};

let remove = (value, list) => {
  let pos = list |> Js.Array.indexOf(value);
  if (pos >= 0) {
    list |> Js.Array.removeFromInPlace(~pos) |> ignore;
  };
};