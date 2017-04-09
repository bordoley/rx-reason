type t = (unit => Subscription.t) => Subscription.t;

let immediate (work: unit => Subscription.t): Subscription.t =>
  work ();
