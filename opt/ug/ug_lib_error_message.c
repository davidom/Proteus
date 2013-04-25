#include "UG_LIB.h"

void ug_lib_error_message
 (char Error_Message[],
  INT_ Error_Flag,
  INT_ *Known_Error)

{

/*
 * Write a UG error message.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_lib_error_message.c,v 1.27 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum 
 */

  CHAR_133 Text;

  if (*Known_Error == 1)
    return;

  if (Error_Flag >= 400 && Error_Flag <= 499)
  {
    sprintf (Text, "*** UG LIBRARY FATAL ERROR (%i) ***", (int) Error_Flag);
    ug_error_message (Text);

    if (Error_Flag == 400 || Error_Flag == 401)
      ug_error_message ("*** error reading data from tmp file ***");

    if (Error_Flag == 402)
      ug_error_message ("*** error writing parallel/distributed mode status data to tmp file ***");

    else if (Error_Flag == 403)
      ug_error_message ("*** invalid processor number ***");

    else if (Error_Flag == 404)
      ug_error_message ("*** invalid number of processors ***");

    else if (Error_Flag == 405)
      ug_error_message ("*** fatal error in another process ***");

    else if (Error_Flag == 406 || Error_Flag == 407)
      ug_error_message ("*** error writing data to tmp file ***");

    else if (Error_Flag == 408)
      ug_error_message ("*** unable to find or create tmp directory ***");

    else if (Error_Flag == 409 || Error_Flag == 410)
      ug_error_message ("*** error processing input parameters ***");

    else if (Error_Flag == 411 || Error_Flag == 412)
      ug_error_message ("*** error in input parameters ***");

    else if (Error_Flag == 413)
      ug_error_message ("*** error opening input parameter file ***");

    else
    {
      sprintf (Text, "*** %s ***", Error_Message);
      ug_error_message (Text);
    }

    *Known_Error = 1;
  }

  if (Error_Flag >= 100400 && Error_Flag <= 100499)
  {
    sprintf (Text, "*** UG LIBRARY FATAL MEMORY ERROR (%i) ***", (int) Error_Flag);
    ug_error_message (Text);
    ug_error_message ("*** unable to allocate required memory ***");

    *Known_Error = 1;
  }

  if (*Known_Error == 0)
  {
    sprintf (Text, "*** UNKNOWN FATAL ERROR (%i) ***", (int) Error_Flag);
    ug_error_message (Text);
  }

  return;

}
