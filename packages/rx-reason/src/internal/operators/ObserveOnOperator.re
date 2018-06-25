let lift =
    (~bufferStrategy=?, ~bufferSize=?, scheduler, observable) =>
  observable
  |> DelayOperator.lift(~bufferStrategy?, ~bufferSize?, ~scheduler, ~delay=0.0);