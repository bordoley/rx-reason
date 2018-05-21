type t = {
  reference: ref(Disposable.t),
  disposable: Disposable.t,
};

let create = () => {
  let reference = ref(Disposable.disposed);
  let disposable = Disposable.create(() => reference^ |> Disposable.dispose);
  {reference, disposable};
};

let assign = (disposable: Disposable.t, assignable: t): t => {
  /* FIXME: what happens if assignable is already disposed? */
  Concurrency.volatileWrite(disposable, assignable.reference);
  assignable;
};

let toDisposable = ({disposable}: t) : Disposable.t => disposable;