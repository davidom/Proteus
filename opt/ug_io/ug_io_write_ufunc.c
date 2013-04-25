#include "UG_IO_LIB.h"

INT_ ug_io_write_ufunc
 (FILE * Func_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Number_of_Nodes,
  INT_ Number_of_U_Scalars,
  INT_ Number_of_U_Vectors,
  CHAR_21 * U_Scalar_Labels,
  CHAR_21 * U_Vector_Labels,
  INT_1D * U_Scalar_Flags,
  INT_1D * U_Vector_Flags,
  DOUBLE_1D * U_Scalars,
  DOUBLE_3D * U_Vectors)

{

/*
 * Write function data to a UFUNC function file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_ufunc.c,v 1.11 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_21 * U_Scalar_Label;
  CHAR_21 * U_Vector_Label;

  DOUBLE_1D * U_Scalar;

  DOUBLE_3D * U_Vector;

  INT_ Index, Number_of_Bytes, Number_of_U_Scalars_Active,
       Number_of_U_Vectors_Active, Number_of_Write_Items, Print_Loop,
       Print_Loop_Limit, Write_Flag, Special_Write_Items, U_Scalar_Flag,
       U_Scalar_Index, U_Scalar_Location, U_Vector_Flag, U_Vector_Index,
       U_Vector_Location;

  float TMP_float, TMP_float_1, TMP_float_2, TMP_float_3;

  Print_Loop_Limit = 5;

  Number_of_U_Scalars_Active = 0;

  for (U_Scalar_Index = 0;
       U_Scalar_Index < Number_of_U_Scalars; ++U_Scalar_Index)
  {
    if (U_Scalar_Flags[U_Scalar_Index] == 1)
      ++Number_of_U_Scalars_Active;
  }

  Number_of_U_Vectors_Active = 0;

  for (U_Vector_Index = 0;
       U_Vector_Index < Number_of_U_Vectors; ++U_Vector_Index)
  {
    if (U_Vector_Flags[U_Vector_Index] == 1)
      ++Number_of_U_Vectors_Active;
  }

  if (File_Format == UG_IO_FORMATTED)
  {
    Write_Flag = fprintf (Func_File, "%i %i %i\n",
                          Number_of_Nodes, Number_of_U_Scalars_Active,
                          Number_of_U_Vectors_Active);

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UFUNC function file");
      return (1);
    }

    for (U_Scalar_Index = 0;
         U_Scalar_Index < Number_of_U_Scalars; ++U_Scalar_Index)
    {
      if (U_Scalar_Flags[U_Scalar_Index] == 1)
      {
        Write_Flag = fprintf (Func_File, "%s\n",
                              U_Scalar_Labels[U_Scalar_Index]);

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }
    }

    for (U_Vector_Index = 0;
         U_Vector_Index < Number_of_U_Vectors; ++U_Vector_Index)
    {
      if (U_Vector_Flags[U_Vector_Index] == 1)
      {
        Write_Flag = fprintf (Func_File, "%s\n",
                              U_Vector_Labels[U_Vector_Index]);

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }
    }

    Number_of_Nodes = Number_of_Nodes;

    U_Scalar_Location = 0;
    U_Vector_Location = 0;

    for (U_Scalar_Index = 0;
         U_Scalar_Index < Number_of_U_Scalars; ++U_Scalar_Index)
    {
      U_Scalar_Flag = U_Scalar_Flags[U_Scalar_Index];

      if (U_Scalar_Flag == 1)
      {
        U_Scalar = &U_Scalars[U_Scalar_Location];

        Index = 1;

        do
        {
          if (Index < Number_of_Nodes)
          {
            Print_Loop = 1;

            do
            {
              Write_Flag = fprintf (Func_File, "%.15g ", U_Scalar[Index]);

              ++Index;

              ++Print_Loop;
            }
            while (Print_Loop < Print_Loop_Limit && Index < Number_of_Nodes);
          }

          Write_Flag = fprintf (Func_File, "%.15g\n", U_Scalar[Index]);

          ++Index;
        }
        while (Index <= Number_of_Nodes);

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }

      if (U_Scalar_Flag != 0)
        U_Scalar_Location = U_Scalar_Location + Number_of_Nodes + 1;
    }

    for (U_Vector_Index = 0;
         U_Vector_Index < Number_of_U_Vectors; ++U_Vector_Index)
    {
      U_Vector_Flag = U_Vector_Flags[U_Vector_Index];

      if (U_Vector_Flag == 1)
      {
        U_Vector = &U_Vectors[U_Vector_Location];

        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          Write_Flag = fprintf (Func_File, "%.15g %.15g %.15g\n",
                                U_Vector[Index][0],
                                U_Vector[Index][1],
                                U_Vector[Index][2]);
        }

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }

      if (U_Vector_Flag != 0)
        U_Vector_Location = U_Vector_Location + Number_of_Nodes + 1;
    }
  }
  else
  {
    Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                            File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Number_of_Bytes = 3 * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;
    
    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Func_File);

    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                         sizeof (INT_), 1, Func_File);

    Write_Flag = Write_Flag + ug_fwrite (&Number_of_U_Scalars_Active,
                                         sizeof (INT_), 1, Func_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_U_Vectors_Active,
                                         sizeof (INT_), 1, Func_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Func_File);

    Number_of_Write_Items = 3 + Special_Write_Items + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing UFUNC function file");
      return (1);
    }

    Number_of_Bytes = ((INT_) (sizeof (CHAR_21)));

    Number_of_Write_Items = 21 + Special_Write_Items + Special_Write_Items;

    for (U_Scalar_Index = 0;
         U_Scalar_Index < Number_of_U_Scalars; ++U_Scalar_Index)
    {
      if (U_Scalar_Flags[U_Scalar_Index] == 1)
      {
        U_Scalar_Label = &U_Scalar_Labels[U_Scalar_Index];

        Write_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE) 
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File); 

        Write_Flag = Write_Flag + ug_fwrite (U_Scalar_Label,
                                             sizeof (char), 21, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File);

        if (Write_Flag != Number_of_Write_Items)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }
    }

    for (U_Vector_Index = 0;
         U_Vector_Index < Number_of_U_Vectors; ++U_Vector_Index)
    {
      if (U_Vector_Flags[U_Vector_Index] == 1)
      {
        U_Vector_Label = &U_Vector_Labels[U_Vector_Index];

        Write_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE) 
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File); 

        Write_Flag = Write_Flag + ug_fwrite (U_Vector_Label,
                                             sizeof (char), 21, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File);

        if (Write_Flag != Number_of_Write_Items)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }
    }

    U_Scalar_Location = 0;
    U_Vector_Location = 0;

    if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
        File_Format == UG_IO_BINARY_SINGLE)
      Number_of_Bytes = Number_of_Nodes * ((INT_) (sizeof (float)));
    else
      Number_of_Bytes = Number_of_Nodes * ((INT_) (sizeof (double)));

    for (U_Scalar_Index = 0;
         U_Scalar_Index < Number_of_U_Scalars; ++U_Scalar_Index)
    {
      U_Scalar_Flag = U_Scalar_Flags[U_Scalar_Index];

      if (U_Scalar_Flag == 1)
      {
        U_Scalar = &U_Scalars[U_Scalar_Location];

        Write_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
            File_Format == UG_IO_BINARY_SINGLE)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            TMP_float = (float) U_Scalar[Index];

            Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                                 sizeof (float), 1,
                                                 Func_File);
          }
        }
        else
          Write_Flag = Write_Flag
                     + ug_fwrite (&U_Scalar[1], sizeof (DOUBLE_1D),
                                  Number_of_Nodes, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File);

        Number_of_Write_Items = Number_of_Nodes
                              + Special_Write_Items + Special_Write_Items;

        if (Write_Flag != Number_of_Write_Items)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }

      if (U_Scalar_Flag != 0)
        U_Scalar_Location = U_Scalar_Location + Number_of_Nodes + 1;
    }

    if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
        File_Format == UG_IO_BINARY_SINGLE)
      Number_of_Bytes = 3 * Number_of_Nodes * ((INT_) (sizeof (float)));
    else
      Number_of_Bytes = 3 * Number_of_Nodes * ((INT_) (sizeof (double)));

    for (U_Vector_Index = 0;
         U_Vector_Index < Number_of_U_Vectors; ++U_Vector_Index)
    {
      U_Vector_Flag = U_Vector_Flags[U_Vector_Index];

      if (U_Vector_Flag == 1)
      {
        U_Vector = &U_Vectors[U_Vector_Location];

        Write_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
            File_Format == UG_IO_BINARY_SINGLE)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            TMP_float_1 = (float) U_Vector[Index][0];
            TMP_float_2 = (float) U_Vector[Index][1];
            TMP_float_3 = (float) U_Vector[Index][2];

            Write_Flag = Write_Flag + ug_fwrite (&TMP_float_1,
                                                 sizeof (float), 1,
                                                 Func_File);
            Write_Flag = Write_Flag + ug_fwrite (&TMP_float_2,
                                                 sizeof (float), 1,
                                                 Func_File);
            Write_Flag = Write_Flag + ug_fwrite (&TMP_float_3,
                                                 sizeof (float), 1,
                                                 Func_File);
          }
        }
        else
          Write_Flag = Write_Flag
                     + ug_fwrite (&U_Vector[1][0], sizeof (double),
                                  Number_of_Nodes*3, Func_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Func_File);

        Number_of_Write_Items = Number_of_Nodes*3
                              + Special_Write_Items + Special_Write_Items;

        if (Write_Flag != Number_of_Write_Items)
        {
          strcpy (Error_Message, "error writing UFUNC function file");
          return (1);
        }
      }

      if (U_Vector_Flag != 0)
        U_Vector_Location = U_Vector_Location + Number_of_Nodes + 1;
    }
  }

  return (0);

}
