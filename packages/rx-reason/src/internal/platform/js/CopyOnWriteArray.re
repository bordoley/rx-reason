type t('a) = array('a);

[@bs.new] external newArrayUnsafe : int => array('a) = "Array";

[@bs.val] external getUnsafe : int => array('a) => 'a = "";
%bs.raw
{|
function get(index, array) {
  return array[index];
}|};


[@bs.val] external setUnsafe : int => 'a => array('a) => unit = "";
%bs.raw
{|
function set(index, value, array) {
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
  let count = count(arr);

  if (count < maxCount) {
    addLast(value, arr);
  } else {
    let retval = newArrayUnsafe(maxCount);

    for (index in 1 to maxCount - 1) {
      let v = getUnsafe(index, arr);
      setUnsafe(index -1, v, retval);
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