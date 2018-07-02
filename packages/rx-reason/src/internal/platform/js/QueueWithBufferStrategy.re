type t('a) = {
  bufferStrategy: BufferStrategy.t,
  disposable: Disposable.t,
  maxSize: int,
  queue: array('a),
};

let asDisposable = ({disposable}) => disposable;

let clear = queue =>
  queue
  |> Js.Array.removeCountInPlace(~pos=0, ~count=Js.Array.length(queue))
  |> ignore;

let create = (~bufferStrategy, ~maxSize) : t('a) => {
  let queue = [||];
  let disposable = Disposable.create1(clear, queue);

  {bufferStrategy, disposable, maxSize, queue};
};

let dispose = qwbs => qwbs |> asDisposable |> Disposable.dispose;

let isDisposed = qwbs => qwbs |> asDisposable |> Disposable.isDisposed;

let raiseIfDisposed = qwbs =>
  qwbs |> asDisposable |> Disposable.raiseIfDisposed;

let enqueue = (v, {queue, bufferStrategy, maxSize}) => {
  let shouldApplyBufferStrategy =
    maxSize > 0 && Js.Array.length(queue) === maxSize;
  switch (bufferStrategy) {
  | Raise when shouldApplyBufferStrategy =>
    BufferCapacityExceededException.raise(maxSize)
  | DropLatest when shouldApplyBufferStrategy => ()
  | _ => queue |> Js.Array.push(v) |> ignore
  };
};

let tryDeque = ({queue}) => queue |> Js.Array.shift;