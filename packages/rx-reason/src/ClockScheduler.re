module type S = {
  include DelayScheduler.S;
  let getCurrentTime: unit => float;
};