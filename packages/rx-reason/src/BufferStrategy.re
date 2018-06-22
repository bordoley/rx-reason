/**
 * Enumeration defining the strategy for dealing with full fixed width 
 * buffers within Operators. 
 */;
type t =
  | Raise
  | DropLatest
  | DropOldest;