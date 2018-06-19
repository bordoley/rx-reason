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
                    ~scheduler=scheduler.scheduleWithDelay(2.0),
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
                    ~scheduler=scheduler.scheduleWithDelay(5.0),
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
              subject0 |> Subject.asObservable,
              subject1 |> Subject.asObservable,
              subject2 |> Subject.asObservable,
              subject3 |> Subject.asObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            subject0 |> Subject.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject1 |> Subject.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject2 |> Subject.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject3 |> Subject.next(3);
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subject0 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject1 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject2 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject3 |> Subject.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;

            subject2 |> Subject.complete(~exn=Division_by_zero);
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
              subject0 |> Subject.asObservable,
              subject1 |> Subject.asObservable,
              subject2 |> Subject.asObservable,
              subject3 |> Subject.asObservable,
              subject4 |> Subject.asObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            subject0 |> Subject.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject1 |> Subject.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject2 |> Subject.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject3 |> Subject.next(3);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject4 |> Subject.next(4);
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
                subject4 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subject0 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject1 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject2 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject3 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject4 |> Subject.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
                subject4 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;

            subject3 |> Subject.complete(~exn=Division_by_zero);
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
              subject0 |> Subject.asObservable,
              subject1 |> Subject.asObservable,
              subject2 |> Subject.asObservable,
              subject3 |> Subject.asObservable,
              subject4 |> Subject.asObservable,
              subject5 |> Subject.asObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            subject0 |> Subject.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject1 |> Subject.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject2 |> Subject.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject3 |> Subject.next(3);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject4 |> Subject.next(4);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject5 |> Subject.next(5);
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
                subject4 |> Subject.asObservable,
                subject5 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subject0 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject1 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject2 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject3 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject4 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject5 |> Subject.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
                subject4 |> Subject.asObservable,
                subject5 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;

            subject4 |> Subject.complete(~exn=Division_by_zero);
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
              subject0 |> Subject.asObservable,
              subject1 |> Subject.asObservable,
              subject2 |> Subject.asObservable,
              subject3 |> Subject.asObservable,
              subject4 |> Subject.asObservable,
              subject5 |> Subject.asObservable,
              subject6 |> Subject.asObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            subject0 |> Subject.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject1 |> Subject.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject2 |> Subject.next(2);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject3 |> Subject.next(3);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject4 |> Subject.next(4);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject5 |> Subject.next(5);
            result^ |> Expect.toBeEqualToListOfInt([]);
            subject6 |> Subject.next(6);
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
                subject4 |> Subject.asObservable,
                subject5 |> Subject.asObservable,
                subject6 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subject0 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject1 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject2 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject3 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject4 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject5 |> Subject.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            subject6 |> Subject.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
                subject0 |> Subject.asObservable,
                subject1 |> Subject.asObservable,
                subject2 |> Subject.asObservable,
                subject3 |> Subject.asObservable,
                subject4 |> Subject.asObservable,
                subject5 |> Subject.asObservable,
                subject6 |> Subject.asObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;

            subject5 |> Subject.complete(~exn=Division_by_zero);
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;
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
                    ~scheduler=scheduler.scheduleWithDelay(2.0),
                    Division_by_zero,
                  ),
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduler.scheduleWithDelay,
                    [(1.0, Next(1)), (3.0, Next(2)), (4.0, Complete)],
                  ),
                ]),
            ~expected=[
              Next(1),
              Next(2),
              CompleteWithException(Division_by_zero),
            ],
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
                Observable.create(observer => {
                  observer |> Observer.next(10);
                  observer |> Observer.next(20);
                  observer |> Observer.complete;
                }),
            ~expected=[Next(10), Next(20), Complete],
            (),
          ),
          observableIt(
            "onSubscribe throws synchronously before completing",
            ~nextToString=string_of_int,
            ~source=_ => Observable.create(_ => raise(Division_by_zero)),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          it(
            "throws when onSubscribe complete synchronously and then throws an exception",
            () => {
            let observable =
              Observable.create(observer => {
                observer |> Observer.complete;
                raise(Division_by_zero);
              });

            (() => observable |> Observable.subscribe) |> Expect.shouldRaise;
          }),
        ],
      ),
      describe(
        "debounce",
        [
          observableIt(
            "debounces",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay}) =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=scheduleWithDelay,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (15.0, Next(4)),
                    (16.0, Next(5)),
                    (17.0, Next(6)),
                    (18.0, Complete),
                  ],
                )
                |> Observable.debounce(scheduleWithDelay(5.0)),
            ~expected=[Next(3), Next(6), Complete],
            (),
          ),
        ],
      ),
      describe(
        "defaultIfEmpty",
        [
          observableIt(
            "returns the default if empty",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty() |> Observable.defaultIfEmpty(1),
            ~expected=[Next(1), Complete],
            (),
          ),
          observableIt(
            "passes through if not empty",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofList([1, 2, 3]) |> Observable.defaultIfEmpty(1),
            ~expected=[Next(1), Next(2), Next(3), Complete],
            (),
          ),
        ],
      ),
      describe(
        "defer",
        [
          it("calls the observable factory on subscribe", () => {
            let count = ref(0);

            let observable =
              Observable.defer(() => {
                incr(count);
                Observable.empty();
              });

            observable |> Observable.subscribe |> ignore;
            observable |> Observable.subscribe |> ignore;
            observable |> Observable.subscribe |> ignore;
            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe(
        "distinctUntilChanged",
        [
          observableIt(
            "removes duplicates",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofList([1, 1, 1, 3, 5, 3, 3, 1])
                |> Observable.distinctUntilChanged(~equals=(===)),
            ~expected=[
              Next(1),
              Next(3),
              Next(5),
              Next(3),
              Next(1),
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe("empty", []),
      describe(
        "every",
        [
          observableIt(
            "returns true for an observer that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.empty() |> Observable.every(i => i > 10),
            ~expected=[Next(true), Complete],
            (),
          ),
          observableIt(
            "completes with false on the first observed value that fails the predicate",
            ~nextToString=string_of_int,
            ~source=
              ({now} as scheduler) =>
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, Next(12)),
                    (2.0, Next(8)),
                    (3.0, Next(14)),
                    (5.0, Next(6)),
                    (6.0, Complete),
                  ],
                )
                |> Observable.every(i => i > 10)
                |> Observable.map(_ => now() |> int_of_float),
            ~expected=[Next(2), Complete],
            (),
          ),
          observableIt(
            "completes with true if all values pass the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                Observable.ofList([12, 13]) |> Observable.every(i => i > 10),
            ~expected=[Next(true), Complete],
            (),
          ),
        ],
      ),
      describe(
        "exhaust",
        [
          observableIt(
            "exhausts",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay} as scheduler) => {
                let childObservableA =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (0.0, Next(1)),
                      (10.0, Next(2)),
                      (20.0, Next(3)),
                      (30.0, Next(4)),
                      (40.0, Complete),
                    ],
                  );

                let childObservableB =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, Complete),
                    ],
                  );

                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, Complete),
                  ],
                )
                |> Observable.exhaust;
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              Next(4),
              Next(5),
              Next(6),
              Next(7),
              Next(8),
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe(
        "find",
        [
          observableIt(
            "finds the first matching element and completes",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofList([1, 3, 10, 6, 8])
                |> Observable.find(x => x mod 2 === 0),
            ~expected=[Next(10), Complete],
            (),
          ),
        ],
      ),
      describe(
        "first",
        [
          observableIt(
            "publishes the first observed value",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([2, 3]) |> Observable.first,
            ~expected=[Next(2), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ => Observable.raise(Division_by_zero) |> Observable.first,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty() |> Observable.first,
            ~expected=[CompleteWithException(EmptyException.Exn)],
            (),
          ),
        ],
      ),
      describe(
        "firstOrNone",
        [
          observableIt(
            "publishes Some of the first observed value",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.ofList([2, 3]) |> Observable.firstOrNone,
            ~expected=[Next(Some(2)), Complete],
            (),
          ),
          observableIt(
            "publishes Some of the first observed value",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observable.raise(Division_by_zero) |> Observable.firstOrNone,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observable.empty()
                |> Observable.first
                |> Observable.firstOrNone,
            ~expected=[Next(None), Complete],
            (),
          ),
        ],
      ),
      describe(
        "ignoreElements",
        [
          observableIt(
            "ignores all elements and publishes an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.ignoreElements,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
        ],
      ),
      describe(
        "isEmpty",
        [
          observableIt(
            "return false if not empty",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.ofValue(1) |> Observable.isEmpty,
            ~expected=[Next(false), Complete],
            (),
          ),
          observableIt(
            "return true if empty",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.empty() |> Observable.isEmpty,
            ~expected=[Next(true), Complete],
            (),
          ),
        ],
      ),
      describe(
        "keep",
        [
          observableIt(
            "completes the observer when the predicate throws an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofValue(1)
                |> Observable.keep(_ => raise(Division_by_zero)),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes the observer when the keep observer is completed",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofValue(1) |> Observable.keep(_ => true),
            ~expected=[Next(1), Complete],
            (),
          ),
        ],
      ),
      describe(
        "last",
        [
          observableIt(
            "publishes the last observed value and disposes",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]) |> Observable.last,
            ~expected=[Next(3), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  Next(3),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.last,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes with exception if no values are produced",
            ~nextToString=string_of_int,
            ~source=_ => Observable.empty() |> Observable.last,
            ~expected=[CompleteWithException(EmptyException.Exn)],
            (),
          ),
        ],
      ),
      describe(
        "lastOrNone",
        [
          observableIt(
            "publishes the Some of the last observed value and completes",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=_ => Observable.ofList([2, 3]) |> Observable.lastOrNone,
            ~expected=[Next(Some(3)), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.lastOrNone,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException, publishes None, and completes normally",
            ~nextEquals=Option.equals,
            ~nextToString=Option.toString(~toString=string_of_int),
            ~source=
              _ =>
                Observable.empty() |> Observable.first |> Observable.lastOrNone,
            ~expected=[Next(None), Complete],
            (),
          ),
        ],
      ),
      describe(
        "map",
        [
          observableIt(
            "completes the observer when the mapper throws an exception",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofList([1, 2, 3])
                |> Observable.map(_ => raise(Division_by_zero)),
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "completes the observer when the mapping observer is completed",
            ~nextToString=string_of_int,
            ~source=
              _ => Observable.ofList([1, 2, 3]) |> Observable.map(i => i + 1),
            ~expected=[Next(2), Next(3), Next(4), Complete],
            (),
          ),
        ],
      ),
      describe(
        "mapTo",
        [
          observableIt(
            "maps any input to value",
            ~nextToString=Functions.identity,
            ~source=
              _ => Observable.ofList([1, 2, 3]) |> Observable.mapTo("a"),
            ~expected=[Next("a"), Next("a"), Next("a"), Complete],
            (),
          ),
        ],
      ),
      describe(
        "maybeFirst",
        [
          observableIt(
            "publishes the first observed value",
            ~nextToString=string_of_int,
            ~source=
              _ => Observable.ofList([1, 2, 3]) |> Observable.maybeFirst,
            ~expected=[Next(1), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.raise(Division_by_zero) |> Observable.maybeFirst,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.empty() |> Observable.first |> Observable.maybeFirst,
            ~expected=[Complete],
            (),
          ),
        ],
      ),
      describe(
        "maybeLast",
        [
          observableIt(
            "publishes the last observed value",
            ~nextToString=string_of_int,
            ~source=_ => Observable.ofList([1, 2, 3]) |> Observable.maybeLast,
            ~expected=[Next(3), Complete],
            (),
          ),
          observableIt(
            "passes through completed exceptions",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofNotifications([
                  Next(1),
                  Next(2),
                  CompleteWithException(Division_by_zero),
                ])
                |> Observable.maybeLast,
            ~expected=[CompleteWithException(Division_by_zero)],
            (),
          ),
          observableIt(
            "ignores EmptyException and completes normally",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.empty() |> Observable.first |> Observable.maybeLast,
            ~expected=[Complete],
            (),
          ),
        ],
      ),
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
                    ~scheduler=scheduler.scheduleWithDelay(2.0),
                    Division_by_zero,
                  ),
                ]),
            ~expected=[Next(1), CompleteWithException(Division_by_zero)],
            (),
          ),
        ],
      ),
      describe("never", []),
      describe("none", []),
      describe("observeOn", []),
      describe(
        "onComplete",
        [
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofList([1, 2])
                |> Observable.onComplete(Functions.alwaysUnit),
            ~expected=[Next(1), Next(2), Complete],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            Observable.ofList([1])
            |> Observable.onComplete(_ => incr(sideEffectCount))
            |> Observable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe(
        "onNext",
        [
          observableIt(
            "passes through notifications",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofList([1, 2])
                |> Observable.onNext(Functions.alwaysUnit),
            ~expected=[Next(1), Next(2), Complete],
            (),
          ),
          it("calls the side effect function", () => {
            let sideEffectCount = ref(0);

            Observable.ofList([1])
            |> Observable.onNext(_ => incr(sideEffectCount))
            |> Observable.subscribe
            |> ignore;

            sideEffectCount^ |> Expect.toBeEqualToInt(1);
          }),
        ],
      ),
      describe("ofList", []),
      describe("ofValue", []),
      describe("publish", []),
      describe("raise", []),
      describe(
        "retry",
        [
          observableIt(
            "with cold observable",
            ~nextToString=string_of_int,
            ~source=
              _ => {
                let retryCount = ref(0);
                let predicate = _ => {
                  let retry = retryCount^ < 2;
                  if (retry) {
                    Interlocked.increment(retryCount) |> ignore;
                  };
                  retry;
                };

                Observable.create(observer => {
                  observer |> Observer.next(1);
                  observer |> Observer.next(2);
                  observer |> Observer.complete(~exn=Division_by_zero);
                })
                |> Observable.retry(~predicate);
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(1),
              Next(2),
              Next(1),
              Next(2),
              CompleteWithException(Division_by_zero),
            ],
            (),
          ),
          it("with hot observable", () => {
            let result = ref([]);
            let subject = ref(Subject.create());

            let subscription =
              Observable.create(observer => {
                subject := Subject.create();
                let observable = subject^ |> Subject.asObservable;
                let subscription =
                  observable
                  |> Observable.subscribeWith(
                       ~onNext=Observer.forwardOnNext(observer),
                       ~onComplete=Observer.forwardOnComplete(observer),
                     );

                observer
                |> Observer.addDisposable(
                     CompositeDisposable.asDisposable(subscription),
                   )
                |> ignore;
              })
              |> Observable.retry
              |> Observable.subscribe(~onNext=x => result := [x, ...result^]);

            let observer = subject^;
            observer |> Subject.next(1);
            observer |> Subject.next(2);
            observer |> Subject.complete(~exn=Division_by_zero);
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToFalse;

            let observer = subject^;
            observer |> Subject.next(3);
            observer |> Subject.next(4);
            observer |> Subject.complete;
            subscription
            |> CompositeDisposable.isDisposed
            |> Expect.toBeEqualToTrue;

            result^ |> Expect.toBeEqualToListOfInt([4, 3, 2, 1]);
          }),
          it("doesn't retry if unsubscribed in shouldRetry callback", () => {
            let vts = VirtualTimeScheduler.create();
            let scheduler = vts |> VirtualTimeScheduler.toClockScheduler;

            let subscription = ref(CompositeDisposable.disposed);
            subscription :=
              Observable.ofAbsoluteTimeNotifications(
                ~scheduler,
                [
                  (1.0, Next(5)),
                  (2.0, CompleteWithException(Division_by_zero)),
                ],
              )
              |> Observable.retry(~predicate=_ => {
                   subscription^ |> CompositeDisposable.dispose;
                   true;
                 })
              |> expectObservableToProduce(
                   ~nextToString=string_of_int,
                   [Next(5), Complete],
                 );

            vts |> VirtualTimeScheduler.run;
          }),
        ],
      ),
      describe(
        "scan",
        [
          observableIt(
            "publishes all intermediate values, including the initial accumulator value",
            ~nextToString=string_of_int,
            ~source=
              _ =>
                Observable.ofList([2, 3, 4])
                |> Observable.scan((acc, next) => acc + next, 0),
            ~expected=[Next(0), Next(2), Next(5), Next(9), Complete],
            (),
          ),
        ],
      ),
      describe(
        "some",
        [
          observableIt(
            "returns false for an observer that completes without producing values",
            ~nextToString=string_of_bool,
            ~source=_ => Observable.empty() |> Observable.some(i => i > 10),
            ~expected=[Next(false), Complete],
            (),
          ),
          observableIt(
            "returns false for an observer for which no value passes the predicate",
            ~nextToString=string_of_bool,
            ~source=
              _ =>
                Observable.ofList([5, 6, 7]) |> Observable.some(i => i > 10),
            ~expected=[Next(false), Complete],
            (),
          ),
          observableIt(
            "completes with true on the first observed value that passed the predicate",
            ~nextToString=string_of_int,
            ~source=
              ({now} as scheduler) =>
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (1.0, Next(8)),
                    (2.0, Next(11)),
                    (3.0, Next(14)),
                    (4.0, Next(6)),
                    (5.0, Next(5)),
                    (6.0, Complete),
                  ],
                )
                |> Observable.some(i => i > 10)
                |> Observable.map(_ => now() |> int_of_float),
            ~expected=[Next(2), Complete],
            (),
          ),
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
      describe(
        "switch_",
        [
          observableIt(
            "switches",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay} as scheduler) => {
                let childObservableA =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (0.0, Next(1)),
                      (10.0, Next(2)),
                      (20.0, Next(3)),
                      (30.0, Next(4)),
                      (40.0, Complete),
                    ],
                  );

                let childObservableB =
                  Observable.ofRelativeTimeNotifications(
                    ~scheduler=scheduleWithDelay,
                    [
                      (0.0, Next(5)),
                      (10.0, Next(6)),
                      (19.0, Next(7)),
                      (30.0, Next(8)),
                      (40.0, Complete),
                    ],
                  );

                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(childObservableA)),
                    (15.0, Next(childObservableB)),
                    (35.0, Next(childObservableA)),
                    (60.0, Next(childObservableB)),
                    (75.0, Complete),
                  ],
                )
                |> Observable.switch_;
              },
            ~expected=[
              Next(1),
              Next(2),
              Next(5),
              Next(6),
              Next(7),
              Next(1),
              Next(2),
              Next(3),
              Next(5),
              Next(6),
              Complete,
            ],
            (),
          ),
        ],
      ),
      describe("synchronize", []),
      describe(
        "timeout",
        [
          observableIt(
            "when timeout does not expire",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay}) =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=scheduleWithDelay,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (10.0, Next(4)),
                    (14.0, Complete),
                  ],
                )
                |> Observable.timeout(scheduleWithDelay(5.0)),
            ~expected=[Next(1), Next(2), Next(3), Next(4), Complete],
            (),
          ),
          observableIt(
            "when timeout expires",
            ~nextToString=string_of_int,
            ~source=
              ({scheduleWithDelay}) =>
                Observable.ofRelativeTimeNotifications(
                  ~scheduler=scheduleWithDelay,
                  [
                    (0.0, Next(1)),
                    (4.0, Next(2)),
                    (6.0, Next(3)),
                    (15.0, Next(4)),
                    (20.0, Complete),
                  ],
                )
                |> Observable.timeout(scheduleWithDelay(5.0)),
            ~expected=[
              Next(1),
              Next(2),
              Next(3),
              CompleteWithException(TimeoutException.Exn),
            ],
            (),
          ),
        ],
      ),
      describe(
        "withLatestFrom",
        [
          observableIt(
            "drops values from the source, if there is no latest value",
            ~nextToString=string_of_int,
            ~source=
              scheduler =>
                Observable.ofAbsoluteTimeNotifications(
                  ~scheduler,
                  [
                    (0.0, Next(1)),
                    (200.0, Next(2)),
                    (400.0, Next(3)),
                    (600.0, Next(4)),
                    (700.0, Complete),
                  ],
                )
                |> Observable.withLatestFrom(
                     ~selector=(a, b) => a + b,
                     Observable.ofAbsoluteTimeNotifications(
                       ~scheduler,
                       [
                         (100.0, Next(1)),
                         (250.0, Next(2)),
                         (300.0, Next(3)),
                         (450.0, Next(4)),
                         (500.0, Complete),
                       ],
                     ),
                   ),
            ~expected=[Next(3), Next(6), Next(8), Complete],
            (),
          ),
        ],
      ),
    ],
  );