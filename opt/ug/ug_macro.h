#ifndef __UG_MACRO_H__

#define __UG_MACRO_H__

// UG LIBRARY : MACRO FUNCTIONS

// The UG include file, UG_LIB.h, includes UG library macro function definitions
// (from file ug_macro.h). The following macro functions are defined.

// FUNCTION      DESCRIPTION
// --------      -----------
// MAX (x,y)     Return value is maximum value of x and y.
// MIN (x,y)     Return value is Determine minimum value of x and y.
// NINT (x)      Return value is nearest integer to x. The return value is type
//               INT_ and the argument x must be type float or double.

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef NINT
#define NINT(x) ((INT_) (((x) >= 0.0) ? floor((x)+0.5) : -floor(0.5-(x))))
#endif

#endif
