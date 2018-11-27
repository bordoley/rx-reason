let operator = onError =>
  ObserveOperator.operator(~onNext=RxFunctions.alwaysUnit1, ~onComplete=maybeExn =>
    switch (maybeExn) {
    | None => ()
    | Some(exn) => onError(exn)
    }
  );