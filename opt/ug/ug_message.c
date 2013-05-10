#include "UG_LIB.h"

FILE * UG_Output_File = NULL;

INT_ UG_Standard_Output_Flag = 1;

void (*ext_ug_message) (FILE *UG_Output_File, INT_ UG_Standard_Output_Flag, char *text) = NULL;

void ug_register_message (void (*ext_ug_message_routine) (FILE *, INT_, char *))
{
  /*
   Register an external routine for handling messages that are normally sent
   to standard output.

   Call routine ug_register_message just after execution is started to capture
   all standard messages.

   void (*ext_ug_message) (FILE *UG_Output_File, INT_ UG_Standard_Output_Flag, char *text) = NULL;

   FILE *UG_Output_File is the file stream that can be used to capture all
   messages. If it is not NULL then the file is open for writing and the user
   has selected the option to capture messages in a file.

   INT_ UG_Standard_Output_Flag is a flag that if set to 1 implies that standard
   output messages should be sent to standard output.

   char *text is the text message.

  */

  ext_ug_message = ext_ug_message_routine;

  return;
}

void ug_message
 (char *text)

/*
 * Write a message to standard output.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_message.c,v 1.12 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

{

  if (ext_ug_message)
    (*ext_ug_message) (UG_Output_File, UG_Standard_Output_Flag, text);

  else
  {
    if (UG_Standard_Output_Flag == 1)
    {
      printf ("%s\n", text);

      fflush (stdout);
    }

    if (UG_Output_File)
    {
      fprintf (UG_Output_File, "%s\n", text);

      fflush (UG_Output_File);
    }
  }

  return;
}
