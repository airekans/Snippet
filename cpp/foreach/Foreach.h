#ifndef _FOREACH_H_
#define _FOREACH_H_

#include <boost/typeof/typeof.hpp>

/// Utitilies to make a unique variable name.
#define CONCAT_LINE(x) CONCAT(x, __LINE__)
#define CONCAT(a, b) CONCAT_INDIRECT(a, b)
#define CONCAT_INDIRECT(a, b) a ## b


/// macro used to loop over the list/vector
#define LIST_FOREACH(e, l)						\
  for (int CONCAT_LINE(__f) = 1, CONCAT_LINE(__breakFlag) = 1;		\
       CONCAT_LINE(__f); CONCAT_LINE(__f) = 0)				\
    for (BOOST_AUTO(CONCAT_LINE(__i), (l).begin());			\
	 CONCAT_LINE(__breakFlag) && CONCAT_LINE(__i) != (l).end();	\
	 ++CONCAT_LINE(__i), CONCAT_LINE(__f) = 1)			\
      for (BOOST_AUTO(& e, *CONCAT_LINE(__i));				\
	   CONCAT_LINE(__f) && (--CONCAT_LINE(__breakFlag) < 1);	\
	   CONCAT_LINE(__f) = 0, CONCAT_LINE(__breakFlag) = 1)


/// macro used to loop over the map
/// Usage: map_foreach(k, v, m) // m is a map
///		   { cout << "key " << k << " value " << v; }
/// Limitation: this should not be used twice on the same line
/// e.g. map_foreach(k1, v1, m1) map_foreach(k2, v2, m2) // (x)
/// You should separate this to 2 lines,
/// e.g. map_foreach(k1, v1, m1)
///          map_foreach(k2, v2, m2) ....
/// And this may have performance issue when looping over a big map.
#define MAP_FOREACH(key, value, m)					\
  for (int CONCAT_LINE(__f) = 1, CONCAT_LINE(__breakFlag) = 1;		\
       CONCAT_LINE(__f); CONCAT_LINE(__f) = 0)				\
    for (BOOST_AUTO(CONCAT_LINE(__i), (m).begin());			\
	 CONCAT_LINE(__breakFlag) && CONCAT_LINE(__i) != (m).end();	\
	 ++CONCAT_LINE(__i), CONCAT_LINE(__f) = 1)			\
      for (BOOST_AUTO(const & key, CONCAT_LINE(__i)->first);			\
	   CONCAT_LINE(__f); CONCAT_LINE(__f) = 0)			\
	for (BOOST_AUTO(& value, CONCAT_LINE(__i)->second);		\
	     CONCAT_LINE(__f) && (--CONCAT_LINE(__breakFlag) < 1);	\
	     CONCAT_LINE(__f) = 0, CONCAT_LINE(__breakFlag) = 1)



#endif

