#include "UG3_LIB.h"

INT_ ug3_read_ufunc_func_file
 (char File_Name[],
  char File_Format_Suffix[],
  FILE **Func_File,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_U_Scalars,
  INT_ *Number_of_U_Vectors,
  char U_Scalar_Label[],
  char U_Vector_Label[],
  DOUBLE_1D * U_Scalar,
  DOUBLE_3D * U_Vector)

{

/*
 * Incrementally read solution data from a UFUNC function file.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_read_ufunc_func_file.c,v 1.19 2013/03/19 04:35:33 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX drive, dname, fname, ext;

  CHAR_133 Text;
  CHAR_31 Name_Text;
  CHAR_21 Label;

  static INT_ Mode_Flag, File_Format;

  INT_ Index, Read_Flag;

  if (ug_file_status_monitor_flag())
  {
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "ug3_read_ufunc_grid_file : reading UFUNC file with File_Name=%s File_Format_Suffix=%s Read_Task_Flag=%i", Name_Text, File_Format_Suffix, Read_Task_Flag);
  }

  if (Read_Task_Flag == 1)
  {
    File_Format = ug3_file_format (File_Format_Suffix);

    if (File_Format == 0)
      return (341);

    strcat (File_Name, File_Format_Suffix);
    strcat (File_Name, ".ufunc");

    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);

    if (File_Format == 3)
    {
      strcat (fname, "+");
      strcpy (File_Name, fname);
    }

    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    if (File_Format == 3)
    {
      if (ug_file_status_monitor_flag())
      {
        sprintf (Text, "ug3_read_ufunc_grid_file : opening TMP file with name %s", Name_Text);
        ug_message (Text);
      }

      *Func_File = ug_fopen (File_Name, "r_tmp");
    }
    else
    {
      if (ug_file_status_monitor_flag())
      {
        sprintf (Text, "ug3_read_ufunc_grid_file : opening regular file with name %s", Name_Text);
        ug_message (Text);
      }

      *Func_File = ug_fopen (File_Name, "r");
    }

    if (*Func_File == NULL)
      return (341);

    if (File_Format == 1)
      Read_Flag = fscanf (*Func_File, "%i %i %i",
                          Number_of_Nodes,
                          Number_of_U_Scalars, Number_of_U_Vectors);
    else
    {
      Read_Flag = 0;
      Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                        sizeof (INT_), 1, *Func_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_U_Scalars,
                                        sizeof (INT_), 1, *Func_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_U_Vectors,
                                        sizeof (INT_), 1, *Func_File);
      Read_Flag = Read_Flag - 3;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (*Func_File);
      return (341);
    }
  }

  else if (Read_Task_Flag == 2)
  {
    if (File_Format == 1)
      Read_Flag = fscanf (*Func_File, "%s", Label);
    else
    {
      Read_Flag = 0;
      Read_Flag = Read_Flag + ug_fread (Label,
                                        sizeof (char), 21, *Func_File);
      Read_Flag = Read_Flag - 21;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (*Func_File);
      return (341);
    }

    strcpy (U_Scalar_Label, "");
    strncat (U_Scalar_Label, Label, 20);
  }

  if (Read_Task_Flag == 3)
  {
    if (File_Format == 1)
      Read_Flag = fscanf (*Func_File, "%s", Label);
    else
    {
      Read_Flag = 0;
      Read_Flag = Read_Flag + ug_fread (Label,
                                        sizeof (char), 21, *Func_File);
      Read_Flag = Read_Flag - 21;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (*Func_File);
      return (341);
    }

    strcpy (U_Vector_Label, "");
    strncat (U_Vector_Label, Label, 20);
  }

  if (Read_Task_Flag == 4)
  {
    if (File_Format == 1)
    {
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (*Func_File, "%lg", &U_Scalar[Index]);
      }
    }
    else
    {
      Read_Flag = ug_fread (&U_Scalar[1],
                            sizeof (double), *Number_of_Nodes, *Func_File);
      Read_Flag = Read_Flag - *Number_of_Nodes;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (*Func_File);
      return (341);
    }
  }

  if (Read_Task_Flag == 5)
  {
    if (File_Format == 1)
    {
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (*Func_File, "%lg %lg %lg", &U_Vector[Index][0],
                                                       &U_Vector[Index][1],
                                                       &U_Vector[Index][2]);
      }
    }
    else
    {
      Read_Flag = ug_fread (&U_Vector[1][0],
                            sizeof (double), *Number_of_Nodes*3, *Func_File);
      Read_Flag = Read_Flag - *Number_of_Nodes*3;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (*Func_File);
      return (341);
    }
  }

  if (Read_Task_Flag == 6)
  {
    Mode_Flag = ug_get_file_stream_mode_flag (*Func_File);

    if (Mode_Flag == UG_FIO_TMP_FILE_MODE)
      ug_rewind (*Func_File);
    else
      ug_fclose (*Func_File);
  }

  return (0);

}
