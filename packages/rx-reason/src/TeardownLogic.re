type t = unit => unit;
type t1('a) = 'a => unit;
type t2('a, 'b) = ('a, 'b) => unit;
type t3('a, 'b, 'c) = ('a, 'b, 'c) => unit;
type t4('a, 'b, 'c, 'd) = ('a, 'b, 'c, 'd) => unit;
type t5('a, 'b, 'c, 'd, 'e) = ('a, 'b, 'c, 'd, 'e) => unit;
type t6('a, 'b, 'c, 'd, 'e, 'f) = ('a, 'b, 'c, 'd, 'e, 'f) => unit;
type t7('a, 'b, 'c, 'd, 'e, 'f, 'g) = ('a, 'b, 'c, 'd, 'e, 'f, 'g) => unit;