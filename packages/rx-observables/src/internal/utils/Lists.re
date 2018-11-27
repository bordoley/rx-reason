let length = {
  let rec loop = (count, list) => switch(list) {
    | [] => count
    | [_, ...tail] => loop(count + 1, tail); 
  };
  list => loop(0, list);
};

let rev = {
  let rec loop = (result, list) => switch(list) {
    | [] => result;
    | [hd, ...tail] => {
      let result = [hd, ...result];
      loop(result, tail);
    };
  };

  list => loop([], list);
};

let mapTailRecursive = {
  let rec loop = (f, result, list) => switch(list) {
    | [] => result;
    | [hd, ...tail] => {
      let result = [f(hd), ...result];
      loop(f, result, tail);
    };
  };
  (f, list) => loop(f, [], list) |> rev;
};