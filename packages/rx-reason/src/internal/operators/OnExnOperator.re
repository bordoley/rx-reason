let operator = onError =>
  ObserveOperator.operator(~onNext=Functions.alwaysUnit1, ~onComplete=maybeExn =>
    switch (maybeExn) {
    | None => ()
    | Some(exn) => onError(exn)
    }
  );