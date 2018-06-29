type source('a) = Subscriber.t('a) => unit;
type source1('ctx0, 'a) = ('ctx0, Subscriber.t('a)) => unit;
type source2('ctx0, 'ctx1, 'a) = ('ctx0, 'ctx1, Subscriber.t('a)) => unit;
type source3('ctx0, 'ctx1, 'ctx2, 'a) =
  ('ctx0, 'ctx1, 'ctx2, Subscriber.t('a)) => unit;
type source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, Subscriber.t('a)) => unit;
type source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, Subscriber.t('a)) => unit;

type t('a) =
  | Source(source('a)): t('a)
  | Lift1(source('b), Operator.t('b, 'a)): t('a)
  | Lift2(source('c), Operator.t('c, 'b), Operator.t('b, 'a)): t('a)
  | Lift3(
           source('d),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Lift4(
           source('e),
           Operator.t('e, 'd),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Lift5(
           source('f),
           Operator.t('f, 'e),
           Operator.t('e, 'd),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Lift6(
           source('g),
           Operator.t('g, 'f),
           Operator.t('f, 'e),
           Operator.t('e, 'd),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Lift7(
           source('h),
           Operator.t('h, 'g),
           Operator.t('g, 'f),
           Operator.t('f, 'e),
           Operator.t('e, 'd),
           Operator.t('d, 'c),
           Operator.t('c, 'b),
           Operator.t('b, 'a),
         ): t('a)
  | Source1(source1('ctx0, 'a), 'ctx0): t('a)
  | Source1Lift1(source1('ctx0, 'b), 'ctx0, Operator.t('b, 'a)): t('a)
  | Source1Lift2(
                  source1('ctx0, 'c),
                  'ctx0,
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift3(
                  source1('ctx0, 'd),
                  'ctx0,
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift4(
                  source1('ctx0, 'e),
                  'ctx0,
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift5(
                  source1('ctx0, 'f),
                  'ctx0,
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift6(
                  source1('ctx0, 'g),
                  'ctx0,
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source1Lift7(
                  source1('ctx0, 'h),
                  'ctx0,
                  Operator.t('h, 'g),
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source2(source2('ctx0, 'ctx1, 'a), 'ctx0, 'ctx1): t('a)
  | Source2Lift1(
                  source2('ctx0, 'ctx1, 'b),
                  'ctx0,
                  'ctx1,
                  Operator.t('b, 'a),
                ): t('a)
  | Source2Lift2(
                  source2('ctx0, 'ctx1, 'c),
                  'ctx0,
                  'ctx1,
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source2Lift3(
                  source2('ctx0, 'ctx1, 'd),
                  'ctx0,
                  'ctx1,
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source2Lift4(
                  source2('ctx0, 'ctx1, 'e),
                  'ctx0,
                  'ctx1,
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source2Lift5(
                  source2('ctx0, 'ctx1, 'f),
                  'ctx0,
                  'ctx1,
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source2Lift6(
                  source2('ctx0, 'ctx1, 'g),
                  'ctx0,
                  'ctx1,
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source2Lift7(
                  source2('ctx0, 'ctx1, 'h),
                  'ctx0,
                  'ctx1,
                  Operator.t('h, 'g),
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source3(source3('ctx0, 'ctx1, 'ctx2, 'a), 'ctx0, 'ctx1, 'ctx2): t('a)
  | Source3Lift1(
                  source3('ctx0, 'ctx1, 'ctx2, 'b),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  Operator.t('b, 'a),
                ): t('a)
  | Source3Lift2(
                  source3('ctx0, 'ctx1, 'ctx2, 'c),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source3Lift3(
                  source3('ctx0, 'ctx1, 'ctx2, 'd),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source3Lift4(
                  source3('ctx0, 'ctx1, 'ctx2, 'e),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source3Lift5(
                  source3('ctx0, 'ctx1, 'ctx2, 'f),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source3Lift6(
                  source3('ctx0, 'ctx1, 'ctx2, 'g),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source3Lift7(
                  source3('ctx0, 'ctx1, 'ctx2, 'h),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  Operator.t('h, 'g),
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source4(
             source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'a),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
           ): t('a)
  | Source4Lift1(
                  source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'b),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  Operator.t('b, 'a),
                ): t('a)
  | Source4Lift2(
                  source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'c),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source4Lift3(
                  source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'd),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source4Lift4(
                  source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'e),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source4Lift5(
                  source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'f),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source4Lift6(
                  source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'g),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source4Lift7(
                  source4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'h),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  Operator.t('h, 'g),
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source5(
             source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'a),
             'ctx0,
             'ctx1,
             'ctx2,
             'ctx3,
             'ctx4,
           ): t('a)
  | Source5Lift1(
                  source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'b),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  'ctx4,
                  Operator.t('b, 'a),
                ): t('a)
  | Source5Lift2(
                  source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'c),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  'ctx4,
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source5Lift3(
                  source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'd),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  'ctx4,
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source5Lift4(
                  source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'e),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  'ctx4,
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source5Lift5(
                  source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'f),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  'ctx4,
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source5Lift6(
                  source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'g),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  'ctx4,
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a)
  | Source5Lift7(
                  source5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'h),
                  'ctx0,
                  'ctx1,
                  'ctx2,
                  'ctx3,
                  'ctx4,
                  Operator.t('h, 'g),
                  Operator.t('g, 'f),
                  Operator.t('f, 'e),
                  Operator.t('e, 'd),
                  Operator.t('d, 'c),
                  Operator.t('c, 'b),
                  Operator.t('b, 'a),
                ): t('a);

let create = onSubscribe => Source(onSubscribe);
let create1 = (onSubscribe, ctx0) => Source1(onSubscribe, ctx0);
let create2 = (onSubscribe, ctx0, ctx1) => Source2(onSubscribe, ctx0, ctx1);
let create3 = (onSubscribe, ctx0, ctx1, ctx2) =>
  Source3(onSubscribe, ctx0, ctx1, ctx2);
let create4 = (onSubscribe, ctx0, ctx1, ctx2, ctx3) =>
  Source4(onSubscribe, ctx0, ctx1, ctx2, ctx3);
let create5 = (onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4) =>
  Source5(onSubscribe, ctx0, ctx1, ctx2, ctx3, ctx4);

let lift = (operator: Operator.t('a, 'b), observable: t('a)) : t('b) =>
  switch (observable) {
  | Source(source) => Lift1(source, operator)
  | Lift1(source, op0) => Lift2(source, op0, operator)
  | Lift2(source, op0, op1) => Lift3(source, op0, op1, operator)
  | Lift3(source, op0, op1, op2) => Lift4(source, op0, op1, op2, operator)
  | Lift4(source, op0, op1, op2, op3) =>
    Lift5(source, op0, op1, op2, op3, operator)
  | Lift5(source, op0, op1, op2, op3, op4) =>
    Lift6(source, op0, op1, op2, op3, op4, operator)
  | Lift6(source, op0, op1, op2, op3, op4, op5) =>
    Lift7(source, op0, op1, op2, op3, op4, op5, operator)
  | Lift7(source, op0, op1, op2, op3, op4, op5, op6) =>
    let op0 = subscriber =>
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ operator @@ subscriber;
    Lift1(source, op0);
  | Source1(source, ctx0) => Source1Lift1(source, ctx0, operator)
  | Source1Lift1(source, ctx0, op0) =>
    Source1Lift2(source, ctx0, op0, operator)
  | Source1Lift2(source, ctx0, op0, op1) =>
    Source1Lift3(source, ctx0, op0, op1, operator)
  | Source1Lift3(source, ctx0, op0, op1, op2) =>
    Source1Lift4(source, ctx0, op0, op1, op2, operator)
  | Source1Lift4(source, ctx0, op0, op1, op2, op3) =>
    Source1Lift5(source, ctx0, op0, op1, op2, op3, operator)
  | Source1Lift5(source, ctx0, op0, op1, op2, op3, op4) =>
    Source1Lift6(source, ctx0, op0, op1, op2, op3, op4, operator)
  | Source1Lift6(source, ctx0, op0, op1, op2, op3, op4, op5) =>
    Source1Lift7(source, ctx0, op0, op1, op2, op3, op4, op5, operator)
  | Source1Lift7(source, ctx0, op0, op1, op2, op3, op4, op5, op6) =>
    let op0 = subscriber =>
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ operator @@ subscriber;
    Source1Lift1(source, ctx0, op0);
  | Source2(source, ctx0, ctx1) => Source2Lift1(source, ctx0, ctx1, operator)
  | Source2Lift1(source, ctx0, ctx1, op0) =>
    Source2Lift2(source, ctx0, ctx1, op0, operator)
  | Source2Lift2(source, ctx0, ctx1, op0, op1) =>
    Source2Lift3(source, ctx0, ctx1, op0, op1, operator)
  | Source2Lift3(source, ctx0, ctx1, op0, op1, op2) =>
    Source2Lift4(source, ctx0, ctx1, op0, op1, op2, operator)
  | Source2Lift4(source, ctx0, ctx1, op0, op1, op2, op3) =>
    Source2Lift5(source, ctx0, ctx1, op0, op1, op2, op3, operator)
  | Source2Lift5(source, ctx0, ctx1, op0, op1, op2, op3, op4) =>
    Source2Lift6(source, ctx0, ctx1, op0, op1, op2, op3, op4, operator)
  | Source2Lift6(source, ctx0, ctx1, op0, op1, op2, op3, op4, op5) =>
    Source2Lift7(source, ctx0, ctx1, op0, op1, op2, op3, op4, op5, operator)
  | Source2Lift7(source, ctx0, ctx1, op0, op1, op2, op3, op4, op5, op6) =>
    let op0 = subscriber =>
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ operator @@ subscriber;
    Source2Lift1(source, ctx0, ctx1, op0);
  | Source3(source, ctx0, ctx1, ctx2) =>
    Source3Lift1(source, ctx0, ctx1, ctx2, operator)
  | Source3Lift1(source, ctx0, ctx1, ctx2, op0) =>
    Source3Lift2(source, ctx0, ctx1, ctx2, op0, operator)
  | Source3Lift2(source, ctx0, ctx1, ctx2, op0, op1) =>
    Source3Lift3(source, ctx0, ctx1, ctx2, op0, op1, operator)
  | Source3Lift3(source, ctx0, ctx1, ctx2, op0, op1, op2) =>
    Source3Lift4(source, ctx0, ctx1, ctx2, op0, op1, op2, operator)
  | Source3Lift4(source, ctx0, ctx1, ctx2, op0, op1, op2, op3) =>
    Source3Lift5(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, operator)
  | Source3Lift5(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, op4) =>
    Source3Lift6(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, op4, operator)
  | Source3Lift6(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, op4, op5) =>
    Source3Lift7(
      source,
      ctx0,
      ctx1,
      ctx2,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
      operator,
    )
  | Source3Lift7(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, op4, op5, op6) =>
    let op0 = subscriber =>
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ operator @@ subscriber;
    Source3Lift1(source, ctx0, ctx1, ctx2, op0);
  | Source4(source, ctx0, ctx1, ctx2, ctx3) =>
    Source4Lift1(source, ctx0, ctx1, ctx2, ctx3, operator)
  | Source4Lift1(source, ctx0, ctx1, ctx2, ctx3, op0) =>
    Source4Lift2(source, ctx0, ctx1, ctx2, ctx3, op0, operator)
  | Source4Lift2(source, ctx0, ctx1, ctx2, ctx3, op0, op1) =>
    Source4Lift3(source, ctx0, ctx1, ctx2, ctx3, op0, op1, operator)
  | Source4Lift3(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2) =>
    Source4Lift4(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, operator)
  | Source4Lift4(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, op3) =>
    Source4Lift5(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, op3, operator)
  | Source4Lift5(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, op3, op4) =>
    Source4Lift6(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      op0,
      op1,
      op2,
      op3,
      op4,
      operator,
    )
  | Source4Lift6(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, op3, op4, op5) =>
    Source4Lift7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
      operator,
    )
  | Source4Lift7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
      op6,
    ) =>
    let op0 = subscriber =>
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ operator @@ subscriber;
    Source4Lift1(source, ctx0, ctx1, ctx2, ctx3, op0);

  | Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4) =>
    Source5Lift1(source, ctx0, ctx1, ctx2, ctx3, ctx4, operator)
  | Source5Lift1(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0) =>
    Source5Lift2(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, operator)
  | Source5Lift2(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, op1) =>
    Source5Lift3(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, op1, operator)
  | Source5Lift3(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, op1, op2) =>
    Source5Lift4(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      operator,
    )
  | Source5Lift4(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, op1, op2, op3) =>
    Source5Lift5(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      operator,
    )
  | Source5Lift5(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
    ) =>
    Source5Lift6(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
      operator,
    )
  | Source5Lift6(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
    ) =>
    Source5Lift7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
      operator,
    )
  | Source5Lift7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
      op6,
    ) =>
    let op0 = subscriber =>
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ operator @@ subscriber;
    Source5Lift1(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0);
  };

let never = Source(Functions.alwaysUnit1);

let subscribeSafe = (subscriber, source) =>
  try (source(subscriber)) {
  | exn =>
    let shouldRaise =
      subscriber |> Subscriber.completeWithResult(~exn) |> (!);
    if (shouldRaise) {
      /* This could happen when the onComplete is called synchronously in the
       * subscribe function which also throws.
       */
      raise(
        exn,
      );
    };
  };

let subscribeSubscriber = (subscriber, observable) =>
  switch (observable) {
  | Source(source) => source |> subscribeSafe(subscriber)
  | Lift1(source, op0) =>
    let subscriber = op0(subscriber);
    source |> subscribeSafe(subscriber);
  | Lift2(source, op0, op1) =>
    let subscriber = op0 @@ op1 @@ subscriber;
    source |> subscribeSafe(subscriber);
  | Lift3(source, op0, op1, op2) =>
    let subscriber = op0 @@ op1 @@ op2 @@ subscriber;
    source |> subscribeSafe(subscriber);
  | Lift4(source, op0, op1, op2, op3) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
    source |> subscribeSafe(subscriber);
  | Lift5(source, op0, op1, op2, op3, op4) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
    source |> subscribeSafe(subscriber);
  | Lift6(source, op0, op1, op2, op3, op4, op5) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
    source |> subscribeSafe(subscriber);
  | Lift7(source, op0, op1, op2, op3, op4, op5, op6) =>
    let subscriber =
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
    source |> subscribeSafe(subscriber);
  | Source1(source, ctx0) => source(ctx0) |> subscribeSafe(subscriber)
  | Source1Lift1(source, ctx0, op0) =>
    let subscriber = op0(subscriber);
    source(ctx0) |> subscribeSafe(subscriber);
  | Source1Lift2(source, ctx0, op0, op1) =>
    let subscriber = op0 @@ op1 @@ subscriber;
    source(ctx0) |> subscribeSafe(subscriber);
  | Source1Lift3(source, ctx0, op0, op1, op2) =>
    let subscriber = op0 @@ op1 @@ op2 @@ subscriber;
    source(ctx0) |> subscribeSafe(subscriber);
  | Source1Lift4(source, ctx0, op0, op1, op2, op3) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
    source(ctx0) |> subscribeSafe(subscriber);
  | Source1Lift5(source, ctx0, op0, op1, op2, op3, op4) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
    source(ctx0) |> subscribeSafe(subscriber);
  | Source1Lift6(source, ctx0, op0, op1, op2, op3, op4, op5) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
    source(ctx0) |> subscribeSafe(subscriber);
  | Source1Lift7(source, ctx0, op0, op1, op2, op3, op4, op5, op6) =>
    let subscriber =
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
    source(ctx0) |> subscribeSafe(subscriber);
  | Source2(source, ctx0, ctx1) =>
    source(ctx0, ctx1) |> subscribeSafe(subscriber)
  | Source2Lift1(source, ctx0, ctx1, op0) =>
    let subscriber = op0(subscriber);
    source(ctx0, ctx1) |> subscribeSafe(subscriber);
  | Source2Lift2(source, ctx0, ctx1, op0, op1) =>
    let subscriber = op0 @@ op1 @@ subscriber;
    source(ctx0, ctx1) |> subscribeSafe(subscriber);
  | Source2Lift3(source, ctx0, ctx1, op0, op1, op2) =>
    let subscriber = op0 @@ op1 @@ op2 @@ subscriber;
    source(ctx0, ctx1) |> subscribeSafe(subscriber);
  | Source2Lift4(source, ctx0, ctx1, op0, op1, op2, op3) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
    source(ctx0, ctx1) |> subscribeSafe(subscriber);
  | Source2Lift5(source, ctx0, ctx1, op0, op1, op2, op3, op4) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
    source(ctx0, ctx1) |> subscribeSafe(subscriber);
  | Source2Lift6(source, ctx0, ctx1, op0, op1, op2, op3, op4, op5) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
    source(ctx0, ctx1) |> subscribeSafe(subscriber);
  | Source2Lift7(source, ctx0, ctx1, op0, op1, op2, op3, op4, op5, op6) =>
    let subscriber =
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
    source(ctx0, ctx1) |> subscribeSafe(subscriber);
  | Source3(source, ctx0, ctx1, ctx2) =>
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber)
  | Source3Lift1(source, ctx0, ctx1, ctx2, op0) =>
    let subscriber = op0(subscriber);
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber);
  | Source3Lift2(source, ctx0, ctx1, ctx2, op0, op1) =>
    let subscriber = op0 @@ op1 @@ subscriber;
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber);
  | Source3Lift3(source, ctx0, ctx1, ctx2, op0, op1, op2) =>
    let subscriber = op0 @@ op1 @@ op2 @@ subscriber;
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber);
  | Source3Lift4(source, ctx0, ctx1, ctx2, op0, op1, op2, op3) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber);
  | Source3Lift5(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, op4) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber);
  | Source3Lift6(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, op4, op5) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber);
  | Source3Lift7(source, ctx0, ctx1, ctx2, op0, op1, op2, op3, op4, op5, op6) =>
    let subscriber =
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
    source(ctx0, ctx1, ctx2) |> subscribeSafe(subscriber);
  | Source4(source, ctx0, ctx1, ctx2, ctx3) =>
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber)
  | Source4Lift1(source, ctx0, ctx1, ctx2, ctx3, op0) =>
    let subscriber = op0(subscriber);
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber);
  | Source4Lift2(source, ctx0, ctx1, ctx2, ctx3, op0, op1) =>
    let subscriber = op0 @@ op1 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber);
  | Source4Lift3(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2) =>
    let subscriber = op0 @@ op1 @@ op2 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber);
  | Source4Lift4(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, op3) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber);
  | Source4Lift5(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, op3, op4) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber);
  | Source4Lift6(source, ctx0, ctx1, ctx2, ctx3, op0, op1, op2, op3, op4, op5) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber);
  | Source4Lift7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
      op6,
    ) =>
    let subscriber =
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3) |> subscribeSafe(subscriber);

  | Source5(source, ctx0, ctx1, ctx2, ctx3, ctx4) =>
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber)
  | Source5Lift1(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0) =>
    let subscriber = op0(subscriber);
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber);
  | Source5Lift2(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, op1) =>
    let subscriber = op0 @@ op1 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber);
  | Source5Lift3(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, op1, op2) =>
    let subscriber = op0 @@ op1 @@ op2 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber);
  | Source5Lift4(source, ctx0, ctx1, ctx2, ctx3, ctx4, op0, op1, op2, op3) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber);
  | Source5Lift5(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
    ) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber);
  | Source5Lift6(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
    ) =>
    let subscriber = op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber);
  | Source5Lift7(
      source,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      op0,
      op1,
      op2,
      op3,
      op4,
      op5,
      op6,
    ) =>
    let subscriber =
      op0 @@ op1 @@ op2 @@ op3 @@ op4 @@ op5 @@ op6 @@ subscriber;
    source(ctx0, ctx1, ctx2, ctx3, ctx4) |> subscribeSafe(subscriber);
  };

let subscribeWith = (~onNext, ~onComplete, observable) => {
  let subscriber = Subscriber.createAutoDisposing(~onNext, ~onComplete);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith1 = (~onNext, ~onComplete, ctx0, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing1(~onNext, ~onComplete, ctx0);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith2 = (~onNext, ~onComplete, ctx0, ctx1, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing2(~onNext, ~onComplete, ctx0, ctx1);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith3 = (~onNext, ~onComplete, ctx0, ctx1, ctx2, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing3(~onNext, ~onComplete, ctx0, ctx1, ctx2);

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith4 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing4(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
    );

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith5 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing5(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
    );

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith6 =
    (~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5, observable) => {
  let subscriber =
    Subscriber.createAutoDisposing6(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
    );

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribeWith7 =
    (
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      observable,
    ) => {
  let subscriber =
    Subscriber.createAutoDisposing7(
      ~onNext,
      ~onComplete,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
    );

  subscribeSubscriber(subscriber, observable);
  subscriber |> Subscriber.asDisposable;
};

let subscribe =
    (
      ~onNext=Functions.alwaysUnit1,
      ~onComplete=Functions.alwaysUnit1,
      observable,
    ) =>
  observable |> subscribeWith(~onNext, ~onComplete);

let subscribe1 =
    (
      ~onNext=Functions.alwaysUnit2,
      ~onComplete=Functions.alwaysUnit2,
      ctx0,
      observable,
    ) =>
  observable |> subscribeWith1(~onNext, ~onComplete, ctx0);

let subscribe2 =
    (
      ~onNext=Functions.alwaysUnit3,
      ~onComplete=Functions.alwaysUnit3,
      ctx0,
      ctx1,
      observable,
    ) =>
  observable |> subscribeWith2(~onNext, ~onComplete, ctx0, ctx1);

let subscribe3 =
    (
      ~onNext=Functions.alwaysUnit4,
      ~onComplete=Functions.alwaysUnit4,
      ctx0,
      ctx1,
      ctx2,
      observable,
    ) =>
  observable |> subscribeWith3(~onNext, ~onComplete, ctx0, ctx1, ctx2);

let subscribe4 =
    (
      ~onNext=Functions.alwaysUnit5,
      ~onComplete=Functions.alwaysUnit5,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      observable,
    ) =>
  observable |> subscribeWith4(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3);

let subscribe5 =
    (
      ~onNext=Functions.alwaysUnit6,
      ~onComplete=Functions.alwaysUnit6,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      observable,
    ) =>
  observable
  |> subscribeWith5(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4);

let subscribe6 =
    (
      ~onNext=Functions.alwaysUnit7,
      ~onComplete=Functions.alwaysUnit7,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      observable,
    ) =>
  observable
  |> subscribeWith6(~onNext, ~onComplete, ctx0, ctx1, ctx2, ctx3, ctx4, ctx5);

let subscribe7 =
    (
      ~onNext=Functions.alwaysUnit8,
      ~onComplete=Functions.alwaysUnit8,
      ctx0,
      ctx1,
      ctx2,
      ctx3,
      ctx4,
      ctx5,
      ctx6,
      observable,
    ) =>
  observable
  |> subscribeWith7(
       ~onNext,
       ~onComplete,
       ctx0,
       ctx1,
       ctx2,
       ctx3,
       ctx4,
       ctx5,
       ctx6,
     );

let publishTo = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscription = observable |> subscribeWith(~onNext, ~onComplete);
        let newConnection =
          Disposable.create2(teardown, subscription, active);

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publishTo1 = {
  let teardown = (subscription, active) => {
    subscription |> Disposable.dispose;
    Atomic.set(active, false);
  };

  (~onNext, ~onComplete, ctx0, observable) => {
    let connection = Atomic.make(Disposable.disposed);
    let active = Atomic.make(false);

    () => {
      if (! Atomic.exchange(active, true)) {
        let subscription =
          observable |> subscribeWith1(~onNext, ~onComplete, ctx0);
        let newConnection =
          Disposable.create2(teardown, subscription, active);

        Atomic.set(connection, newConnection);
      };
      Atomic.get(connection);
    };
  };
};

let publish =
    (
      ~onNext=Functions.alwaysUnit1,
      ~onComplete=Functions.alwaysUnit1,
      observable,
    ) =>
  publishTo(~onNext, ~onComplete, observable);

let publish1 =
    (
      ~onNext=Functions.alwaysUnit2,
      ~onComplete=Functions.alwaysUnit2,
      ctx,
      observable,
    ) =>
  publishTo1(~onNext, ~onComplete, ctx, observable);