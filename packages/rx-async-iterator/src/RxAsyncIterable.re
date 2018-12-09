let create = (~createResource, ~disposeResource, ~request) => {
  let createIterator = RxAsyncIterator.create(~request, ~disposeResource);

  () => {
    let resource = createResource();
    createIterator(resource);
  };
};

let create1 = (~createResource, ~disposeResource, ~request) => {
  let createIterator = RxAsyncIterator.create(~request, ~disposeResource);

  ctx0 => {
    let resource = createResource(ctx0);
    createIterator(resource);
  };
};

let create2 = (~createResource, ~disposeResource, ~request) => {
  let createIterator = RxAsyncIterator.create(~request, ~disposeResource);

  (ctx0, ctx1) => {
    let resource = createResource(ctx0, ctx1);
    createIterator(resource);
  };
};

let create3 = (~createResource, ~disposeResource, ~request) => {
  let createIterator = RxAsyncIterator.create(~request, ~disposeResource);

  (ctx0, ctx1, ctx2) => {
    let resource = createResource(ctx0, ctx1, ctx2);
    createIterator(resource);
  };
};