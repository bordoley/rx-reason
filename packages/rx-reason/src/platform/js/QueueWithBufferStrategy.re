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
  queue
  |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue))
  |> ignore;

let tryDeque = ({queue}) => queue |> Js.Array.shift;

let enqueue = (v, {queue, bufferStrategy, maxSize} as this) => {
  let shouldApplyBufferStrategy =
    maxSize > 0 && Js.Array.length(queue) === maxSize;
  switch (bufferStrategy) {
  | Raise when shouldApplyBufferStrategy =>
    BufferCapacityExceededException.raise(maxSize)
  | DropOldest when shouldApplyBufferStrategy =>
    tryDeque(this) |> ignore;
    queue |> Js.Array.push(v) |> ignore;
  | DropLatest when shouldApplyBufferStrategy => ()
  | _ => queue |> Js.Array.push(v) |> ignore
  };
};