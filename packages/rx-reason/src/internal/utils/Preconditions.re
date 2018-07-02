let checkArgument = (state, msg) =>
  if (! state) {
    failwith(msg);
  };