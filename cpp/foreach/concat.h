#ifndef _CONCAT_H_
#define _CONCAT_H_

/// Utitilies to make a unique variable name.
#define CONCAT_LINE(x) CONCAT(x ## _foreach__, __LINE__)
#define CONCAT(a, b) CONCAT_INDIRECT(a, b)
#define CONCAT_INDIRECT(a, b) a ## b

#endif
