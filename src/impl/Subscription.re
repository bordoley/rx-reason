open Immutable;
open Option.Operators;

type t = unit => unit;

let create (dispose: unit => unit): t => {
  let dispose = ref (Some dispose);

  fun () => Concurrency.interlockedExchange dispose None
    >>| (fun dispose => dispose ())
    |? ();
};

let dispose (subscription: t) => subscription ();

let compose (subscriptions: list t): t => {
  let dispose = fun () => subscriptions
    |> List.toIterable
    |> Iterable.forEach (fun x => x ());
  create dispose
};

let empty: t = fun () => ();
