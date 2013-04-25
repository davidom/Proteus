#include "UG_LIB.h"

INT_ ug_backup_file
 (char File_Name[])

{

/*
 * Backup file.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_backup_file.c,v 1.5 2013/03/13 04:05:32 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Backup_File_Name;

  INT_ Error_Flag = 0;

  sprintf (Backup_File_Name, "%s.bak", File_Name);

  Error_Flag = ug_copy_file (File_Name, Backup_File_Name);

  return (Error_Flag);

}
