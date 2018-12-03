let create = count => {
  let createSubject = () => RxReplayBufferSubject.create(count);
  obs => ShareWithSubjectFactoryObservable.create(createSubject, obs);
};