exception JSPromiseErrorException(Js.Promise.error);

let toSingle: Js.Promise.t('a) => RxReason.Single.t('a);

let fromSingle: RxReason.Single.t('a) => Js.Promise.t('a);
