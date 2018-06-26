type t('a) = Subscriber.t('a);

let onSuccess = (result, subscriber) => {
  subscriber |> Subscriber.next(result);
  subscriber |> Subscriber.complete;
};

let onError = (exn, subscriber) => 
  subscriber |> Subscriber.complete(~exn);

let asSubscriber = Functions.identity;
let delegate = Functions.identity;

let asCompositeDisposable = Subscriber.asCompositeDisposable;
let asDisposable = Subscriber.asDisposable;
let addTeardown = Subscriber.addTeardown;
let addTeardown1 = Subscriber.addTeardown1;
let addTeardown2 = Subscriber.addTeardown2;
let addTeardown3 = Subscriber.addTeardown3;
let addTeardown4 = Subscriber.addTeardown4;
let addTeardown5 = Subscriber.addTeardown5;
let addTeardown6 = Subscriber.addTeardown6;
let addTeardown7 = Subscriber.addTeardown7;
let dispose = Subscriber.dispose;
let disposed = Subscriber.disposed;
let isDisposed = Subscriber.isDisposed;
let raiseIfDisposed = Subscriber.raiseIfDisposed;