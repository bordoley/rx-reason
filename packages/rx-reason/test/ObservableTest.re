open ReUnit;
open ReUnit.Test;
open ReUnitHelpers;

let test =
  describe(
    "Observable",
    [
      describe(
        "combineLatest2",
        [
          observableIt(
            "combines latest values from each observable",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.combineLatest2(
                  ~selector=(a, b) => a + b,
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(0)),
                      (3.0, Next(2)),
                      (5.0, Next(4)),
                      (6.0, Complete),
                    ],
                  ),
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (0.0, Next(0)),
                      (2.0, Next(2)),
                      (4.0, Next(4)),
                      (5.0, Complete),
                    ],
                  ),
                ),
            ~expected=[
              Next(0),
              Next(2),
              Next(4),
              Next(6),
              Next(8),
              Complete,
            ],
            (),
          ),
          observableIt(
            "completes when either observable completes with an exception",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.combineLatest2(
                  ~selector=(a, b) => a + b,
                  Observable.raise(
                    ~scheduler=scheduler.scheduleWithDelay(~delay=2.0),
                    Division_by_zero,
                  ),
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(0)),
                      (3.0, Next(2)),
                      (5.0, Next(4)),
                      (6.0, Complete),
                    ],
                  ),
                ),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
        ],
      ),
      describe(
        "combineLatest3",
        [
          observableIt(
            "combines latest values from each observable",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.combineLatest3(
                  ~selector=(a, b, c) => a + b + c,
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(1.0, Next(0)), (6.0, Next(3)), (7.0, Complete)],
                  ),
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(2.0, Next(1)), (5.0, Complete)],
                  ),
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(3.0, Next(2)), (5.0, Complete)],
                  ),
                ),
            ~expected=[Next(3), Next(6), Complete],
            (),
          ),
          observableIt(
            "completes when either observable completes with an exception",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.combineLatest3(
                  ~selector=(a, b, c) => a + b + c,
                  Observable.raise(
                    ~scheduler=scheduler.scheduleWithDelay(~delay=5.0),
                    Division_by_zero,
                  ),
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(0)),
                      (3.0, Next(2)),
                      (5.0, Next(4)),
                      (6.0, Complete),
                    ],
                  ),
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [
                      (1.0, Next(0)),
                      (3.0, Next(2)),
                      (5.0, Next(4)),
                      (6.0, Complete),
                    ],
                  ),
                ),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
        ],
      ),
      describe(
        "combineLatest4",
        [
          it("combines latest values from each subject", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let result = ref([]);

            Observable.combineLatest4(
              ~selector=(a, b, c, d) => [a, b, c, d],
              subject0 |> Subject.toObservable,
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
              subject3 |> Subject.toObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

            observer0 |> Observer.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer1 |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer2 |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer3 |> Observer.next(3);
            result^ |> Expect.toBeEqualToListOfInt([0, 1, 2, 3]);
          }),
          it("completes only when all observables complete", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest4(
                ~selector=(a, b, c, d) => [a, b, c, d],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

            observer0 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer1 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer2 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer3 |> Observer.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes when the first observable completes with an exception",
            () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest4(
                ~selector=(a, b, c, d) => [a, b, c, d],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;
            let observer2 = subject2 |> Subject.toObserver;

            observer2 |> Observer.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            result^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "combineLatest5",
        [
          it("combines latest values from each subject", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();

            let result = ref([]);

            Observable.combineLatest5(
              ~selector=(a, b, c, d, e) => [a, b, c, d, e],
              subject0 |> Subject.toObservable,
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
              subject3 |> Subject.toObservable,
              subject4 |> Subject.toObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;
            let observer4 = subject4 |> Subject.toObserver;

            observer0 |> Observer.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer1 |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer2 |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer3 |> Observer.next(3);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer4 |> Observer.next(4);
            result^ |> Expect.toBeEqualToListOfInt([0, 1, 2, 3, 4]);
          }),
          it("completes only when all observables complete", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest5(
                ~selector=(a, b, c, d, e) => [a, b, c, d, e],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
                subject4 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;
            let observer4 = subject4 |> Subject.toObserver;

            observer0 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer1 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer2 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer3 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer4 |> Observer.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes when the first observable completes with an exception",
            () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest5(
                ~selector=(a, b, c, d, e) => [a, b, c, d, e],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
                subject4 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;
            let observer3 = subject3 |> Subject.toObserver;

            observer3 |> Observer.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            result^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "combineLatest6",
        [
          it("combines latest values from each subject", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();
            let subject5 = Subject.create();

            let result = ref([]);

            Observable.combineLatest6(
              ~selector=(a, b, c, d, e, f) => [a, b, c, d, e, f],
              subject0 |> Subject.toObservable,
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
              subject3 |> Subject.toObservable,
              subject4 |> Subject.toObservable,
              subject5 |> Subject.toObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;
            let observer4 = subject4 |> Subject.toObserver;
            let observer5 = subject5 |> Subject.toObserver;

            observer0 |> Observer.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer1 |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer2 |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer3 |> Observer.next(3);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer4 |> Observer.next(4);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer5 |> Observer.next(5);
            result^ |> Expect.toBeEqualToListOfInt([0, 1, 2, 3, 4, 5]);
          }),
          it("completes only when all observables complete", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();
            let subject5 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest6(
                ~selector=(a, b, c, d, e, f) => [a, b, c, d, e, f],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
                subject4 |> Subject.toObservable,
                subject5 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;
            let observer4 = subject4 |> Subject.toObserver;
            let observer5 = subject5 |> Subject.toObserver;

            observer0 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer1 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer2 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer3 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer4 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer5 |> Observer.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes when the first observable completes with an exception",
            () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();
            let subject5 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest6(
                ~selector=(a, b, c, d, e, f) => [a, b, c, d, e, f],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
                subject4 |> Subject.toObservable,
                subject5 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;
            let observer4 = subject4 |> Subject.toObserver;

            observer4 |> Observer.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            result^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "combineLatest7",
        [
          it("combines latest values from each subject", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();
            let subject5 = Subject.create();
            let subject6 = Subject.create();

            let result = ref([]);

            Observable.combineLatest7(
              ~selector=(a, b, c, d, e, f, g) => [a, b, c, d, e, f, g],
              subject0 |> Subject.toObservable,
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
              subject3 |> Subject.toObservable,
              subject4 |> Subject.toObservable,
              subject5 |> Subject.toObservable,
              subject6 |> Subject.toObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;
            let observer4 = subject4 |> Subject.toObserver;
            let observer5 = subject5 |> Subject.toObserver;
            let observer6 = subject6 |> Subject.toObserver;

            observer0 |> Observer.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer1 |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer2 |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer3 |> Observer.next(3);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer4 |> Observer.next(4);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer5 |> Observer.next(5);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer6 |> Observer.next(6);
            result^ |> Expect.toBeEqualToListOfInt([0, 1, 2, 3, 4, 5, 6]);
          }),
          it("completes only when all observables complete", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();
            let subject5 = Subject.create();
            let subject6 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest7(
                ~selector=(a, b, c, d, e, f, g) => [a, b, c, d, e, f, g],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
                subject4 |> Subject.toObservable,
                subject5 |> Subject.toObservable,
                subject6 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;
            let observer4 = subject4 |> Subject.toObserver;
            let observer5 = subject5 |> Subject.toObserver;
            let observer6 = subject6 |> Subject.toObserver;

            observer0 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer1 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer2 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer3 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer4 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer5 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer6 |> Observer.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes when the first observable completes with an exception",
            () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            let subject4 = Subject.create();
            let subject5 = Subject.create();
            let subject6 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest7(
                ~selector=(a, b, c, d, e, f, g) => [a, b, c, d, e, f, g],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
                subject4 |> Subject.toObservable,
                subject5 |> Subject.toObservable,
                subject6 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;
            let observer5 = subject5 |> Subject.toObserver;

            observer5 |> Observer.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            result^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "concat",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.concat([
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduler.scheduleWithDelay,
                    [(1.0, Next(7)), (3.0, Next(9)), (4.0, Complete)],
                  ),
                  Observable.ofList([1, 2, 3]),
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduler.scheduleWithDelay,
                    [(2.0, Next(8)), (4.0, Next(10)), (5.0, Complete)],
                  ),
                  Observable.ofList([4, 5, 6]),
                ]),
            ~expected=[
              Next(7),
              Next(9),
              Next(1),
              Next(2),
              Next(3),
              Next(8),
              Next(10),
              Next(4),
              Next(5),
              Next(6),
              Complete,
            ],
            (),
          ),
          observableIt(
            "completes early if any observable completes with an exception",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.concat([
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduler.scheduleWithDelay,
                    [(1.0, Next(1)), (3.0, Next(2)), (4.0, Complete)],
                  ),
                  Observable.raise(
                    ~scheduler=scheduler.scheduleWithDelay(~delay=2.0),
                    Division_by_zero,
                  ),
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduler.scheduleWithDelay,
                    [(1.0, Next(1)), (3.0, Next(2)), (4.0, Complete)],
                  ),
                ]),
            ~expected=[Next(1), Next(2), CompleteWithException(Division_by_zero)],
            (),
          ),
        ],
      ),
      describe(
        "create",
        [
          observableIt(
            "onNext and onComplete function calls delegate to the subscriber",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.create((~onNext, ~onComplete) => {
                  onNext(10);
                  onNext(20);
                  onComplete(None);
                  Disposable.disposed;
                }),
            ~expected=[Next(10), Next(20), Complete],
            (),
          ),
          observableIt(
            "onSubscribe throws synchronously before completing",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.create((~onNext as _, ~onComplete as _) =>
                  raise(Division_by_zero)
                ),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          it(
            "throws when onSubscribe complete synchronously and then throws an exception",
            () => {
            let observable =
              Observable.create((~onNext as _, ~onComplete) => {
                onComplete(None);
                raise(Division_by_zero);
              });

            (() => observable |> Observable.subscribe) |> Expect.shouldRaise;
          }),
        ],
      ),
      describe(
        "defer",
        [
          it("calls the observable factory on subscribe", () => {
            let count = ref(0);

            let observable =
              Observable.defer(() => {
                count := count^ + 1;
                Observable.empty();
              });

            observable |> Observable.subscribe |> ignore;
            observable |> Observable.subscribe |> ignore;
            observable |> Observable.subscribe |> ignore;
            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe("empty", []),
      describe(
        "merge",
        [
          observableIt(
            "synchronous and asynchronous observables",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.merge([
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(1.0, Next(7)), (3.0, Next(9)), (4.0, Complete)],
                  ),
                  Observable.ofList([1, 2, 3]),
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(2.0, Next(8)), (4.0, Next(10)), (5.0, Complete)],
                  ),
                  Observable.ofList([4, 5, 6]),
                ]),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              Next(6),
              Next(7),
              Next(8),
              Next(9),
              Next(10),
              Complete,
            ],
            (),
          ),
          observableIt(
            "completes early if any observable completes with an exception",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.merge([
                  Observable.ofAbsoluteTimeNotifications(
                    ~scheduler,
                    [(1.0, Next(1)), (3.0, Next(2)), (4.0, Complete)],
                  ),
                  Observable.raise(
                    ~scheduler=scheduler.scheduleWithDelay(~delay=2.0),
                    Division_by_zero,
                  ),
                ]),
            ~expected=[Next(1), CompleteWithException(Division_by_zero)],
            (),
          ),
        ],
      ),
      describe("never", []),
      describe("ofList", []),
      describe("ofValue", []),
      describe("publish", []),
      describe("raise", []),
      describe(
        "retry",
        [
          it("with cold observable", () => {
            let result = ref([]);
            let retryCount = ref(0);
            let shouldRetry = _ => {
              let retry = retryCount^ < 2;
              if (retry) {
                Interlocked.increment(retryCount) |> ignore;
              };
              retry;
            };

            Observable.create((~onNext, ~onComplete) => {
              onNext(1);
              onNext(2);
              onComplete(Some(Division_by_zero));
              Disposable.disposed;
            })
            |> Observable.retry(shouldRetry)
            |> Observable.subscribe(~onNext=x => result := [x, ...result^])
            |> ignore;
            result^ |> Expect.toBeEqualToListOfInt([2, 1, 2, 1, 2, 1]);
          }),
          it("with hot observable", () => {
            let result = ref([]);
            let subject = ref(Subject.create());

            let subscription =
              Observable.create((~onNext, ~onComplete) => {
                subject := Subject.create();
                let observable = subject^ |> Subject.toObservable;
                observable
                |> Observable.subscribeWithCallbacks(~onNext, ~onComplete);
              })
              |> Observable.retry(_ => true)
              |> Observable.subscribe(~onNext=x => result := [x, ...result^]);

            let observer = subject^ |> Subject.toObserver;
            observer |> Observer.next(1);
            observer |> Observer.next(2);
            observer |> Observer.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            let observer = subject^ |> Subject.toObserver;
            observer |> Observer.next(3);
            observer |> Observer.next(4);
            observer |> Observer.complete;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;

            result^ |> Expect.toBeEqualToListOfInt([4, 3, 2, 1]);
          }),
          it("doesn't retry if unsubscribed in shouldRetry callback", () => {
            let subscription = ref(Disposable.disposed);
            let result = ref([]);
            let subject = ref(Subject.create());

            subscription :=
              Observable.create((~onNext, ~onComplete) => {
                subject := Subject.create();
                let observable = subject^ |> Subject.toObservable;
                observable
                |> Observable.subscribeWithCallbacks(~onNext, ~onComplete);
              })
              |> Observable.retry(_ => {
                   subscription^ |> Disposable.dispose;
                   true;
                 })
              |> Observable.subscribe(~onNext=x => result := [x, ...result^]);

            subject^ |> Subject.toObserver |> Observer.next(5);
            subject^
            |> Subject.toObserver
            |> Observer.complete(~exn=Division_by_zero);
            subject^ |> Subject.toObserver |> Observer.next(6);

            result^ |> Expect.toBeEqualToListOfInt([5]);
          }),
        ],
      ),
      describe(
        "startWithlist",
        [
          observableIt(
            "prepends the values",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay}) =>
                Observable.startWithList(
                  [1, 2, 3],
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [(1.0, Next(4)), (2.0, Next(5)), (3.0, Complete)],
                  ),
                ),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe(
        "startWithValue",
        [
          observableIt(
            "prepends the values",
            ~nextToString=string_of_int,
            ~source=
              _ => Observable.startWithValue(1, Observable.ofList([2, 3])),
            ~expected=[Next(1), Next(2), Next(3), Complete],
            (),
          ),
        ],
      ),
      describe("subscribeOn", []),
    ],
  );