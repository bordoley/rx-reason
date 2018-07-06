module Result = {
  type t('state) =
    | Done
    | Continue('state)
    | ContinueAfter(float, 'state);
};

  
type t('state) = 'state => Result.t('state);
type t1('ctx0, 'state) = ('ctx0, 'state) => Result.t('state);
type t2('ctx0, 'ctx1, 'state) = ('ctx0, 'ctx1, 'state) => Result.t('state);
type t3('ctx0, 'ctx1, 'ctx2, 'state) =
  ('ctx0, 'ctx1, 'ctx2, 'state) => Result.t('state);
type t4('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'state) => Result.t('state);
type t5('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'state) => Result.t('state);
type t6('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'state) => Result.t('state);
type t7('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state) =
  ('ctx0, 'ctx1, 'ctx2, 'ctx3, 'ctx4, 'ctx5, 'ctx6, 'state) =>
  Result.t('state);