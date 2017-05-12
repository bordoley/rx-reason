type t = (unit => Subscription.t) => Subscription.t;

let immediate: t = fun (work: unit => Subscription.t): Subscription.t =>
  work ();

let schedule (work: unit => Subscription.t) (schedule: t): Subscription.t =>
  schedule work;
