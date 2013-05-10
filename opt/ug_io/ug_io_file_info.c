#include "UG_IO_LIB.h"

INT_ ug_io_file_info
 (char File_Name[],
  char Case_Name[],
  char Error_Message[],
  char File_Compression_Suffix_[],
  char File_Format_Suffix_[],
  char File_Mode_Suffix_[],
  char File_Type_Suffix_[],
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr,
  INT_ Check_File_Flag,
  INT_ *File_Data_Type_,
  INT_ *File_Format_,
  INT_ *File_Mode_,
  INT_ *File_Type_Format_Flag_)

{

/*
 * Determine file compression suffix, format suffix, type suffix, data type and
 * extract the case name (with leading path if any) from a standard UG file
 * name.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_file_info.c,v 1.38 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Data_File;

  CHAR_UG_MAX drive, dname, fname, ext;
  CHAR_41 *File_Mode_Label, *File_Type_Label;
  CHAR_31 Name_Text;
  CHAR_21 *File_Type_Suffix;
  CHAR_11 *File_Compression_Suffix, *File_Format_Suffix, *File_Mode_Suffix;

  INT_1D *File_Data_Type, *File_Format, *File_Mode, *File_Type_Format_Flag;

  CHAR_UG_MAX Test_Name;
  CHAR_UG_MAX Try_Name;

  INT_ ASCII_Format_Index, Case_Name_Length, File_Compression_Index,
       File_Format_Index, File_Found_Flag, File_Mode_Index, File_Name_Length,
       File_Type_Index, Format_Index, Format_Option, isFound, m,
       Max_IO_Param_Labels, Mode_Index, Mode_Option, n, No_Compression_Index,
       Number_of_File_Types, Number_of_File_Formats, Number_of_File_Modes,
       Number_of_File_Compressions, Standard_Mode_Index, Test_Name_Length,
       Try_Name_Length;

  if (File_Name == NULL || strcmp (File_Name, "") == 0)
  {
    strcpy (Error_Message, "blank or NULL file name");
    return (-600);
  }

  Test_Name_Length = (INT_) strlen (File_Name) - 1;
  
  strcpy (Test_Name, "");
  strncat (Test_Name, File_Name, Test_Name_Length);
  strcat (Test_Name, ".");

  if (strcmp (Test_Name, File_Name) == 0)
  {
    strcpy (Test_Name, "");
    strncat (Test_Name, File_Name, Test_Name_Length);
    strcpy (File_Name, Test_Name);
  }

  Max_IO_Param_Labels = UG_IO_Param_Struct_Ptr->Max_IO_Param_Labels;
  Number_of_File_Types = UG_IO_Param_Struct_Ptr->Number_of_File_Types;
  File_Data_Type = UG_IO_Param_Struct_Ptr->File_Data_Type;
  File_Type_Format_Flag = UG_IO_Param_Struct_Ptr->File_Type_Format_Flag;
  File_Type_Label = UG_IO_Param_Struct_Ptr->File_Type_Label;
  File_Type_Suffix = UG_IO_Param_Struct_Ptr->File_Type_Suffix;

  Number_of_File_Formats = UG_IO_Param_Struct_Ptr->Number_of_File_Formats;
  File_Format = UG_IO_Param_Struct_Ptr->File_Format;
  File_Format_Suffix = UG_IO_Param_Struct_Ptr->File_Format_Suffix;

  Number_of_File_Modes = UG_IO_Param_Struct_Ptr->Number_of_File_Modes;
  File_Mode = UG_IO_Param_Struct_Ptr->File_Mode;
  File_Mode_Label = UG_IO_Param_Struct_Ptr->File_Mode_Label;
  File_Mode_Suffix = UG_IO_Param_Struct_Ptr->File_Mode_Suffix;

  Number_of_File_Compressions = UG_IO_Param_Struct_Ptr->Number_of_File_Compressions;
  File_Compression_Suffix = UG_IO_Param_Struct_Ptr->File_Compression_Suffix;

  *File_Data_Type_ = 0;

  File_Type_Index = 0;

  File_Name_Length = (INT_) strlen (File_Name);
  do
  {
    strcpy (File_Type_Suffix_, File_Type_Suffix[File_Type_Index]);

    if (strstr (File_Name, File_Type_Suffix_) != NULL)
    {
       strcpy (Try_Name, "");
       strcat (Try_Name, File_Type_Suffix_);
       strcat (Try_Name, ".");

       isFound = 1;

       if (strstr (File_Name, Try_Name) == NULL)
       {
          Try_Name_Length = (INT_) strlen (File_Type_Suffix_);

          m = File_Name_Length-Try_Name_Length;
          n = 0;

          do
          {
             if (File_Type_Suffix_[n] != File_Name[m])
               isFound = 0;

            ++m;
            ++n;
          }
          while (n < Try_Name_Length && isFound == 1);
       }

       if (isFound)
       {
          *File_Data_Type_ = File_Data_Type[File_Type_Index];
          *File_Type_Format_Flag_ = File_Type_Format_Flag[File_Type_Index];
       }
    }

    ++File_Type_Index;
  }
  while (File_Type_Index < Number_of_File_Types && *File_Data_Type_ == 0);

  --File_Type_Index;

  if (*File_Data_Type_ == 0)
  {
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Error_Message, "file %s has an unknown file type", Name_Text);
    return (600);
  }

  ASCII_Format_Index = -1;

  File_Format_Index = 0;

  do
  {
    if (strcmp (File_Format_Suffix[File_Format_Index], "") == 0)
      ASCII_Format_Index = File_Format_Index;

    ++File_Format_Index;
  }
  while (File_Format_Index < Number_of_File_Formats && ASCII_Format_Index== -1);

  strcpy (File_Format_Suffix_, "");

  Format_Index = ASCII_Format_Index;

  *File_Format_ = File_Format[ASCII_Format_Index];

  File_Format_Index = 0;

  do
  {
    if (File_Format_Index != ASCII_Format_Index &&
        strstr (File_Name, File_Format_Suffix[File_Format_Index]) != NULL)
    {
       strcpy (Try_Name, "");
       strcat (Try_Name, File_Format_Suffix[File_Format_Index]);
       strcat (Try_Name, ".");

       isFound = 1;

       if (strstr (File_Name, Try_Name) == NULL)
       {
          Try_Name_Length = (INT_) strlen (File_Format_Suffix[File_Format_Index]);

          m = File_Name_Length-Try_Name_Length;
          n = 0;

          do
          {
             if (File_Format_Suffix[File_Format_Index][n] != File_Name[m])
               isFound = 0;

            ++m;
            ++n;
          }
          while (n < Try_Name_Length && isFound == 1);
       }

       if (isFound)
       {
          strcpy (File_Format_Suffix_, File_Format_Suffix[File_Format_Index]);
          Format_Index = File_Format_Index;
          *File_Format_ = File_Format[File_Format_Index];
       }
    }

    ++File_Format_Index;
  }
  while (File_Format_Index < Number_of_File_Formats &&
         strcmp (File_Format_Suffix_, "") == 0);

  File_Format_Index = Format_Index;

  Standard_Mode_Index = -1;

  File_Mode_Index = 0;

  do
  {
    if (strcmp (File_Mode_Suffix[File_Mode_Index], "") == 0)
      Standard_Mode_Index = File_Mode_Index;

    ++File_Mode_Index;
  }
  while (File_Mode_Index < Number_of_File_Modes && Standard_Mode_Index== -1);

  strcpy (File_Mode_Suffix_, "");

  Mode_Index = Standard_Mode_Index;

  *File_Mode_ = File_Mode[Standard_Mode_Index];

  File_Mode_Index = 0;

  do
  {
    if (File_Mode_Index != Standard_Mode_Index &&
        strstr (File_Name, File_Mode_Suffix[File_Mode_Index]) != NULL)
    {
       strcpy (Try_Name, "");
       strcat (Try_Name, File_Mode_Suffix[File_Mode_Index]);
       strcat (Try_Name, ".");

       isFound = 1;

       if (strstr (File_Name, Try_Name) == NULL)
       {
          Try_Name_Length = (INT_) strlen (File_Mode_Suffix[File_Mode_Index]);

          m = File_Name_Length-Try_Name_Length;
          n = 0;

          do
          {
             if (File_Mode_Suffix[File_Mode_Index][n] != File_Name[m])
               isFound = 0;

            ++m;
            ++n;
          }
          while (n < Try_Name_Length && isFound == 1);
       }

       if (isFound)
       {
          strcpy (File_Mode_Suffix_, File_Mode_Suffix[File_Mode_Index]);
          Mode_Index = File_Mode_Index;
          *File_Mode_ = File_Mode[File_Mode_Index];
       }
    }

    ++File_Mode_Index;
  }
  while (File_Mode_Index < Number_of_File_Modes &&
         strcmp (File_Mode_Suffix_, "") == 0);

  File_Mode_Index = Mode_Index;

  No_Compression_Index = -1;

  File_Compression_Index = 0;

  do
  {
    if (strcmp (File_Compression_Suffix[File_Compression_Index], "") == 0)
      No_Compression_Index = File_Compression_Index;

    ++File_Compression_Index;
  }
  while (File_Compression_Index < Number_of_File_Compressions &&
         No_Compression_Index == -1);

  strcpy (File_Compression_Suffix_, "");

  File_Compression_Index = 0;

  do
  {
    if (File_Compression_Index != No_Compression_Index &&
        strstr (File_Name,
                File_Compression_Suffix[File_Compression_Index]) != NULL)
    {
       strcpy (Try_Name, "");
       strcat (Try_Name, File_Compression_Suffix[File_Compression_Index]);
       strcat (Try_Name, ".");

       isFound = 1;

       if (strstr (File_Name, Try_Name) == NULL)
       {
          Try_Name_Length = (INT_) strlen (File_Compression_Suffix[File_Compression_Index]);

          m = File_Name_Length-Try_Name_Length;
          n = 0;

          do
          {
             if (File_Compression_Suffix[File_Compression_Index][n] != File_Name[m])
               isFound = 0;

            ++m;
            ++n;
          }
          while (n < Try_Name_Length && isFound == 1);
       }

       if (isFound)
       {
          strcpy (File_Compression_Suffix_,
                  File_Compression_Suffix[File_Compression_Index]);
       }
    }

    ++File_Compression_Index;
  }
  while (File_Compression_Index < Number_of_File_Compressions &&
         strcmp (File_Compression_Suffix_, "") == 0);

  --File_Compression_Index;

  Case_Name_Length = (INT_) strlen (File_Name)
                   - (INT_) strlen (File_Mode_Suffix_)
                   - (INT_) strlen (File_Format_Suffix_)
                   - (INT_) strlen (File_Type_Suffix_)
                   - (INT_) strlen (File_Compression_Suffix_);

  strcpy (Case_Name, "");
  strncat (Case_Name, File_Name, Case_Name_Length);

  strcpy (Test_Name, "");
  strcat (Test_Name, Case_Name);
  strcat (Test_Name, File_Mode_Suffix_);
  strcat (Test_Name, File_Format_Suffix_);
  strcat (Test_Name, File_Type_Suffix_);
  strcat (Test_Name, File_Compression_Suffix_);

  if (strcmp (Test_Name, File_Name) != 0)
  {
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Error_Message,
             "file %s has an unknown file type, format, and/or mode",
             Name_Text);
    return (600);
  }

  Format_Option = *File_Type_Format_Flag_ / 8;
  Mode_Option = (*File_Type_Format_Flag_ - Format_Option * 8) / 4;

  if (Format_Option == 0 && *File_Format_ != UG_IO_FORMATTED)
  {
    sprintf (Error_Message, "%s files must use a fixed file format",
             File_Type_Label[File_Type_Index*Max_IO_Param_Labels]);
             
    return (600);
  }

  if (Mode_Option == 0 && *File_Mode_ != UG_FIO_STANDARD_FILE_MODE)
  {
    sprintf (Error_Message, "%s must use standard mode",
             File_Type_Label[File_Type_Index*Max_IO_Param_Labels]);
             
    return (600);
  }

  if (Mode_Option == 1 && *File_Mode_ != UG_FIO_STANDARD_FILE_MODE &&
      *File_Format_ != UG_IO_BINARY_DOUBLE)
  {
    sprintf (Error_Message, "%s files must use C binary double file format",
             File_Mode_Label[File_Mode_Index*Max_IO_Param_Labels]);
             
    return (600);
  }

  if (Check_File_Flag == 1 && *File_Mode_ == UG_FIO_STANDARD_FILE_MODE)
  {
    File_Found_Flag = 0;

    Data_File = ug_fopen (File_Name, "r");

    if (Data_File != NULL)
    {
      ug_fclose (Data_File);

      File_Found_Flag = 1;
    }

    if (File_Found_Flag == 0 && strcmp (File_Compression_Suffix_, "") == 0)
    {
      File_Compression_Index = 0;

      do
      {
        if (File_Compression_Index != No_Compression_Index)
        {
          strcpy (Test_Name, "");
          strcat (Test_Name, File_Name);

          strcat (Test_Name, File_Compression_Suffix[File_Compression_Index]);

          Data_File = ug_fopen (Test_Name, "r");

          if (Data_File != NULL)
          {
            ug_fclose (Data_File);

            File_Found_Flag = 1;

            strcpy (File_Name, "");
            strcat (File_Name, Test_Name);

            strcpy (File_Compression_Suffix_,
                    File_Compression_Suffix[File_Compression_Index]);
          }
        }

        ++File_Compression_Index;
      }
      while (File_Compression_Index < Number_of_File_Compressions &&
             File_Found_Flag == 0);
    }

    if (File_Found_Flag == 0)
    {
      ug_splitpath (File_Name, drive, dname, fname, ext);

      strcat (fname, ext);
      strcpy (Name_Text, "");
      strncat (Name_Text, fname, 29);

      sprintf (Error_Message, "unable to open input file %s", Name_Text);

      return (600);
    }
  }

  ug_set_byte_order (*File_Format_);

  return (0);

}

