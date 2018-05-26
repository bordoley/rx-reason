let debounceTime: (~scheduler: DelayScheduler.t=?, float) => Operator.t('a, 'a);

let defaultIfEmpty: ('a) => Operator.t('a, 'a);

let distinctUntilChanged: (~comparer:('a,'a)=>bool=?) => Operator.t('a, 'a);

let doOnNext: ('a => unit) => Operator.t('a, 'a);

let first: Operator.t('a, 'a);

let firstOrNone: Operator.t('a, option('a));

let identity: Operator.t('a, 'a);

let keep: ('a => bool) => Operator.t('a, 'a);

let last: Operator.t('a, 'a);

let lastOrNone: Operator.t('a, option('a));

let map: ('a => 'b)  =>  Operator.t('a, 'b);

let mapTo: 'b => Operator.t('a, 'b);

let maybe: Operator.t('a, 'a);

let maybeFirst: Operator.t('a, 'a);

let maybeLast: Operator.t('a, 'a);

let scan: (('acc, 'a) => 'acc, 'acc) => Operator.t('a, 'acc);

let switch_: Operator.t(Observable.t('a), 'a);

let switchMap: ('a => Observable.t('b)) => Operator.t('a, 'b);

let synchronize : Operator.t('a, 'a) ;

/* bufferCount */
/* BufferSkipCount */
/* BufferTime */
/* withLatestFrom */