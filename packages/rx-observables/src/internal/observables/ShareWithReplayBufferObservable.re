let create = count => {
  RxPreconditions.checkArgument(
    count > 0,
    "ShareWithReplayBufferObservable: count must be greater than 0.",
  );

  let createSubject = () => RxReplayBufferSubject.create(count);
  obs => ShareWithSubjectFactoryObservable.create(createSubject, obs);
};