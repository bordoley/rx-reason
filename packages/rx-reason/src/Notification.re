type t('a) = 
  | Next('a)
  | Complete(option(exn));