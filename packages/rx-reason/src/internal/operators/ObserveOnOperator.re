let operator = (~bufferStrategy=?, ~bufferSize=?, scheduler) => 
  DelayOperator.operator(       ~bufferStrategy?,
  ~bufferSize?,
  ~scheduler,
  ~delay=0.0,)

let lift = (~bufferStrategy=?, ~bufferSize=?, scheduler, observable) =>
  observable
  |> DelayOperator.lift(
       ~bufferStrategy?,
       ~bufferSize?,
       ~scheduler,
       ~delay=0.0,
     );