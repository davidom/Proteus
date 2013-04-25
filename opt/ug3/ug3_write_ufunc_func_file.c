#include "UG3_LIB.h"

INT_ ug3_write_ufunc_func_file
 (char File_Name[],
  char File_Format_Suffix[],
  FILE **Func_File,
  INT_ Message_Flag,
  INT_ Write_Task_Flag,
  INT_ Number_of_Nodes,
  INT_ Number_of_U_Scalars,
  INT_ Number_of_U_Vectors,
  char U_Scalar_Label[],
  char U_Vector_Label[],
  DOUBLE_1D * U_Scalar,
  DOUBLE_3D * U_Vector)

{

/*
 * Write solution data to a UFUNC function file. Incrementally write one of six
 * steps if Write_Task_Flag > 0 or write everything at once if
 * Write_Task_Flag = 0. If Write_Task_Flag = 0 then only one scalar function
 * and/or vector function can be written.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_write_ufunc_func_file.c,v 1.24 2013/03/19 04:35:21 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX drive, dname, fname, ext;

  CHAR_31 Name_Text;
  CHAR_133 Text;
  CHAR_21 Label;

  static INT_ File_Format;

  INT_ Index, Mode_Flag, Write_Flag;

  if (ug_file_status_monitor_flag())
  {
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "ug3_write_ufunc_func_file: writing UFUNC file with File_Name=%s File_Format_Suffix=%s Write_Task_Flag=%i", Name_Text, File_Format_Suffix, Write_Task_Flag);
    ug_message (Text);
  }

  if (Write_Task_Flag == 0 || Write_Task_Flag == 1)
  {
    File_Format = ug3_file_format (File_Format_Suffix);

    if (File_Format == 0)
      return (342);

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
        sprintf (Text, "ug3_write_ufunc_func_file: opening TMP file with name %s", Name_Text);
        ug_message (Text);
      }

      *Func_File = ug_fopen (File_Name, "tmp");
    }
    else
    {
      ug_backup_file (File_Name);

      if (ug_file_status_monitor_flag())
      {
        sprintf (Text, "ug3_write_ufunc_func_file: opening regular file with name %s", Name_Text);
        ug_message (Text);
      }

      *Func_File = ug_fopen (File_Name, "w");
    }

    if (*Func_File == NULL)
      return (342);

    if (Message_Flag >= 1 && File_Format >= 1)
    { 
      ug_message (" ");
      ug_cpu_message ("UG3      :");
      ug_message (" ");
      ug_message ("UG3      : OUTPUT FUNCTION DATA");
      ug_message (" ");

      sprintf (Text, "UG3      : Function File Name= %s", Name_Text);
      ug_message (Text);
      sprintf (Text, "UG3      : Scalar Variables  =%10i", Number_of_U_Scalars);
      ug_message (Text);
      sprintf (Text, "UG3      : Vector Variables  =%10i", Number_of_U_Vectors);
      ug_message (Text);
    }

    if (File_Format == 1)
      Write_Flag = fprintf (*Func_File, "%i %i %i\n",
                            Number_of_Nodes,
                            Number_of_U_Scalars, Number_of_U_Vectors);
    else
    {
      Write_Flag = 0;
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                           sizeof (INT_), 1, *Func_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_U_Scalars,
                                           sizeof (INT_), 1, *Func_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_U_Vectors,
                                           sizeof (INT_), 1, *Func_File);
      Write_Flag = Write_Flag - 3;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (*Func_File);
      return (342);
    }
  }

  if ((Write_Task_Flag == 0 || Write_Task_Flag == 2) && Number_of_U_Scalars > 0)
  {
    strcpy (Label, "");
    strncat (Label, U_Scalar_Label, 20);

    if (File_Format == 1)
      Write_Flag = fprintf (*Func_File, "%s\n", Label);
    else
    {
      Write_Flag = 0;
      Write_Flag = Write_Flag + ug_fwrite (Label,
                                           sizeof (char), 21, *Func_File);
      Write_Flag = Write_Flag - 21;
    }

    if (Write_Flag < 0 || (Write_Task_Flag == 0 && Number_of_U_Scalars > 1))
    {
      ug_fclose (*Func_File);
      return (342);
    }

    if (Message_Flag >= 1 && File_Format >= 1)
    { 
      sprintf (Text, "UG3      : Scalar Label      = %s", Label);
      ug_message (Text);
    }
  }

  if ((Write_Task_Flag == 0 || Write_Task_Flag == 3) && Number_of_U_Vectors > 0)
  {
    strcpy (Label, "");
    strncat (Label, U_Vector_Label, 20);

    if (File_Format == 1)
      Write_Flag = fprintf (*Func_File, "%s\n", Label);
    else
    {
      Write_Flag = 0;
      Write_Flag = Write_Flag + ug_fwrite (Label,
                                           sizeof (char), 21, *Func_File);
      Write_Flag = Write_Flag - 21;
    }

    if (Write_Flag < 0 || (Write_Task_Flag == 0 && Number_of_U_Vectors > 1))
    {
      ug_fclose (*Func_File);
      return (342);
    }

    if (Message_Flag >= 1 && File_Format >= 1)
    { 
      sprintf (Text, "UG3      : Vector Label      = %s", Label);
      ug_message (Text);
    }
  }

  if ((Write_Task_Flag == 0 || Write_Task_Flag == 4) && Number_of_U_Scalars > 0)
  {
    if (File_Format == 1)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        Write_Flag = fprintf (*Func_File, "%.15g\n", U_Scalar[Index]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&U_Scalar[1],
                              sizeof (double), Number_of_Nodes, *Func_File);
      Write_Flag = Write_Flag - Number_of_Nodes;
    }

    if (Write_Flag < 0 || (Write_Task_Flag == 0 && Number_of_U_Scalars > 1))
    {
      ug_fclose (*Func_File);
      return (342);
    }
  }

  if ((Write_Task_Flag == 0 || Write_Task_Flag == 5) && Number_of_U_Vectors > 0)
  {
    if (File_Format == 1)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        Write_Flag = fprintf (*Func_File, "%.15g %.15g %.15g\n",
                              U_Vector[Index][0],
                              U_Vector[Index][1],
                              U_Vector[Index][2]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&U_Vector[1][0],
                              sizeof (double), Number_of_Nodes*3, *Func_File);
      Write_Flag = Write_Flag - Number_of_Nodes*3;
    }

    if (Write_Flag < 0 || (Write_Task_Flag == 0 && Number_of_U_Vectors > 1))
    {
      ug_fclose (*Func_File);
      return (342);
    }
  }

  if (Write_Task_Flag == 0 || Write_Task_Flag == 6)
  {
    Mode_Flag = ug_get_file_stream_mode_flag (*Func_File);

    if (Mode_Flag == UG_FIO_TMP_FILE_MODE)
      ug_rewind (*Func_File);
    else
      ug_fclose (*Func_File);

    if (Message_Flag >= 1 && File_Format >= 1)
      ug_message (" ");
  }

  return (0);

}
