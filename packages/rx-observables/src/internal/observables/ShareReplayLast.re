let create = obs =>
  ShareWithSubjectFactoryObservable.create(RxSubject.createReplayLast, obs);