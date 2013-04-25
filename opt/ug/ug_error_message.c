#include "UG_LIB.h"

void (*ext_ug_error_message) (FILE *UG_Output_File, char *text) = NULL;

void ug_register_error_message (void (*ext_ug_error_message_routine) (FILE *, char *))
{
  /*
   Register an external routine for handling error messages that are normally
   sent to standard error output. Note that these messages are sent just prior
   to program termination.

   Call routine ug_register_error_message just after execution is started to
   capture all error messages.

   void (*ext_ug_error_message) (FILE *UG_Output_File, char *text) = NULL;

   FILE *UG_Output_File is the file stream that can be used to capture all
   messages. If it is not NULL then the file is open for writing and the user
   has selected the option to capture messages in a file.

   char *text is the text message.
  */

  ext_ug_error_message = ext_ug_error_message_routine;

  return;
}

void ug_error_message
 (char *text)

/*
 * Write a message to standard error output.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_error_message.c,v 1.10 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

{
  extern FILE * UG_Output_File;

  if (ext_ug_error_message)
    (*ext_ug_error_message) (UG_Output_File, text);

  else
  {
    fprintf (stderr, "%s\n", text);

    if (UG_Output_File)
      fprintf (UG_Output_File, "%s\n", text);
  }

  return;
}
