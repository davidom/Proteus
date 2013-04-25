#include "UG_IO_LIB.h"

INT_ ug_io_read_vfunc
 (FILE * Func_File,
  char Error_Message[],
  INT_ Block_Option_Flag,
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Blocks,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_U_Scalars,
  INT_ *Number_of_U_Vectors,
  CHAR_21 * U_Scalar_Labels,
  CHAR_21 * U_Vector_Labels,
  INT_1D * Block_Flags,
  INT_3D * Block_Sizes,
  INT_1D * U_Scalar_Flags,
  INT_1D * U_Vector_Flags,
  DOUBLE_1D * U_Scalars,
  DOUBLE_3D * U_Vectors)

{

/*
 * Read solution data from a UFUNC/VFUNC function file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_vfunc.c,v 1.24 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  DOUBLE_1D * U_Scalar;
  DOUBLE_3D * U_Vector;

  CHAR_21 Label;
  CHAR_133 Long_Label;

  static INT_ Number_of_U_Func_Scalars, Number_of_U_Func_Vectors;

  INT_ Block_Flag, Block_Index, Block_Location, Index, Label_Length,
       Max_Label_Length, Mode_Flag, N1, N2, N3, Number_of_Block_Nodes,
       Number_of_Bytes, Number_of_Func_Nodes, Number_of_Func_Blocks,
       Number_of_Functions, Number_of_Read_Items, Read_Flag, Special_Read_Items,
       U_Scalar_Index, U_Scalar_Location, U_Scalar_Location_I, U_Vector_Index,
       U_Vector_Location, U_Vector_Location_I;

  int true_int = 0;

  double TMP_double;

  float fc0, TMP_float, TMP_float_1, TMP_float_2, TMP_float_3;

  fc0 = 0.0;

  Max_Label_Length = (INT_) sizeof (CHAR_21) - 1;

  if (Block_Option_Flag == 0)
    *Number_of_Blocks = 1;

  if (Read_Task_Flag == 3 && Block_Option_Flag != 0)
  {
    Number_of_Func_Nodes = 0;

    for (Block_Index = 0; Block_Index < *Number_of_Blocks; ++Block_Index)
    {
      if (Block_Flags[Block_Index] != 0)
      {
        N1 = Block_Sizes[Block_Index][0];
        N2 = Block_Sizes[Block_Index][1];
        N3 = Block_Sizes[Block_Index][2];

        Number_of_Func_Nodes = Number_of_Func_Nodes + N1 * N2 * N3;
      }
    }

    if (*Number_of_Nodes == 0)
      *Number_of_Nodes = Number_of_Func_Nodes;
    else
    {
      if (Number_of_Func_Nodes != *Number_of_Nodes)
      {
        strcpy (Error_Message,
                "VFUNC flow file dimensions do not match existing");
        return (-1);
      }
    }

    return (0);
  }

  if (File_Format == UG_IO_FORMATTED)
  {
    Read_Flag = fscanf (Func_File, "%i", &true_int);

    Number_of_Func_Blocks = (INT_) true_int;

    Read_Flag = fscanf (Func_File, "%i", &true_int);

    Number_of_U_Func_Scalars = (INT_) true_int;

    Read_Flag = fscanf (Func_File, "%i", &true_int);

    Number_of_U_Func_Vectors = (INT_) true_int;

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
      return (1);
    }

    if (Read_Task_Flag == 1)
    {
      if (Block_Option_Flag == 0)
      {
        Number_of_Func_Nodes = Number_of_Func_Blocks;

        if (*Number_of_Nodes == 0)
          *Number_of_Nodes = Number_of_Func_Nodes;
        else
        {
          if (Number_of_Func_Nodes != *Number_of_Nodes)
          {
            strcpy (Error_Message,
                    "UFUNC function file dimensions do not match existing");
            return (-1);
          }
        }

        if (Number_of_Func_Nodes <= 0)
        {
          strcpy (Error_Message, "UFUNC function file has 0 nodes");
          return (-1);
        }
      }
      else
      {
        if (*Number_of_Blocks == 0)
          *Number_of_Blocks = Number_of_Func_Blocks;
        else
        {
          if (Number_of_Func_Blocks != *Number_of_Blocks)
          {
            strcpy (Error_Message,
                    "VFUNC function file blocks do not match existing");
            return (-1);
          }
        }

        if (Number_of_Func_Blocks <= 0)
        {
          strcpy (Error_Message, "VFUNC function file has 0 blocks");
          return (-1);
        }
      }

      Number_of_Functions = Number_of_U_Func_Scalars + Number_of_U_Func_Vectors;

      if (Number_of_Functions <= 0)
      {
        strcpy (Error_Message, "UFUNC/VFUNC function file has 0 functions");
        return (-1);
      }

      *Number_of_U_Scalars = *Number_of_U_Scalars + Number_of_U_Func_Scalars;
      *Number_of_U_Vectors = *Number_of_U_Vectors + Number_of_U_Func_Vectors;
    }

    if (Read_Task_Flag >= 2)
    {
      if (Block_Option_Flag != 0)
      {
        for (Block_Index = 0; Block_Index < *Number_of_Blocks; ++Block_Index)
        {
          Read_Flag = fscanf (Func_File, "%i", &true_int);

          N1 = (INT_) true_int;

          Read_Flag = fscanf (Func_File, "%i", &true_int);

          N2 = (INT_) true_int;

          Read_Flag = fscanf (Func_File, "%i", &true_int);

          N3 = (INT_) true_int;

          if (Read_Task_Flag == 2)
          {
            if (*Number_of_Nodes == 0)
            {
              if (N1 <= 0 || N2 <= 0 || N3 <= 0)
              {
                strcpy (Error_Message,
                        "VFUNC function file has block with 0 nodes");
                return (-1);
              }

              Block_Flags[Block_Index] = 1;

              Block_Sizes[Block_Index][0] = N1;
              Block_Sizes[Block_Index][1] = N2;
              Block_Sizes[Block_Index][2] = N3;
            }
            else
            {
              if (Block_Sizes[Block_Index][0] != N1 ||
                  Block_Sizes[Block_Index][1] != N2 ||
                  Block_Sizes[Block_Index][2] != N3)
              {
                strcpy (Error_Message,
                        "VFUNC function file dimensions do not match existing");
                return (-1);
              }
            }
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading VFUNC function file");
          return (1);
        }
      }

      for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
           U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
      {
        Read_Flag = fscanf (Func_File, "%s\n", Long_Label);

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
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
          strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
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

    if (Block_Option_Flag == 0)
      ++Read_Task_Flag;

    if (Read_Task_Flag == 4)
    {
      U_Scalar_Location = 0;

      for (U_Scalar_Index = 0;
           U_Scalar_Index < *Number_of_U_Scalars - Number_of_U_Func_Scalars;
           ++U_Scalar_Index)
      {
        if (U_Scalar_Flags[U_Scalar_Index] != 0)
          U_Scalar_Location = U_Scalar_Location + *Number_of_Nodes + 1;
      }

      U_Vector_Location = 0;

      for (U_Vector_Index = 0;
           U_Vector_Index < *Number_of_U_Vectors - Number_of_U_Func_Vectors;
           ++U_Vector_Index)
      {
        if (U_Vector_Flags[U_Vector_Index] != 0)
          U_Vector_Location = U_Vector_Location + *Number_of_Nodes + 1;
      }

      Block_Location = 0;

      for (Block_Index = 0; Block_Index < *Number_of_Blocks; ++Block_Index)
      {
        if (Block_Option_Flag == 0)
        {
          Block_Flag = 1;

          Number_of_Block_Nodes = *Number_of_Nodes;
        }
        else
        {
          Block_Flag = Block_Flags[Block_Index];

          N1 = Block_Sizes[Block_Index][0];
          N2 = Block_Sizes[Block_Index][1];
          N3 = Block_Sizes[Block_Index][2];

          Number_of_Block_Nodes = N1 * N2 * N3;
        }

        U_Scalar_Location_I = U_Scalar_Location;
        U_Vector_Location_I = U_Vector_Location;

        for (U_Scalar_Index = *Number_of_U_Scalars-Number_of_U_Func_Scalars;
             U_Scalar_Index < *Number_of_U_Scalars; ++U_Scalar_Index)
        {
          if (U_Scalar_Flags[U_Scalar_Index] != 0 && Block_Flag != 0)
          {
            U_Scalar = &U_Scalars[U_Scalar_Location_I];

            for (Index = Block_Location+1;
                 Index <= Block_Location+Number_of_Block_Nodes; ++Index)
            {
              Read_Flag = fscanf (Func_File, "%lg", &U_Scalar[Index]);
            }

            U_Scalar_Location_I = U_Scalar_Location_I + *Number_of_Nodes + 1;
          }
          else
          {
            for (Index = 1; Index <= Number_of_Block_Nodes; ++Index)
            {
              Read_Flag = fscanf (Func_File, "%g", &TMP_float);
            }
          }

          if (Read_Flag == EOF)
          {
            strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
            return (1);
          }
        }

        for (U_Vector_Index = *Number_of_U_Vectors-Number_of_U_Func_Vectors;
             U_Vector_Index < *Number_of_U_Vectors; ++U_Vector_Index)
        {
          if (U_Vector_Flags[U_Vector_Index] != 0 && Block_Flag != 0)
          {
            U_Vector = &U_Vectors[U_Vector_Location_I];

            for (Index = Block_Location+1;
                 Index <= Block_Location+Number_of_Block_Nodes; ++Index)
            {
              Read_Flag = fscanf (Func_File, "%lg %lg %lg", &U_Vector[Index][0],
                                  &U_Vector[Index][1], &U_Vector[Index][2]);
            }

            U_Vector_Location_I = U_Vector_Location_I + *Number_of_Nodes + 1;
          }
          else
          {
            for (Index = 1; Index <= Number_of_Block_Nodes; ++Index)
            {
              Read_Flag = fscanf (Func_File, "%g", &TMP_float);
            }
          }

          if (Read_Flag == EOF)
          {
            strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
            return (1);
          }
        }

        if (Block_Flag != 0)
          Block_Location = Block_Location + Number_of_Block_Nodes;
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
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Func_File);

      Read_Flag = Read_Flag + ug_fread (&Number_of_Func_Blocks,
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
        strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
        return (1);
      }
    }

    if (Read_Task_Flag == 1)
    {
      if (Block_Option_Flag == 0)
      {
        Number_of_Func_Nodes = Number_of_Func_Blocks;

        if (*Number_of_Nodes == 0)
          *Number_of_Nodes = Number_of_Func_Nodes;
        else
        {
          if (Number_of_Func_Nodes != *Number_of_Nodes)
          {
            strcpy (Error_Message,
                    "UFUNC function file dimensions do not match existing");
            return (-1);
          }
        }

        if (Number_of_Func_Nodes <= 0)
        {
          strcpy (Error_Message, "UFUNC function file has 0 nodes");
          return (-1);
        }
      }
      else
      {
        if (*Number_of_Blocks == 0)
          *Number_of_Blocks = Number_of_Func_Blocks;
        else
        {
          if (Number_of_Func_Blocks != *Number_of_Blocks)
          {
            strcpy (Error_Message,
                    "VFUNC function file blocks do not match existing");
            return (-1);
          }
        }

        if (Number_of_Func_Blocks <= 0)
        {
          strcpy (Error_Message, "VFUNC function file has 0 blocks");
          return (-1);
        }
      }

      Number_of_Functions = Number_of_U_Func_Scalars + Number_of_U_Func_Vectors;

      if (Number_of_Functions <= 0)
      {
        strcpy (Error_Message, "UFUNC/VFUNC function file has 0 functions");
        return (-1);
      }

      *Number_of_U_Scalars = *Number_of_U_Scalars + Number_of_U_Func_Scalars;
      *Number_of_U_Vectors = *Number_of_U_Vectors + Number_of_U_Func_Vectors;
    }

    if (Read_Task_Flag == 2 ||
        (Read_Task_Flag >= 2 && Mode_Flag == UG_FIO_STANDARD_FILE_MODE))
    {
      if (Block_Option_Flag != 0)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        for (Block_Index = 0; Block_Index < *Number_of_Blocks; ++Block_Index)
        {
          Read_Flag = Read_Flag + ug_fread (&N1, sizeof (INT_), 1, Func_File);
          Read_Flag = Read_Flag + ug_fread (&N2, sizeof (INT_), 1, Func_File);
          Read_Flag = Read_Flag + ug_fread (&N3, sizeof (INT_), 1, Func_File);

          if (Read_Task_Flag == 2)
          {
            if (*Number_of_Nodes == 0)
            {
              Block_Flags[Block_Index] = 1;

              Block_Sizes[Block_Index][0] = N1;
              Block_Sizes[Block_Index][1] = N2;
              Block_Sizes[Block_Index][2] = N3;

              if (N1 <= 0 || N2 <= 0 || N3 <= 0)
              {
                strcpy (Error_Message,
                        "VFUNC function file has block with 0 nodes");
                return (-1);
              }
            }
            else
            {
              if (Block_Sizes[Block_Index][0] != N1 ||
                  Block_Sizes[Block_Index][1] != N2 ||
                  Block_Sizes[Block_Index][2] != N3)
              {
                strcpy (Error_Message,
                        "VFUNC function file dimensions do not match existing");
                return (-1);
              }
            }
          }
        }

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Func_File);

        Number_of_Read_Items = 3 * (*Number_of_Blocks)
                             + Special_Read_Items + Special_Read_Items;

        if (Read_Flag != Number_of_Read_Items)
        {
          strcpy (Error_Message, "error reading VFUNC function file");
          return (1);
        }
      }

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
          strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
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
          strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
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

    if (Block_Option_Flag == 0)
      ++Read_Task_Flag;

    if (Read_Task_Flag == 4)
    {
      U_Scalar_Location = 0;

      for (U_Scalar_Index = 0;
           U_Scalar_Index < *Number_of_U_Scalars - Number_of_U_Func_Scalars;
           ++U_Scalar_Index)
      {
        if (U_Scalar_Flags[U_Scalar_Index] != 0)
          U_Scalar_Location = U_Scalar_Location + *Number_of_Nodes + 1;
      }

      U_Vector_Location = 0;

      for (U_Vector_Index = 0;
           U_Vector_Index < *Number_of_U_Vectors - Number_of_U_Func_Vectors;
           ++U_Vector_Index)
      {
        if (U_Vector_Flags[U_Vector_Index] != 0)
          U_Vector_Location = U_Vector_Location + *Number_of_Nodes + 1;
      }

      Block_Location = 0;

      for (Block_Index = 0; Block_Index < *Number_of_Blocks; ++Block_Index)
      {
        if (Block_Option_Flag == 0)
        {
          Block_Flag = 1;

          Number_of_Block_Nodes = *Number_of_Nodes;
        }
        else
        {
          Block_Flag = Block_Flags[Block_Index];

          N1 = Block_Sizes[Block_Index][0];
          N2 = Block_Sizes[Block_Index][1];
          N3 = Block_Sizes[Block_Index][2];

          Number_of_Block_Nodes = N1 * N2 * N3;
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

          if (U_Scalar_Flags[U_Scalar_Index] != 0 && Block_Flag != 0)
          {
            U_Scalar = &U_Scalars[U_Scalar_Location_I];

            if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
                File_Format == UG_IO_BINARY_SINGLE)
            {
              TMP_float = fc0;

              for (Index = Block_Location+1;
                   Index <= Block_Location+Number_of_Block_Nodes; ++Index)
              {
                Read_Flag = Read_Flag + ug_fread (&TMP_float,
                                                  sizeof (float), 1, Func_File);

                U_Scalar[Index] = (double) TMP_float;
              }
            }
            else
              Read_Flag = Read_Flag + ug_fread (&U_Scalar[Block_Location+1],
                                                sizeof (DOUBLE_1D),
                                                Number_of_Block_Nodes,
                                                Func_File);

            U_Scalar_Location_I = U_Scalar_Location_I + *Number_of_Nodes + 1;
          }
          else
          {
            if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
                File_Format == UG_IO_BINARY_SINGLE)
            {
              for (Index = 1; Index <= Number_of_Block_Nodes; ++Index)
              {
                Read_Flag = Read_Flag + ug_fread (&TMP_float,
                                                  sizeof (float), 1, Func_File);
              }
            }
            else
            {
              for (Index = 1; Index <= Number_of_Block_Nodes; ++Index)
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

          Number_of_Read_Items = Number_of_Block_Nodes
                               + Special_Read_Items + Special_Read_Items;

          if (Read_Flag != Number_of_Read_Items)
          {
            strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
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

          if (U_Vector_Flags[U_Vector_Index] != 0 && Block_Flag != 0)
          {
            U_Vector = &U_Vectors[U_Vector_Location_I];

            if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
                File_Format == UG_IO_BINARY_SINGLE)
            {
              TMP_float_1 = fc0;
              TMP_float_2 = fc0;
              TMP_float_3 = fc0;

              for (Index = Block_Location+1;
                   Index <= Block_Location+Number_of_Block_Nodes; ++Index)
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
              Read_Flag = Read_Flag
                        + ug_fread (&U_Vector[Block_Location+1][0],
                                    sizeof (double),
                                    Number_of_Block_Nodes*3, Func_File);

            U_Vector_Location_I = U_Vector_Location_I + *Number_of_Nodes + 1;
          }
          else
          {
            if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
                File_Format == UG_IO_BINARY_SINGLE)
            {
              for (Index = 1; Index <= Number_of_Block_Nodes; ++Index)
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
              for (Index = 1; Index <= Number_of_Block_Nodes; ++Index)
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

          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE ||
              U_Vector_Flags[U_Vector_Index] == 0 || Block_Flag == 0)
            Number_of_Read_Items = 3 * Number_of_Block_Nodes
                                 + Special_Read_Items + Special_Read_Items;
          else
            Number_of_Read_Items = Number_of_Block_Nodes*3
                                 + Special_Read_Items + Special_Read_Items;


          if (Read_Flag != Number_of_Read_Items)
          {
            strcpy (Error_Message, "error reading UFUNC/VFUNC function file");
            return (1);
          }
        }

        if (Block_Flag != 0)
          Block_Location = Block_Location + Number_of_Block_Nodes;
      }
    }
  }

  return (0);

}
