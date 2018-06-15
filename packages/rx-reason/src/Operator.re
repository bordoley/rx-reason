/**
 * Fuction type to map/wrap a downstream Observer to an upstream Observer.
 */;
type t('a, 'b) = Observer.t('b) => Observer.t('a);