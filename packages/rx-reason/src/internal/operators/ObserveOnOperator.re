let operator = (~bufferStrategy=?, ~bufferSize=?, scheduler) =>
  DelayOperator.operator(
    ~bufferStrategy?,
    ~bufferSize?,
    ~scheduler,
    ~delay=0.0,
  );