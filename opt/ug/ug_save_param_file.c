#include "UG_LIB.h"

INT_ ug_save_param_file
 (char Case_Name[],
  char Suffix_Name[],
  char UG_Param_File_Name[],
  char Error_Message[])

{

/*
 * Save UG parameter file.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_save_param_file.c,v 1.16 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX File_Name, drive, dname, fname, ext;
  CHAR_31 Name_Text;

  INT_ Error_Flag = 0;

  sprintf (File_Name, "%s.%s.par", Case_Name, Suffix_Name);

  if (strcmp (File_Name, UG_Param_File_Name) != 0)
  {
    Error_Flag = ug_check_file (UG_Param_File_Name, 0);

    if (Error_Flag != 0)
    {
      ug_splitpath (UG_Param_File_Name, drive, dname, fname, ext);

      strcat (fname, ext);
      strcpy (Name_Text, "");
      strncat (Name_Text, fname, 29);

      sprintf (Error_Message, "unable to get status of existing parameter file %s",
               Name_Text);
      return (455);
    }

    Error_Flag = ug_backup_file (File_Name);

    Error_Flag = ug_copy_file (UG_Param_File_Name, File_Name);

    if (Error_Flag != 0)
    {
      ug_splitpath (UG_Param_File_Name, drive, dname, fname, ext);

      strcat (fname, ext);
      strcpy (Name_Text, "");
      strncat (Name_Text, fname, 29);

      sprintf (Error_Message, "unable to copy parameter file from %s to ",
               Name_Text);

      ug_splitpath (File_Name, drive, dname, fname, ext);

      strcat (fname, ext);
      strncat (Error_Message, fname, 29);

      return (456);
    }

    Error_Flag = ug_remove_file (UG_Param_File_Name);
  }

  return (0);

}
