/***
 * Copyright (c) 2017 - present Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
type t('a) = array('a);

let count = (arr: t('a)) : int => Array.length(arr);

let getOrRaise = (index: int, arr: t('a)) : 'a => arr[index];

let reduce =
    (
      ~while_ as predicate: ('acc, 'a) => bool,
      f: ('acc, 'a) => 'acc,
      acc: 'acc,
      arr: t('a),
    )
    : 'acc => {
  let arrCount = count(arr);
  let rec loop = (acc, index) =>
    if (index < arrCount) {
      let next = arr[index];
      if (predicate(acc, next)) {
        let acc = f(acc, arr[index]);
        loop(acc, index + 1);
      } else {
        acc;
      };
    } else {
      acc;
    };
  loop(acc, 0);
};

let forEach = (f: 'a => unit, arr: t('a)) => {
  let arrCount = count(arr);
  let rec loop = index =>
    if (index < arrCount) {
      let next = arr[index];
      f(next);
      loop(index + 1);
    };
  loop(0);
};

let findIndex = (comparer: ('a, 'a) => bool, value: 'a, arr: t('a)) : int => {
  let arrCount = count(arr);
  let rec loop = index =>
    if (index < arrCount) {
      let next = arr[index];
      if (comparer(next, value)) {
        index;
      } else {
        loop(index + 1);
      }
    } else {
      -1;
    };
  loop(0);
};

let addLast = (item: 'a, arr: t('a)) : t('a) => {
  let count = count(arr);
  let retval = Array.make(count + 1, item);
  Array.blit(arr, 0, retval, 0, count);
  retval;
};

let empty = () : t('a) => [||];

let insertAt = (index: int, item: 'a, arr: t('a)) : t('a) => {
  let count = count(arr);
  let retval = Array.make(count + 1, item);
  Array.blit(arr, 0, retval, 0, index);
  Array.blit(arr, index, retval, index + 1, count - index);
  retval;
};

let removeAt = (index: int, arr: t('a)) : t('a) => {
  let count = count(arr);
  let newLength = count - 1;
  let anyItem = arr[0];
  let retval = Array.make(newLength, anyItem);
  Array.blit(arr, 0, retval, 0, index);
  Array.blit(arr, index + 1, retval, index, newLength - index);
  retval;
};

let findAndRemove = (comparer: ('a, 'a) => bool, value: 'a, arr: t('a)) => {
  let index = arr |> findIndex(comparer, value);
  if (index >= 0) {
    arr |> removeAt(index);
  } else {
    arr;
  };
};