type context('a) = {
  activeCount: RxAtomic.t(int),
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
    | (Some(_), _) => self.parent.subscriber |> RxSubscriber.complete(~exn?)
    | (_, Some(next)) => doSubscribeInternal(self, next)
    | _ => ()
    };
  }
  and innerSubscriberOperator = self =>
    RxSubscriber.decorate1(~onNext, ~onComplete, self)
  and doSubscribeInternal = (self, next) => {
    RxAtomic.incr(self.parent.activeCount) |> ignore;
    self.disposable =
      next
      |> RxObservable.lift(innerSubscriberOperator(self))
      |> RxObservable.subscribe;
    self.parent.subscriber
    |> RxSubscriber.addDisposable(self.disposable)
    |> ignore;
  }
  and doSubscribe = (parent, delegate, next) => {
    let self = {parent, delegate, disposable: RxDisposable.disposed};
    doSubscribeInternal(self, next);
  };
};

let onNext = (self, delegate, next) =>
  if (self.activeCount |> RxAtomic.get < self.maxConcurrency) {
    InnerSubscriber.doSubscribe(self, delegate, next);
  } else if (RxMutableQueue.length(self.queue) < self.maxBufferSize) {
    self.queue |> RxMutableQueue.enqueue(next);
  };

let onComplete = (self, delegate, exn) =>
  switch (exn) {
  | Some(_)
  | _ when self.activeCount |> RxAtomic.get === 0 =>
    delegate |> RxSubscriber.complete(~exn?)
  | _ => ()
  };

let create = (~maxBufferSize=max_int, ~maxConcurrency=max_int, subscriber) => {
  RxPreconditions.checkArgument(
    maxBufferSize >= 0,
    "MergeOperator: maxBufferSize must be greater than or equal to 0",
  );

  RxPreconditions.checkArgument(
    maxConcurrency > 0,
    "MergeOperator: maxConcurrency must be greater than 0",
  );

  let self = {
    activeCount: RxAtomic.make(0),
    maxBufferSize,
    maxConcurrency,
    queue: RxMutableQueue.create(),
    subscriber: RxSubscriber.disposed,
  };

  self.subscriber =
    subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, self);
  self.subscriber;
};