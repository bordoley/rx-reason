type t('a) = array('a);

[@bs.new] external newArrayUnsafe : int => array('a) = "Array";

[@bs.val] external getUnsafe : (int, array('a)) => 'a = "";
%bs.raw
{|
function getUnsafe(index, array) {
  return array[index];
}|};

[@bs.val] external setUnsafe : (int, 'a, array('a)) => unit = "";
%bs.raw
{|
function setUnsafe(index, value, array) {
  array[index] = value;
}|};

let count = Js.Array.length;

let empty = () => [||];

let forEach = Js.Array.forEach;

let addLast = (value, arr) => {
  let count = count(arr);
  let newLength = count + 1;
  let retval = newArrayUnsafe(newLength);

  for (index in 0 to count - 1) {
    let v = getUnsafe(index, arr);
    setUnsafe(index, v, retval);
  };

  setUnsafe(count, value, retval);
  retval;
};

let addLastWithMaxCount = (maxCount, value, arr) => {
  RxPreconditions.checkArgument(
    maxCount >= 0,
    "max count must be greater than or equal to 0",
  );
  let count = count(arr);
  RxPreconditions.checkArgument(
    maxCount >= count,
    "max count must be less than or equal to 0",
  );

  if (count < maxCount) {
    addLast(value, arr);
  } else {
    let retval = newArrayUnsafe(maxCount);

    for (index in 1 to maxCount - 1) {
      let v = getUnsafe(index, arr);
      setUnsafe(index - 1, v, retval);
    };

    setUnsafe(maxCount - 1, value, retval);
    retval;
  };
};

let findAndRemoveReference = (value, arr) => {
  let index = Js.Array.indexOf(value, arr);
  if (index < 0) {
    arr;
  } else {
    let count = count(arr);
    let newLength = count - 1;
    let retval = newArrayUnsafe(newLength);

    for (index in 0 to index - 1) {
      let v = getUnsafe(index, arr);
      setUnsafe(index, v, retval);
    };

    for (index in index + 1 to count - 1) {
      let v = getUnsafe(index, arr);
      setUnsafe(index - 1, v, retval);
    };
    retval;
  };
};