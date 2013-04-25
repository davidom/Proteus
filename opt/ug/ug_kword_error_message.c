#include "UG_LIB.h"

void ug_kword_error_message
 (INT_ Error_Flag,
  INT_ *Known_Error,
  char Program[])

/*
 * Write a KWORD error message to standard error output.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_kword_error_message.c,v 1.8 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

{
  CHAR_133 Text;

  if (*Known_Error == 1)
    return;

  if (Error_Flag >= 130000 && Error_Flag <= 130099)
  {
    *Known_Error = 1;

    sprintf (Text, "*** KWORD LIBRARY FATAL ERROR (%i) ***", Error_Flag);
    ug_error_message (Text);

    if (strcmp (Program, "") != 0)
    {
      sprintf (Text, "*** ERROR IN LICENSE KEY FOR %s ***", Program);
      ug_error_message (Text);
    }

    if (Error_Flag == 130001)
      ug_error_message ("*** UNABLE TO GET HOME ENVIRONMENT VARIABLE ***");

    if (Error_Flag == 130002)
      ug_error_message ("*** UNABLE TO OPEN MASTER KEY FILE ***");

    if (Error_Flag == 130003)
      ug_error_message ("*** UNABLE TO GET SIMCENTER_SYS_DIR ENVIRONMENT VARIABLE ***");

    if (Error_Flag == 130004)
      ug_error_message ("*** UNABLE TO OPEN LICENSE KEY FILE ***");

    if (Error_Flag == 130005)
      ug_error_message ("*** UNABLE TO READ LICENSE KEY FILE ***");

    if (Error_Flag == 130006)
      ug_error_message ("*** UNABLE TO FIND LICENSE KEY IN LICENSE KEY FILE ***");

    if (Error_Flag == 130007)
      ug_error_message ("*** LICENSE KEY IS NOT VALID ***");

    if (Error_Flag == 130008)
      ug_error_message ("*** LICENSE KEY HAS EXPIRED ***");

    if (Error_Flag >= 130009)
      ug_error_message ("*** LICENSE KEY CODE ERROR ***");
  }

  return;

}
