#include "UG_IO_LIB.h"

INT_ ug_io_read_ufunc
 (FILE * Func_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Read_Task_Flag,
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
 * Read function data from a UFUNC function file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_ufunc.c,v 1.17 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  DOUBLE_1D * U_Scalar;
  DOUBLE_3D * U_Vector;

  CHAR_21 Label;
  CHAR_133 Long_Label;

  static INT_ Number_of_U_Func_Scalars, Number_of_U_Func_Vectors;

  INT_ Index, Label_Length,
       Max_Label_Length, Mode_Flag, 
       Number_of_Bytes, 
       Number_of_Functions, Number_of_Read_Items, Read_Flag, Special_Read_Items,
       U_Scalar_Index, U_Scalar_Location, U_Scalar_Location_I, U_Vector_Index,
       U_Vector_Location, U_Vector_Location_I;

  double TMP_double;

  float fc0, TMP_float, TMP_float_1, TMP_float_2, TMP_float_3;

  fc0 = 0.0;

  Max_Label_Length = (INT_) sizeof (CHAR_21) - 1;

  if (File_Format == UG_IO_FORMATTED)
  {
    Read_Flag = fscanf (Func_File, "%i %i %i", Number_of_Nodes,
                        &Number_of_U_Func_Scalars, &Number_of_U_Func_Vectors);

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading UFUNC function file");
      return (1);
    }

    if (Read_Task_Flag == 1)
    {
      Number_of_Functions = Number_of_U_Func_Scalars + Number_of_U_Func_Vectors;

      if (Number_of_Functions <= 0)
      {
        strcpy (Error_Message, "UFUNC function file has 0 functions");
        return (-1);
      }

      *Number_of_U_Scalars = *Number_of_U_Scalars + Number_of_U_Func_Scalars;
      *Number_of_U_Vectors = *Number_of_U_Vectors + Number_of_U_Func_Vectors;
    }

    if (Read_Task_Flag >= 2)
    {
      for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
           U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
      {
        Read_Flag = fscanf (Func_File, "%s\n", Long_Label);

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }

        if (Read_Task_Flag == 2)
        {
          Label_Length = (INT_) strcspn (Long_Label, " ");
          Label_Length = MIN (Label_Length, Max_Label_Length);

          strcpy (U_Scalar_Labels[U_Scalar_Index], "");

          strncat (U_Scalar_Labels[U_Scalar_Index], Long_Label, Label_Length);
        }
      }

      for (U_Vector_Index = *Number_of_U_Vectors-Number_of_U_Func_Vectors;
           U_Vector_Index < *Number_of_U_Vectors; ++U_Vector_Index)
      {
        Read_Flag = fscanf (Func_File, "%s\n", Long_Label);

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }

        if (Read_Task_Flag == 2)
        {
          Label_Length = (INT_) strcspn (Long_Label, " ");
          Label_Length = MIN (Label_Length, Max_Label_Length);

          strcpy (U_Vector_Labels[U_Vector_Index], "");

          strncat (U_Vector_Labels[U_Vector_Index], Long_Label, Label_Length);
        }
      }
    }

    if (Read_Task_Flag == 2)
    {
      for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
           U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
      {
        U_Scalar_Flags[U_Scalar_Index] = 1;
      }

      for (U_Vector_Index = *Number_of_U_Vectors-Number_of_U_Func_Vectors;
           U_Vector_Index < *Number_of_U_Vectors; ++U_Vector_Index)
      {
        U_Vector_Flags[U_Vector_Index] = 1;
      }
    }

    ++Read_Task_Flag;

    if (Read_Task_Flag == 4)
    {
      U_Scalar_Location = 0;

      for (U_Scalar_Index = 0;
           U_Scalar_Index < *Number_of_U_Scalars - Number_of_U_Func_Scalars;
           ++U_Scalar_Index)
      {
        if (U_Scalar_Flags[U_Scalar_Index] != 0)
          U_Scalar_Location = U_Scalar_Location + (*Number_of_Nodes) + 1;
      }

      U_Vector_Location = 0;

      for (U_Vector_Index = 0;
           U_Vector_Index < *Number_of_U_Vectors - Number_of_U_Func_Vectors;
           ++U_Vector_Index)
      {
        if (U_Vector_Flags[U_Vector_Index] != 0)
          U_Vector_Location = U_Vector_Location + (*Number_of_Nodes) + 1;
      }

      U_Scalar_Location_I = U_Scalar_Location;
      U_Vector_Location_I = U_Vector_Location;

      for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
           U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
      {
        if (U_Scalar_Flags[U_Scalar_Index] == 1)
        {
          U_Scalar = &U_Scalars[U_Scalar_Location_I];

          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Func_File, "%lg", &U_Scalar[Index]);
          }

          U_Scalar_Location_I = U_Scalar_Location_I + (*Number_of_Nodes) + 1;
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Func_File, "%g", &TMP_float);
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }
      }

      for (U_Vector_Index = *Number_of_U_Vectors-Number_of_U_Func_Vectors;
           U_Vector_Index < *Number_of_U_Vectors; ++U_Vector_Index)
      {
        if (U_Vector_Flags[U_Vector_Index] == 1)
        {
          U_Vector = &U_Vectors[U_Vector_Location_I];

          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Func_File, "%lg %lg %lg", &U_Vector[Index][0],
                                &U_Vector[Index][1], &U_Vector[Index][2]);
          }

          U_Vector_Location_I = U_Vector_Location_I + (*Number_of_Nodes) + 1;
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Func_File, "%g", &TMP_float);
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }
      }
    }
  }
  else
  {
    Mode_Flag = ug_get_file_stream_mode_flag (Func_File);

    Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                           File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    if (Read_Task_Flag == 1 || Mode_Flag == UG_FIO_STANDARD_FILE_MODE)
    {
      Read_Flag = 0;
      
      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                          sizeof (INT_), 1, Func_File);

      Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                        sizeof (INT_), 1, Func_File);
      Read_Flag = Read_Flag + ug_fread (&Number_of_U_Func_Scalars,
                                        sizeof (INT_), 1, Func_File);
      Read_Flag = Read_Flag + ug_fread (&Number_of_U_Func_Vectors,
                                        sizeof (INT_), 1, Func_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Func_File);

      Number_of_Read_Items = 3 + Special_Read_Items + Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items)
      {
        strcpy (Error_Message, "error reading UFUNC function file");
        return (1);
      }
    }

    if (Read_Task_Flag == 1)
    {
      Number_of_Functions = Number_of_U_Func_Scalars + Number_of_U_Func_Vectors;

      if (Number_of_Functions <= 0)
      {
        strcpy (Error_Message, "UFUNC function file has 0 functions");
        return (-1);
      }

      *Number_of_U_Scalars = *Number_of_U_Scalars + Number_of_U_Func_Scalars;
      *Number_of_U_Vectors = *Number_of_U_Vectors + Number_of_U_Func_Vectors;
    }

    if (Read_Task_Flag == 2 ||
        (Read_Task_Flag >= 2 && Mode_Flag == UG_FIO_STANDARD_FILE_MODE))
    {
      for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
           U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE) 
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        Read_Flag = Read_Flag + ug_fread (&Label, sizeof (char), 21, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        Number_of_Read_Items = 21 + Special_Read_Items + Special_Read_Items;

        if (Read_Flag != Number_of_Read_Items)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }

        if (Read_Task_Flag == 2)
        {
          Label_Length = (INT_) strcspn (Label, " ");
          Label_Length = MIN (Label_Length, Max_Label_Length);

          strcpy (U_Scalar_Labels[U_Scalar_Index], "");

          strncat (U_Scalar_Labels[U_Scalar_Index], Label, Label_Length);
        }
      }

      for (U_Vector_Index = *Number_of_U_Vectors-Number_of_U_Func_Vectors;
           U_Vector_Index < *Number_of_U_Vectors; ++U_Vector_Index)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE) 
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        Read_Flag = Read_Flag + ug_fread (&Label, sizeof (char), 21, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        Number_of_Read_Items = 21 + Special_Read_Items + Special_Read_Items;

        if (Read_Flag != Number_of_Read_Items)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }

        if (Read_Task_Flag == 2)
        {
          Label_Length = (INT_) strcspn (Label, " ");
          Label_Length = MIN (Label_Length, Max_Label_Length);

          strcpy (U_Vector_Labels[U_Vector_Index], "");

          strncat (U_Vector_Labels[U_Vector_Index], Label, Label_Length);
        }
      }
    }

    if (Read_Task_Flag == 2)
    {
      for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
           U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
      {
        U_Scalar_Flags[U_Scalar_Index] = 1;
      }

      for (U_Vector_Index = *Number_of_U_Vectors-Number_of_U_Func_Vectors;
           U_Vector_Index < *Number_of_U_Vectors; ++U_Vector_Index)
      {
        U_Vector_Flags[U_Vector_Index] = 1;
      }
    }

    ++Read_Task_Flag;

    if (Read_Task_Flag == 4)
    {
      U_Scalar_Location = 0;

      for (U_Scalar_Index = 0;
           U_Scalar_Index < *Number_of_U_Scalars - Number_of_U_Func_Scalars;
           ++U_Scalar_Index)
      {
        if (U_Scalar_Flags[U_Scalar_Index] != 0)
          U_Scalar_Location = U_Scalar_Location + (*Number_of_Nodes) + 1;
      }

      U_Vector_Location = 0;

      for (U_Vector_Index = 0;
           U_Vector_Index < *Number_of_U_Vectors - Number_of_U_Func_Vectors;
           ++U_Vector_Index)
      {
        if (U_Vector_Flags[U_Vector_Index] != 0)
          U_Vector_Location = U_Vector_Location + (*Number_of_Nodes) + 1;
      }

      U_Scalar_Location_I = U_Scalar_Location;
      U_Vector_Location_I = U_Vector_Location;

      for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
           U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        if (U_Scalar_Flags[U_Scalar_Index] == 1)
        {
          U_Scalar = &U_Scalars[U_Scalar_Location_I];

          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            TMP_float = fc0;

            for (Index = 1; Index <= *Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag + ug_fread (&TMP_float,
                                                sizeof (float), 1, Func_File);

              U_Scalar[Index] = (double) TMP_float;
            }
          }
          else
            Read_Flag = Read_Flag + ug_fread (&U_Scalar[1],
                                              sizeof (DOUBLE_1D),
                                              *Number_of_Nodes,
                                              Func_File);

          U_Scalar_Location_I = U_Scalar_Location_I + (*Number_of_Nodes) + 1;
        }
        else
        {
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            for (Index = 1; Index <= *Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag + ug_fread (&TMP_float,
                                                sizeof (float), 1, Func_File);
            }
          }
          else
          {
            for (Index = 1; Index <= *Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag + ug_fread (&TMP_double,
                                                sizeof (double), 1,
                                                Func_File);
            }
          }
        }

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        Number_of_Read_Items = (*Number_of_Nodes)
                             + Special_Read_Items + Special_Read_Items;

        if (Read_Flag != Number_of_Read_Items)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }
      }

      for (U_Vector_Index = *Number_of_U_Vectors-Number_of_U_Func_Vectors;
           U_Vector_Index < *Number_of_U_Vectors; ++U_Vector_Index)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        if (U_Vector_Flags[U_Vector_Index] == 1)
        {
          U_Vector = &U_Vectors[U_Vector_Location_I];

          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            TMP_float_1 = fc0;
            TMP_float_2 = fc0;
            TMP_float_3 = fc0;

            for (Index = 1; Index <= *Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag + ug_fread (&TMP_float_1,
                                                sizeof (float), 1, Func_File);
              Read_Flag = Read_Flag + ug_fread (&TMP_float_2,
                                                sizeof (float), 1, Func_File);
              Read_Flag = Read_Flag + ug_fread (&TMP_float_3,
                                                sizeof (float), 1, Func_File);

              U_Vector[Index][0] = (double) TMP_float_1;
              U_Vector[Index][1] = (double) TMP_float_2;
              U_Vector[Index][2] = (double) TMP_float_3;
            }
          }
          else
            Read_Flag = Read_Flag + ug_fread (&U_Vector[1][0],
                                              sizeof (double),
                                              (*Number_of_Nodes)*3,
                                              Func_File);

          U_Vector_Location_I = U_Vector_Location_I + (*Number_of_Nodes) + 1;
        }
        else
        {
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            for (Index = 1; Index <= *Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag + ug_fread (&TMP_float,
                                                sizeof (float), 1, Func_File);
              Read_Flag = Read_Flag + ug_fread (&TMP_float,
                                                sizeof (float), 1, Func_File);
              Read_Flag = Read_Flag + ug_fread (&TMP_float,
                                                sizeof (float), 1, Func_File);
            }
          }
          else
          {
            for (Index = 1; Index <= *Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag + ug_fread (&TMP_double,
                                                sizeof (double), 1,
                                                Func_File);
              Read_Flag = Read_Flag + ug_fread (&TMP_double,
                                                sizeof (double), 1,
                                                Func_File);
              Read_Flag = Read_Flag + ug_fread (&TMP_double,
                                                sizeof (double), 1,
                                                Func_File);
            }
          }
        }

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        Number_of_Read_Items = 3 * (*Number_of_Nodes)
                             + Special_Read_Items + Special_Read_Items;

        if (Read_Flag != Number_of_Read_Items)
        {
          strcpy (Error_Message, "error reading UFUNC function file");
          return (1);
        }
      }
    }
  }

  return (0);

}
