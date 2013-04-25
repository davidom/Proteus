#include "UG_LIB.h"

INT_ ug_copy_file
 (char old[],
  char new[])

{

/*
 * Copy file.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_copy_file.c,v 1.9 2013/03/14 03:06:46 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX command;

  INT_ err;

  err = ug_check_file (old, 0);

  if (err < 0)
    return (-1);

  strcpy (command, UG_SHELL_COMMAND_CP); 
  strcat (command, " ");
  strcat (command, old);
  strcat (command, " ");
  strcat (command, new);

  err = ug_system (command);

  err = ug_check_file (new, 1);

  if (err < 0)
    return (-2);

  return (0);

}
