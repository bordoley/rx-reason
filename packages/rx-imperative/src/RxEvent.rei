type t('a);

include RxDisposable.S1 with type t('a) := t('a);
include RxEventLike.S1 with type t('a) := t('a);

let create: unit => t('a);
let disposed: t('a);