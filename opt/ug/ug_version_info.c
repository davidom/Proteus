#include "UG_LIB.h"

void ug_version_info (char Lib_Name[],
                      void (*Version_Function) (char[],
                                                char[],
                                                char[],
                                                char[]))

/*
 * Write library version information.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_version_info.c,v 1.15 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

{

  static INT_ Build_Number_1 = 0;
  static INT_ Build_Number_2 = 0;
  static INT_ Build_Number_3 = 0;

  char *Lib_Name_;

  CHAR_81 Compile_Date, Compile_OS, Text, Version_Date, Version_Number;

  INT_ Message_Flag, Task_Flag, v1, v2, v3;

  if (strncmp (Lib_Name, "*", 1))
  {
    Message_Flag = 1;

    Lib_Name_ = Lib_Name;
  }
  else
  {
    Message_Flag = 0;

    Lib_Name_ = &(Lib_Name[1]);
  }

  if (strcmp (Lib_Name_, "Build_Number"))
    Task_Flag = 1;
  else
    Task_Flag = 0;

  if (Task_Flag == 1)
  {
    (*Version_Function) (Compile_Date, Compile_OS, Version_Date, Version_Number);

    if (Message_Flag == 1)
    {
      ug_message ("");

      sprintf (Text, "Lib Info     : Library Name   %s", Lib_Name_);
      ug_message (Text);

      sprintf (Text, "Lib Info     : Version Number %-50s", Version_Number);
      ug_message (Text);

      sprintf (Text, "Lib Info     : Version Date   %-50s", Version_Date);
      ug_message (Text);

      sprintf (Text, "Lib Info     : Compile OS     %-50s", Compile_OS);
      ug_message (Text);

      sprintf (Text, "Lib Info     : Compile Date   %-50s", Compile_Date);
      ug_message (Text);
    }

    sscanf (Version_Number, "%i.%i.%i", (int *) &v1, (int *) &v2, (int *) &v3);

    Build_Number_1 = Build_Number_1 + v1; 
    Build_Number_2 = Build_Number_2 + v2; 
    Build_Number_3 = Build_Number_3 + v3; 
  }
  else
  {
    if (Message_Flag == 1)
    {
      ug_message ("");

      sprintf (Text, "Executable   : Build Number   %i.%i.%i", (int) Build_Number_1, (int) Build_Number_2, (int) Build_Number_3);
      ug_message (Text);
    }
    else
      sprintf (Lib_Name, "Build_Number %i.%i.%i", (int) Build_Number_1, (int) Build_Number_2, (int) Build_Number_3);

    Build_Number_1 = 0;
    Build_Number_2 = 0;
    Build_Number_3 = 0;
  }

  return;

}
