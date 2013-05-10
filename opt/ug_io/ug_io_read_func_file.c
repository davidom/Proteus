#include "UG_IO_LIB.h"

INT_ ug_io_read_func_file
 (char Func_File_Name[],
  char Error_Message[],
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr,
  INT_ Message_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_U_Scalars,
  INT_ *Number_of_U_Vectors,
  CHAR_21 * U_Scalar_Labels,
  CHAR_21 * U_Vector_Labels,
  INT_1D * U_Scalar_Flags,
  INT_1D * U_Vector_Flags,
  DOUBLE_1D * U_Scalars,
  DOUBLE_3D * U_Vectors)

{

/*
 * Read function data from a standard UG function file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_func_file.c,v 1.7 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Case_Name, File_Name, drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;
  CHAR_21 File_Type_Suffix_;
  CHAR_11 File_Compression_Suffix_, File_Format_Suffix_, File_Mode_Suffix_;

  FILE * Func_File;

  INT_ Close_Flag, Error_Flag, File_Data_Type_, File_Format_, File_Mode_,
       File_Name_Length, File_Type_Format_Flag_, Index;

  INT_ Check_File_Flag = 0;

  static INT_ Read_Task_Flag = 0;

  double dc0;

  dc0 = 0.0;

  ++Read_Task_Flag;

  if (Read_Task_Flag > 3)
    Read_Task_Flag = 1;

  if (Read_Task_Flag == 1)
  {
    *Number_of_U_Scalars = 0;
    *Number_of_U_Vectors = 0;
  }

  if (Read_Task_Flag == 2)
  {
    for (Index = 0; Index < *Number_of_U_Scalars; ++Index)
    {
      strcpy (U_Scalar_Labels[Index], "                    ");
    }

    for (Index = 0; Index < *Number_of_U_Vectors; ++Index)
    {
      strcpy (U_Vector_Labels[Index], "                    ");
    }
  }

  Error_Flag = ug_io_file_info (Func_File_Name, Case_Name, Error_Message,
                                File_Compression_Suffix_, File_Format_Suffix_,
                                File_Mode_Suffix_, File_Type_Suffix_,
                                UG_IO_Param_Struct_Ptr,
                                Check_File_Flag, &File_Data_Type_,
                                &File_Format_, &File_Mode_,
                                &File_Type_Format_Flag_);
  
  if (Error_Flag > 0)
  {
    Read_Task_Flag = 0;
    return (619);
  }

  if (File_Data_Type_ != UG_IO_FUNCTION_DATA)
  {
    strcpy (Error_Message, "file is not a 3D function file");
    return (-619);
  }

  File_Format_ = abs (File_Format_);

  if (Read_Task_Flag == 1 && Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG_IO    : INPUT FUNCTION FIELD");
    ug_message (" ");
    ug_message ("UG_IO    : Reading data");
    ug_message (" ");

    ug_cpu_message ("");
  }

  File_Name_Length = (INT_) strlen (Func_File_Name)
                   - (INT_) strlen (File_Compression_Suffix_);

  strcpy (File_Name, "");
  strncat (File_Name, Func_File_Name, File_Name_Length);

  if (Read_Task_Flag == 1)
  {
    Error_Flag = ug_uncompress_file (Func_File_Name);

    if (Error_Flag != 0)
    {
      Read_Task_Flag = 0;
      strcpy (Error_Message, "unable to uncompress function file");
      return (619);
    }
  }

  if (File_Mode_ == UG_FIO_TMP_FILE_MODE)
    Func_File = ug_fopen (File_Name, "r_tmp");

  else
    Func_File = ug_fopen (File_Name, "r");

  if (Func_File == NULL)
  {
    Read_Task_Flag = 0;
    strcpy (Error_Message, "unable to open function file");
    return (619);
  }

  if (File_Mode_ == UG_FIO_STANDARD_FILE_MODE)
    Close_Flag = 1;
  else
    Close_Flag = 0;

  if (strcmp (File_Type_Suffix_, ".ufunc") == 0)
    Error_Flag = ug_io_read_ufunc (Func_File, Error_Message, File_Format_,
                                   Read_Task_Flag, Number_of_Nodes,
                                   Number_of_U_Scalars, Number_of_U_Vectors,
                                   U_Scalar_Labels, U_Vector_Labels,
                                   U_Scalar_Flags, U_Vector_Flags,
                                   U_Scalars, U_Vectors);

  else
  { 
    Error_Flag = 1;
    strcpy (Error_Message, "unknown input function file type");
  }

  if ((Close_Flag == 1 && Read_Task_Flag == 3) || Error_Flag > 0)
    Close_Flag = ug_fclose (Func_File);

  if (Error_Flag != 0)
  {
    Read_Task_Flag = 0;
    return (619);
  }

  if (Read_Task_Flag == 1 && Message_Flag >= 1)
  {
    ug_splitpath (Func_File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "UG_IO    : Function File Name= %s", Name_Text);

    ug_message (Text);

    sprintf (Text, "UG_IO    : Scalar Variables  =%10i",
             *Number_of_U_Scalars);

    ug_message (Text);

    sprintf (Text, "UG_IO    : Vector Variables  =%10i",
             *Number_of_U_Vectors);

    ug_message (Text);
  }

  if (Read_Task_Flag == 3 && Message_Flag >= 1)
  {
    for (Index = 0; Index < *Number_of_U_Scalars; ++Index)
    {
      if (U_Scalar_Flags[Index] != 0)
      {
        sprintf (Text, "UG_IO    : Scalar Label %-4i = %s",
                 Index, U_Scalar_Labels[Index]);

        ug_message (Text);
      }
    }

    for (Index = 0; Index < *Number_of_U_Vectors; ++Index)
    {
      if (U_Vector_Flags[Index] != 0)
      {
        sprintf (Text, "UG_IO    : Vector Label %-4i = %s",
                 Index, U_Vector_Labels[Index]);

        ug_message (Text);
      }
    }

    ug_message (" ");
    ug_cpu_message ("UG_IO    :");
  }

  if (Read_Task_Flag == 3)
    Error_Flag = ug_compress_file (1, File_Compression_Suffix_, File_Name);

  if (Read_Task_Flag == 3)
    ug_io_save_func_data (*Number_of_Nodes,
                          *Number_of_U_Scalars, *Number_of_U_Vectors,
                          &U_Scalar_Labels, &U_Vector_Labels,
                          &U_Scalar_Flags, &U_Vector_Flags,
                          &U_Scalars, &U_Vectors);

  return (0);

}
