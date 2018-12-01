type t('a) = array('a);

let add = (value, list) => list |> Js.Array.push(value) |> ignore;

let clear = queue =>
  queue
  |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue))
  |> ignore;

let create = () : t('a) => [||];

let forEachReversed = (f, list) => {
  let length = Js.Array.length(list);
  for (x in (length - 1) to 0) {
    f(list[x]);
  };
};

let remove = (value, list) => {
  let pos = list |> Js.Array.indexOf(value);
  if (pos >= 0) {
    list |> Js.Array.removeFromInPlace(~pos) |> ignore;
  };
};