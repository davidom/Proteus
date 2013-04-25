#include "UG_IO_LIB.h"

INT_ ug_io_find_file
 (char Case_Name[],
  char Error_Message[],
  char File_Name[],
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr,
  INT_ File_Data_Type_)

{

/*
 * Find file name of an existing (and readable) standard UG file with given case
 * name and data type.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_find_file.c,v 1.22 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Data_File;

  CHAR_UG_MAX drive, dname, fname, ext;
  CHAR_31 Name_Text;
  CHAR_21 *File_Type_Suffix;
  CHAR_11 *File_Compression_Suffix, *File_Format_Suffix;

  INT_1D *File_Data_Type;

  CHAR_UG_MAX Test_Name;

  INT_ Check, File_Compression_Index, File_Data_Type_2_, File_Data_Type_Check,
       File_Format_Index, File_Format_Order, File_Found_Flag, File_Type_Index,
       Number_of_Checks, Number_of_File_Types, Number_of_File_Formats,
       Number_of_File_Compressions, Test_Name_Length;

  Test_Name_Length = (INT_) strlen (Case_Name) - 1;
  
  strcpy (Test_Name, "");
  strncat (Test_Name, Case_Name, Test_Name_Length);
  strcat (Test_Name, ".");

  if (strcmp (Test_Name, Case_Name) == 0)
  {
    strcpy (Test_Name, "");
    strncat (Test_Name, Case_Name, Test_Name_Length);
    strcpy (Case_Name, Test_Name);
  }

  Number_of_File_Types = UG_IO_Param_Struct_Ptr->Number_of_File_Types;
  File_Data_Type = UG_IO_Param_Struct_Ptr->File_Data_Type;
  File_Type_Suffix = UG_IO_Param_Struct_Ptr->File_Type_Suffix;

  Number_of_File_Formats = UG_IO_Param_Struct_Ptr->Number_of_File_Formats;
  File_Format_Suffix = UG_IO_Param_Struct_Ptr->File_Format_Suffix;

  Number_of_File_Compressions = UG_IO_Param_Struct_Ptr->Number_of_File_Compressions;
  File_Compression_Suffix = UG_IO_Param_Struct_Ptr->File_Compression_Suffix;

  if (File_Data_Type_ == UG_IO_SURFACE_GRID ||
      File_Data_Type_ == UG_IO_2D_EDGE_GRID)
  {
    File_Data_Type_2_ = File_Data_Type_ + 1;

    File_Format_Order = 1;

    Number_of_Checks = 2;
  }
  else
  {
    File_Format_Order = -1;

    Number_of_Checks = 1;
  }

  File_Found_Flag = 0;

  Check = 1;

  do
  {
    File_Data_Type_Check = (Check == 1) ? File_Data_Type_ : File_Data_Type_2_;

    File_Type_Index = 0;

    do
    {
      if (File_Data_Type_Check == File_Data_Type[File_Type_Index])
      {
        if (File_Format_Order == 1)
          File_Format_Index = 0;
        else
          File_Format_Index = Number_of_File_Formats-1;

        do
        {
          File_Compression_Index = 0;

          do
          {
            strcpy (File_Name, "");
            strcat (File_Name, Case_Name);
            strcat (File_Name, File_Format_Suffix[File_Format_Index]);
            strcat (File_Name, File_Type_Suffix[File_Type_Index]);
            strcat (File_Name, File_Compression_Suffix[File_Compression_Index]);

            Data_File = ug_fopen (File_Name, "r");

            if (Data_File != NULL)
            {
              File_Found_Flag = 1;

              ug_fclose (Data_File);
            }

            ++File_Compression_Index;
          }
          while (File_Compression_Index < Number_of_File_Compressions &&
                 File_Found_Flag == 0);

          if (File_Format_Order == 1)
            ++File_Format_Index;
          else
            --File_Format_Index;
        }
        while (File_Format_Index >= 0 &&
               File_Format_Index < Number_of_File_Formats &&
               File_Found_Flag == 0);
      }

      ++File_Type_Index;
    }
    while (File_Type_Index < Number_of_File_Types && File_Found_Flag == 0);

    ++Check;
  }
  while (Check <= Number_of_Checks && File_Found_Flag == 0);

  if (File_Found_Flag == 0)
  {
    strcpy (File_Name, "");

    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Error_Message, "no files found that match case name %s",
             Name_Text);
    return (-601);
  }

  return (0);

}
