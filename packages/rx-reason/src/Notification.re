type t('a) = 
  | Next('a)
  | Complete
  | CompleteWithException(exn);