#include "UG_IO_LIB.h"

INT_ ug_io_read_2d_fgrid
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Trias,
  INT_3D * Tria_Connectivity,
  DOUBLE_2D * Coordinates)

{

/*
 * Read grid data from a 2D FGRID planar grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_2d_fgrid.c,v 1.12 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Connectivity_1, Connectivity_2, Connectivity_3, Dummy_Index, Index,
       Number_of_Bytes, Number_of_Read_Items, Number_of_Vol_Tets, Read_Flag,
       Special_Read_Items;

  int true_int = 0;

  float fc0, TMP_float;

  double dc0, TMP_double;

  fc0 = 0.0;
  dc0 = 0.0;

  if (File_Format == UG_IO_FORMATTED)
  {
    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Nodes = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Trias = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    Number_of_Vol_Tets = (INT_) true_int;

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }
 
    if (Number_of_Vol_Tets != 0)
    {
      strcpy (Error_Message, "2D FGRID grid file contains 3D data");
      return (1);
    }

    TMP_double = dc0;

    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%lf", &TMP_double);

      if (Read_Task_Flag == 2)
        Coordinates[Index][0] = TMP_double;
    }

    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%lf", &TMP_double);

      if (Read_Task_Flag == 2)
        Coordinates[Index][1] = TMP_double;
    }

    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%lf", &TMP_double);
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= *Number_of_Trias; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Connectivity_1 = (INT_) true_int;

      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Connectivity_2 = (INT_) true_int;

      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Connectivity_3 = (INT_) true_int;

      if (Read_Task_Flag == 2)
      {
        Tria_Connectivity[Index][0] = Connectivity_1;
        Tria_Connectivity[Index][1] = Connectivity_2;
        Tria_Connectivity[Index][2] = Connectivity_3;
      }
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= *Number_of_Trias; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }
  }
  else
  {
    Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                           File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Read_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

    Read_Flag = Read_Flag + (INT_) ug_fread (Number_of_Nodes,
                                             sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + (INT_) ug_fread (Number_of_Trias,
                                             sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Vol_Tets,
                                             sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = 3 + Special_Read_Items + Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }
 
    if (Number_of_Vol_Tets != 0)
    {
      strcpy (Error_Message, "2D FGRID grid file contains 3D data");
      return (1);
    }

    Read_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
        File_Format == UG_IO_BINARY_SINGLE)
    {
      TMP_float = fc0;

      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&TMP_float,
                                                 sizeof (float), 1, Grid_File);

        if (Read_Task_Flag == 2)
          Coordinates[Index][0] = (double)TMP_float;
      }

      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&TMP_float,
                                                 sizeof (float), 1, Grid_File);
 
        if (Read_Task_Flag == 2)
          Coordinates[Index][1] = (double)TMP_float;
      }

      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&TMP_float,
                                                 sizeof (float), 1, Grid_File);
      }
    }
    else
    {
      TMP_double = dc0;

      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&TMP_double,
                                                 sizeof (double), 1, Grid_File);
 
        if (Read_Task_Flag == 2)
          Coordinates[Index][0] = TMP_double;
      }
 
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&TMP_double,
                                                 sizeof (double), 1, Grid_File);
 
        if (Read_Task_Flag == 2)
          Coordinates[Index][1] = TMP_double;
      }
 
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&TMP_double,
                                                 sizeof (double), 1, Grid_File);
      }
    }

    Number_of_Read_Items = *Number_of_Nodes*3 + Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items )
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }

    Read_Flag = 0;
 
    for (Index = 1; Index <= *Number_of_Trias; ++Index)
    {
      Read_Flag = Read_Flag + (INT_) ug_fread (&Connectivity_1,
                                               sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + (INT_) ug_fread (&Connectivity_2,
                                               sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + (INT_) ug_fread (&Connectivity_3,
                                               sizeof (INT_), 1, Grid_File);

      if (Read_Task_Flag == 2)
      {
        Tria_Connectivity[Index][0] = Connectivity_1;
        Tria_Connectivity[Index][1] = Connectivity_2;
        Tria_Connectivity[Index][2] = Connectivity_3;
      }
    }

    if (Read_Flag != *Number_of_Trias*3)
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }

    Read_Flag = 0;
 
    for (Index = 1; Index <= *Number_of_Trias; ++Index)
    {
      Read_Flag = Read_Flag + (INT_) ug_fread (&Dummy_Index,
                                               sizeof (INT_), 1, Grid_File);
    }

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = *Number_of_Trias + Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading 2D FGRID grid file");
      return (1);
    }
  }

  return (0);

}
