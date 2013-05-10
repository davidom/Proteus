#include "UG_IO_LIB.h"

void ug_io_error_message
 (char Error_Message[],
  INT_ Error_Flag,
  INT_ *Known_Error)

{

/*
 * Write a UG_IO error message.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_error_message.c,v 1.9 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Text;

  if (*Known_Error == 1)
    return;

  if (Error_Flag >= 600 && Error_Flag <= 699)
  {
    ug_error_message ("*** UG_IO LIBRARY FATAL ERROR ***");
    sprintf (Text, "*** %s ***", Error_Message);
    ug_error_message (Text);

    *Known_Error = 1;
  }

  return;

}
