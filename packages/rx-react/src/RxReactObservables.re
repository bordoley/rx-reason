let onNextWithRef = {
  let onNextCallback = (onNext, reactRef, next) => {
    let ele = reactRef |> React.Ref.currentGet;
    switch (ele) {
    | Some(ele) => ele |> onNext(next)
    | None => ()
    };
  };

  (onNext, reactRef, observable) =>
    observable |> RxObservables.onNext2(onNextCallback, onNext, reactRef);
};