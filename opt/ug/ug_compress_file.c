#include "UG_LIB.h"

INT_ ug_compress_file
 (INT_ bg,
  char ext[],
  char path[])

{

/*
 * Compress file.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_compress_file.c,v 1.15 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX command;

  INT_ err = 0;

  strcpy (command, "");

#if defined (UG_SHELL_COMMAND_BZIP)
  if (strstr (ext, UG_SHELL_COMMAND_BZIP_EXT)) strcpy (command, UG_SHELL_COMMAND_BZIP);
#endif 
#if defined (UG_SHELL_COMMAND_GZIP)
  if (strstr (ext, UG_SHELL_COMMAND_GZIP_EXT)) strcpy (command, UG_SHELL_COMMAND_GZIP);
#endif 
#if defined (UG_SHELL_COMMAND_COMPRESS)
  if (strstr (ext, UG_SHELL_COMMAND_COMPRESS_EXT)) strcpy (command, UG_SHELL_COMMAND_COMPRESS);
#endif 

  if (strcmp (command, ""))
  {
    strcat (command, " ");
    strcat (command, path);

    if (bg)
      strcat (command, UG_SHELL_COMMAND_BG);

    err = ug_system (command);
  }

  return (err);

}

INT_ ug_uncompress_file
 (char path[])

{

/*
 * Uncompress file.
 */

  CHAR_UG_MAX command;

  INT_ err = 0;

  strcpy (command, "");
#if defined (UG_SHELL_COMMAND_BUNZIP)
  if (strstr (path, UG_SHELL_COMMAND_BZIP_EXT)) strcpy (command, UG_SHELL_COMMAND_BUNZIP);
#endif 
#if defined (UG_SHELL_COMMAND_GUNZIP)
  if (strstr (path, UG_SHELL_COMMAND_GZIP_EXT)) strcpy (command, UG_SHELL_COMMAND_GUNZIP);
#endif 
#if defined (UG_SHELL_COMMAND_UNCOMPRESS)
  if (strstr (path, UG_SHELL_COMMAND_COMPRESS_EXT)) strcpy (command, UG_SHELL_COMMAND_UNCOMPRESS);
#endif 

  if (strcmp (command, ""))
  {
    strcat (command, " ");
    strcat (command, path);

    err = ug_system (command);
  }

  return (err);

}
