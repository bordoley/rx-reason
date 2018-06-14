open ReUnit;
open ReUnit.Test;

let test =
  describe(
    "Observable",
    [
      describe(
        "combineLatest2",
        [
          it("combines latest values from each subject", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let result = ref([]);

            Observable.combineLatest2(
              ~selector=(a, b) => [a, b],
              subject0 |> Subject.toObservable,
              subject1 |> Subject.toObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;

            observer0 |> Observer.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer1 |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([0, 1]);
          }),
          it("completes only when all observables complete", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest2(
                ~selector=(a, b) => [a, b],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;

            observer0 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer1 |> Observer.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes when the first observable completes with an exception",
            () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest2(
                ~selector=(a, b) => [a, b],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;
            let observer0 = subject0 |> Subject.toObserver;

            observer0 |> Observer.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            result^ |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe(
        "combineLatest3",
        [
          it("combines latest values from each subject", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();

            let result = ref([]);

            Observable.combineLatest3(
              ~selector=(a, b, c) => [a, b, c],
              subject0 |> Subject.toObservable,
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
            )
            |> Observable.subscribe(~onNext=next => result := next)
            |> ignore;

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer0 |> Observer.next(0);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer1 |> Observer.next(1);
            result^ |> Expect.toBeEqualToListOfInt([]);
            observer2 |> Observer.next(2);
            result^ |> Expect.toBeEqualToListOfInt([0, 1, 2]);
          }),
          it("completes only when all observables complete", () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest3(
                ~selector=(a, b, c) => [a, b, c],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            let observer0 = subject0 |> Subject.toObserver;
            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer0 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer1 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            observer2 |> Observer.complete;
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes when the first observable completes with an exception",
            () => {
            let subject0 = Subject.create();
            let subject1 = Subject.create();
            let subject2 = Subject.create();

            let result = ref(false);

            let subscription =
              Observable.combineLatest3(
                ~selector=(a, b, c) => [a, b, c],
                subject0 |> Subject.toObservable,
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
              )
              |> Observable.subscribe(~onComplete=_ => result := true);

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            result^ |> Expect.toBeEqualToFalse;
            let observer1 = subject1 |> Subject.toObserver;

            observer1 |> Observer.complete(~exn=Division_by_zero);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            result^ |> Expect.toBeEqualToTrue;
          }),
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
          it("mixing async and sync observables", () => {
            let result = ref([]);
            let subject0 = Subject.create();
            let subject1 = Subject.create();

            let subscription =
              Observable.concat([
                Observable.ofList([1, 2, 3]),
                subject0 |> Subject.toObservable,
                Observable.ofList([7, 8, 9]),
                subject1 |> Subject.toObservable,
              ])
              |> Observable.subscribe(~onNext=x => result := [x, ...result^]);
            result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);

            subject0 |> Subject.toObserver |> Observer.next(4);
            subject0 |> Subject.toObserver |> Observer.next(5);
            subject0 |> Subject.toObserver |> Observer.next(6);
            subject0 |> Subject.toObserver |> Observer.complete;
            subject1 |> Subject.toObserver |> Observer.next(10);
            subject1 |> Subject.toObserver |> Observer.next(11);
            subject1 |> Subject.toObserver |> Observer.complete;

            result^
            |> Expect.toBeEqualToListOfInt([
                 11,
                 10,
                 9,
                 8,
                 7,
                 6,
                 5,
                 4,
                 3,
                 2,
                 1,
               ]);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
          it(
            "completes with an exception when the first observable completes with an exception",
            () => {
              let result = ref([]);
              let err = ref(None);
              Observable.concat([
                Observable.ofList([1, 2, 3]),
                Observable.raise(Division_by_zero),
                Observable.ofList([4, 5, 6]),
              ])
              |> Observable.subscribeWithCallbacks(
                   ~onNext=x => result := [x, ...result^],
                   ~onComplete=exn => err := exn,
                 )
              |> ignore;
              result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);
              switch (err^) {
              | Some(Division_by_zero) => ()
              | _ => failwith("expected Division_by_zero exception")
              };
            },
          ),
        ],
      ),
      describe(
        "create",
        [
          it(
            "onNext and onComplete function calls delegate to the subscriber",
            () => {
            let observable =
              Observable.create((~onNext, ~onComplete) => {
                onNext(10);
                onNext(20);
                onComplete(None);
                Disposable.disposed;
              });

            let value = ref(0);
            let complete = ref(false);
            let subscription =
              observable
              |> Observable.subscribeWithCallbacks(
                   ~onNext=next => value := next,
                   ~onComplete=_ => complete := true,
                 );

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
            value^ |> Expect.toBeEqualToInt(20);
            complete^ |> Expect.toBeEqualToTrue;
          }),
          it("onSubscribe throws synchronously before completing", () => {
            let observable =
              Observable.create((~onNext as _, ~onComplete as _) =>
                raise(Division_by_zero)
              );

            let complete = ref(None);
            observable
            |> Observable.subscribe(~onComplete=exn => complete := exn)
            |> ignore;

            switch (complete^) {
            | Some(Division_by_zero) => ()
            | _ =>
              failwith("expected subscription to complete with exception")
            };
          }),
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

            let subscription = observable |> Observable.subscribe;

            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;

            observable |> Observable.subscribe |> ignore;

            observable |> Observable.subscribe |> ignore;

            count^ |> Expect.toBeEqualToInt(3);
          }),
        ],
      ),
      describe("empty", []),
      describe(
        "lift",
        [
          it("applies the operator", () =>
            Observable.ofList([1, 2, 3])
            |> Observable.lift(
                 Operators.(
                   scan((acc, next) => acc + next, 0)
                   >> Operators.map(string_of_int)
                   >> Operators.last
                 ),
               )
            |> Observable.subscribe(
                 ~onNext=Expect.toBeEqualToString("6"),
               )
            |> Disposable.isDisposed
            |> Expect.toBeEqualToTrue
          ),
        ],
      ),
      describe(
        "merge",
        [
          it("merges host and cold observables", () => {
            let result = ref([]);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            Observable.merge([
              subject1 |> Subject.toObservable,
              Observable.ofList([1, 2, 3]),
              subject2 |> Subject.toObservable,
              Observable.ofList([4, 5, 6]),
            ])
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
               )
            |> ignore;

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer1 |> Observer.next(7);
            observer2 |> Observer.next(8);
            observer1 |> Observer.next(9);
            result^
            |> Expect.toBeEqualToListOfInt([9, 8, 7, 6, 5, 4, 3, 2, 1]);
          }),
          it("merges hot observables", () => {
            let result = ref([]);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();
            Observable.merge([
              subject1 |> Subject.toObservable,
              subject2 |> Subject.toObservable,
              subject3 |> Subject.toObservable,
            ])
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
               )
            |> ignore;

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

            observer1 |> Observer.next(1);
            observer2 |> Observer.next(2);
            observer1 |> Observer.next(3);
            observer2 |> Observer.next(4);
            observer1 |> Observer.complete;
            observer2 |> Observer.next(5);
            observer2 |> Observer.complete;
            observer3 |> Observer.next(6);
            observer3 |> Observer.complete;

            result^ |> Expect.toBeEqualToListOfInt([6, 5, 4, 3, 2, 1]);
          }),
          it("completes when the last observable completes", () => {
            let result = ref(false);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let subscription =
              Observable.merge([
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              ])
              |> Observable.subscribe(
                   ~onComplete=_ =>
                   result := true
                 );

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;
            let observer3 = subject3 |> Subject.toObserver;

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
            "completes early if any observable completes with an exception", () => {
            let result = ref(false);
            let subject1 = Subject.create();
            let subject2 = Subject.create();
            let subject3 = Subject.create();

            let subscription =
              Observable.merge([
                subject1 |> Subject.toObservable,
                subject2 |> Subject.toObservable,
                subject3 |> Subject.toObservable,
              ])
              |> Observable.subscribe(
                  ~onComplete=_ =>
                   result := true
                 );

            let observer1 = subject1 |> Subject.toObserver;
            let observer2 = subject2 |> Subject.toObserver;

            observer1 |> Observer.complete;
            result^ |> Expect.toBeEqualToFalse;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;
            observer2 |> Observer.complete(~exn=Division_by_zero);
            result^ |> Expect.toBeEqualToTrue;
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToTrue;
          }),
        ],
      ),
      describe("never", []),
      describe("ofList", []),
      describe("ofValue", []),
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
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
               )
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
              |> Observable.subscribe(
                   ~onNext=x => result := [x, ...result^],
                 );

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
              |> Observable.subscribe(
                   ~onNext=x => result := [x, ...result^],
                 );

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
          it("prepends values", () => {
            let result = ref([]);
            let subject = Subject.create();
            let subscription =
              Observable.startWithList(
                [1, 2, 3],
                subject |> Subject.toObservable,
              )
              |> Observable.subscribe(
                   ~onNext=x => result := [x, ...result^],
                 );
            result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);
            subscription |> Disposable.isDisposed |> Expect.toBeEqualToFalse;

            let observer = subject |> Subject.toObserver;
            observer |> Observer.next(4);
            observer |> Observer.next(5);

            result^ |> Expect.toBeEqualToListOfInt([5, 4, 3, 2, 1]);
          }),
        ],
      ),
      describe(
        "startWithValue",
        [
          it("prepends the value", () => {
            let result = ref([]);
            Observable.startWithValue(1, Observable.ofList([2, 3]))
            |> Observable.subscribe(
                 ~onNext=x => result := [x, ...result^],
               )
            |> ignore;
            result^ |> Expect.toBeEqualToListOfInt([3, 2, 1]);
          }),
        ],
      ),
      describe("subscribe", []),
      describe("subscribeOn", []),
    ],
  );