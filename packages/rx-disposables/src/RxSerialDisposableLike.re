module type S = {
  type t;

  include RxDisposableLike.S with type t := t;

  /** Returns the currently contained Disposable */
  let getInnerDisposable: t => RxDisposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let setInnerDisposable: (RxDisposable.t, t) => unit;
};

module type S1 = {
  type t('a);

  include RxDisposableLike.S1 with type t('a) := t('a);

  /** Returns the currently contained Disposable */
  let getInnerDisposable: t('a) => RxDisposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let setInnerDisposable: (RxDisposable.t, t('a)) => unit;
};

module type S2 = {
  include RxDisposableLike.S2;

  /** Returns the currently contained Disposable */
  let getInnerDisposable: t('a, 'b) => RxDisposable.t;

  /** Atomically set the next disposable on this container and dispose the previous
   *  one (if any) or dispose next if the container has been disposed
   */
  let setInnerDisposable: (RxDisposable.t, t('a, 'b)) => unit;
};