type t('a) = {
  queue: array('a),
  bufferStrategy: BufferStrategy.t,
  maxSize: int,
};

let create = (~bufferStrategy, ~maxSize) : t('a) => {
  queue: [||],
  bufferStrategy,
  maxSize,
};

let clear = ({queue}) =>
  queue |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue)) |> ignore;

let tryDeque = ({queue}) => queue |> Js.Array.shift;

let enqueue = (v, {queue, bufferStrategy, maxSize} as this) => {
  let shouldApplyBackPressure = maxSize > 0 && Js.Array.length(queue) === maxSize;
  switch (bufferStrategy) {
  | Throw when shouldApplyBackPressure =>
    failwith("BufferException")
  | DropOldest when shouldApplyBackPressure =>
    tryDeque(this) |> ignore;
    queue |> Js.Array.push(v) |> ignore;
  | DropLatest when shouldApplyBackPressure => ()
  | _ => queue |> Js.Array.push(v) |> ignore
  };
};