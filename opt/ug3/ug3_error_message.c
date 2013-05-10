#include "UG3_LIB.h"

void ug3_error_message
 (INT_ Error_Flag,
  INT_ *Known_Error)

{

/*
 * Write UG3 error message.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_error_message.c,v 1.26 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  if (*Known_Error == 1)
    return;

  if (Error_Flag >= 300 && Error_Flag <= 399)
  {
    sprintf (Text, "*** UG (3D) LIBRARY FATAL ERROR (%i) ***", Error_Flag);
    ug_error_message (Text);

    if (Error_Flag == 300)
      ug_error_message ("*** unable to find a boundary element for each boundary face ***");

    if (Error_Flag == 304)
      ug_error_message ("*** boundary face connectivity is wrong ***");

    else if (Error_Flag == 305 || Error_Flag == 333)
    {
      if (Error_Flag == 333)
        ug_error_message ("*** boundary surface for external object is not fully connected ***");
      ug_error_message ("*** boundary faces found without neighbor(s) ***");
      ug_error_message ("*** there may be extra boundary faces unconnected ***");
      ug_error_message ("*** or partially connected to surface ***");
      ug_error_message ("*** there may be a hole in the boundary surface ***");
    }

    else if (Error_Flag == 307)
      ug_error_message ("*** unable to find an external boundary ***");

    else if (Error_Flag == 312)
      ug_error_message ("*** unable to check boundary object face ordering ***");

    else if (Error_Flag == 316 || (Error_Flag == 325))
      ug_error_message ("*** found element with negative volume ***");

    else if (Error_Flag == 337)
      ug_error_message ("*** error reading SURF surface grid file ***");

    else if (Error_Flag == 338)
      ug_error_message ("*** error reading UGRID grid file ***");

    else if (Error_Flag == 339)
      ug_error_message ("*** error writing SURF surface grid file ***");

    else if (Error_Flag == 340)
      ug_error_message ("*** error writing UGRID grid file ***");

    else if (Error_Flag == 341)
      ug_error_message ("*** error reading UFUNC function file ***");

    else if (Error_Flag == 342)
      ug_error_message ("*** error writing UFUNC function file ***");

    else if (Error_Flag == 343)
      ug_error_message ("*** error writing BC SURF surface grid file ***");

    else if (Error_Flag == 345)
      ug_error_message ("*** external boundary object has transparent/embedded faces ***");

    else if (Error_Flag == 346)
      ug_error_message ("*** error writing VNODE node data file ***");

    else if (Error_Flag == 347)
      ug_error_message ("*** unable to extracting faces from grid with no elements ***");

    else if (Error_Flag == 348)
      ug_error_message ("*** number of faces extracted from volume grid do not match ***");

    else if (Error_Flag == 349)
      ug_error_message ("*** unable to repair mismatched faces extracted from volume grid ***");

    *Known_Error = 1;
  }

  if (Error_Flag >= 100300 && Error_Flag <= 100399)
  {
    sprintf (Text, "*** UG (3D) LIBRARY FATAL MEMORY ERROR (%i) ***",
             Error_Flag);
    ug_error_message (Text);
    ug_error_message ("*** unable to allocate required memory ***");

    *Known_Error = 1;
  }

  if (Error_Flag >= 200300 && Error_Flag <= 200399)
  {
    sprintf (Text, "*** UG (3D) LIBRARY FATAL SURFACE GRID ERROR (%i) ***",
             Error_Flag);
    ug_error_message (Text);

    *Known_Error = 1;
  }

  return;

}
