type context('a) = {
  activeCount: RxAtomic.t(int),
  isStopped: RxAtomic.t(bool),
  maxBufferSize: int,
  maxConcurrency: int,
  queue: RxMutableQueue.t(RxObservable.t('a)),
  mutable subscriber: RxSubscriber.t(RxObservable.t('a)),
};

module InnerSubscriber = {
  type innerContext('a) = {
    parent: context('a),
    delegate: RxSubscriber.t('a),
    mutable disposable: RxDisposable.t,
  };

  let onNext = (self, _, next) => self.delegate |> RxSubscriber.next(next);

  let rec onComplete = (self, _, exn) => {
    RxAtomic.decr(self.parent.activeCount) |> ignore;
    self.parent.subscriber
    |> RxSubscriber.removeDisposable(self.disposable)
    |> ignore;

    let next = self.parent.queue |> RxMutableQueue.dequeue;
    switch (exn, next) {
    | (Some(_), _)
    | _ when self.parent.activeCount |> RxAtomic.get === 0 =>
      self.parent.subscriber |> RxSubscriber.complete(~exn?)
    | (_, Some(next)) => doSubscribe(self.parent, self.delegate, next)
    | _ => ()
    };
  }
  and innerSubscriberOperator = self =>
    RxSubscriber.decorate1(~onNext, ~onComplete, self)
  and doSubscribe = (parent, delegate, next) => {
    RxAtomic.incr(parent.activeCount) |> ignore;

    let self = {parent, delegate, disposable: RxDisposable.disposed};

    self.disposable =
      next
      |> RxObservable.lift(innerSubscriberOperator(self))
      |> RxObservable.subscribe;

    parent.subscriber |> RxSubscriber.addDisposable(self.disposable) |> ignore;
  };
};

let onNext = (self, delegate, next) =>
  if (self.activeCount |> RxAtomic.get < self.maxConcurrency) {
    InnerSubscriber.doSubscribe(self, delegate, next);
  } else if (RxMutableQueue.length(self.queue) < self.maxBufferSize) {
    self.queue |> RxMutableQueue.enqueue(next);
  };

let onComplete = (self, delegate, exn) => {
  RxAtomic.set(self.isStopped, true);

  switch (exn) {
  | Some(_)
  | _ when self.activeCount |> RxAtomic.get === 0 =>
    delegate |> RxSubscriber.complete(~exn?)
  | _ => ()
  };
};

let operator = (~maxBufferSize=max_int, ~maxConcurrency=max_int, subscriber) => {
  let self = {
    activeCount: RxAtomic.make(0),
    isStopped: RxAtomic.make(false),
    maxBufferSize,
    maxConcurrency,
    queue: RxMutableQueue.create(),
    subscriber: RxSubscriber.disposed,
  };

  self.subscriber =
    subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, self);
  self.subscriber;
};