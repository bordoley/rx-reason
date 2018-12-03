let create = observable =>
  ShareWithSubjectFactoryObservable.create(
    RxSubject.createMulticast,
    observable,
  );