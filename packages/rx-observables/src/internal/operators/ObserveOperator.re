type t('a) =
  | C0('a => unit, option(exn) => unit): t('a)
  | C1(('c0, 'a) => unit, ('c0, option(exn)) => unit, 'c0): t('a)
  | C2(('c0, 'c1, 'a) => unit, ('c0, 'c1, option(exn)) => unit, 'c0, 'c1): t(
                                                                  'a,
                                                                  )
  | C3(
        ('c0, 'c1, 'c2, 'a) => unit,
        ('c0, 'c1, 'c2, option(exn)) => unit,
        'c0,
        'c1,
        'c2,
      ): t('a)
  | C4(
        ('c0, 'c1, 'c2, 'c3, 'a) => unit,
        ('c0, 'c1, 'c2, 'c3, option(exn)) => unit,
        'c0,
        'c1,
        'c2,
        'c3,
      ): t('a)
  | C5(
        ('c0, 'c1, 'c2, 'c3, 'c4, 'a) => unit,
        ('c0, 'c1, 'c2, 'c3, 'c4, option(exn)) => unit,
        'c0,
        'c1,
        'c2,
        'c3,
        'c4,
      ): t('a);

let onNext = (context: t('a), subscriber, next) => {
  switch (context) {
  | C0(onNext, _) => onNext(next)
  | C1(onNext, _, c0) => onNext(c0, next)
  | C2(onNext, _, c0, c1) => onNext(c0, c1, next)
  | C3(onNext, _, c0, c1, c2) => onNext(c0, c1, c2, next)
  | C4(onNext, _, c0, c1, c2, c3) => onNext(c0, c1, c2, c3, next)
  | C5(onNext, _, c0, c1, c2, c3, c4) => onNext(c0, c1, c2, c3, c4, next)
  };
  subscriber |> RxSubscriber.next(next);
};

let onComplete = (context, subscriber, exn) => {
  switch (context) {
  | C0(_, onComplete) =>
    onComplete(exn);
  | C1(_, onComplete, c0) =>
    onComplete(c0, exn);
  | C2(_, onComplete, c0, c1) =>
    onComplete(c0, c1, exn);
  | C3(_, onComplete, c0, c1, c2) =>
    onComplete(c0, c1, c2, exn);
  | C4(_, onComplete, c0, c1, c2, c3) =>
    onComplete(c0, c1, c2, c3, exn);
  | C5(_, onComplete, c0, c1, c2, c3, c4) =>
    onComplete(c0, c1, c2, c3, c4, exn);
  };
  subscriber |> RxSubscriber.complete(~exn?);
};

let decorate = (context, subscriber) =>
  subscriber |> RxSubscriber.decorate1(~onNext, ~onComplete, context);

let create = (~onNext, ~onComplete, subscriber) => {
  let context = C0(onNext, onComplete);
  decorate(context, subscriber);
};

let create1 = (~onNext, ~onComplete, ctx0, subscriber) => {
  let context = C1(onNext, onComplete, ctx0);
  decorate(context, subscriber);
};

let create2 = (~onNext, ~onComplete, ctx0, ctx1, subscriber) => {
  let context = C2(onNext, onComplete, ctx0, ctx1);
  decorate(context, subscriber);
};

let create3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, subscriber) => {
  let context = C3(onNext, onComplete, ctx0, ctx1, ctx2);
  decorate(context, subscriber);
};

let create4 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, subscriber) => {
  let context = C4(onNext, onComplete, ctx0, ctx1, ctx2, ctx3);
  decorate(context, subscriber);
};

let create5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, subscriber) => {
  let context = C5(onNext, onComplete, ctx0, ctx1, ctx2, ctx3, ctx4);
  decorate(context, subscriber);
};