/**
 * Represents reactive event types and their values.
 */;
 
type t('a) =
  | Next('a)
  | Complete(option(exn));

let complete: (option(exn)) => t('a);
let next: 'a => t('a);