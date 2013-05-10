#include "UG_IO_LIB.h"

INT_ ug_io_read_2d_ugrid
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Quads,
  INT_ *Number_of_Trias,
  INT_4D * Quad_Connectivity,
  INT_3D * Tria_Connectivity,
  DOUBLE_2D * Coordinates)

{

/*
 * Read grid data from a 2D UGRID planar grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_2d_ugrid.c,v 1.13 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Connectivity_1, Connectivity_2, Connectivity_3, Connectivity_4,
       Dummy_Index, Index, Number_of_Bytes, Number_of_Faces,
       Number_of_Read_Items, Number_of_Vol_Hexs, Number_of_Vol_Pents_5,
       Number_of_Vol_Pents_6, Number_of_Vol_Tets, Read_Flag, Special_Read_Items;

  int true_int = 0;

  float Coordinate_1_float, Coordinate_2_float, Coordinate_3_float, fc0;

  double Coordinate_1, Coordinate_2, Coordinate_3, dc0;

  fc0 = 0.0;
  dc0 = 0.0;

  if (File_Format == UG_IO_FORMATTED)
  {
    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Nodes = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Trias = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Quads = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    Number_of_Vol_Tets = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    Number_of_Vol_Pents_5 = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    Number_of_Vol_Pents_6 = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    Number_of_Vol_Hexs = (INT_) true_int;

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
      return (1);
    }
 
    if (Number_of_Vol_Tets != 0 ||
        Number_of_Vol_Pents_5 != 0 ||
        Number_of_Vol_Pents_6 != 0 ||
        Number_of_Vol_Hexs != 0)
    {
      strcpy (Error_Message, "2D UGRID grid file contains 3D data");
      return (1);
    }

    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%lf %lf %lf",
                          &Coordinate_1, &Coordinate_2, &Coordinate_3);

      if (Read_Task_Flag == 2)
      {
        Coordinates[Index][0] = Coordinate_1;
        Coordinates[Index][1] = Coordinate_2;
      }
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
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
      strcpy (Error_Message, "error reading 2D UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= *Number_of_Quads; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Connectivity_1 = (INT_) true_int;

      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Connectivity_2 = (INT_) true_int;

      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Connectivity_3 = (INT_) true_int;

      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Connectivity_4 = (INT_) true_int;

      if (Read_Task_Flag == 2)
      {
        Quad_Connectivity[Index][0] = Connectivity_1;
        Quad_Connectivity[Index][1] = Connectivity_2;
        Quad_Connectivity[Index][2] = Connectivity_3;
        Quad_Connectivity[Index][3] = Connectivity_4;
      }
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
      return (1);
    }

    Number_of_Faces = *Number_of_Trias + *Number_of_Quads;

    for (Index = 1; Index <= Number_of_Faces; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
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
    Read_Flag = Read_Flag + (INT_) ug_fread (Number_of_Quads,
                                             sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Vol_Tets,
                                             sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Vol_Pents_5,
                                             sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Vol_Pents_6,
                                             sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Vol_Hexs,
                                             sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = 7 + Special_Read_Items + Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
      return (1);
    }
 
    if (Number_of_Vol_Tets != 0 ||
        Number_of_Vol_Pents_5 != 0 ||
        Number_of_Vol_Pents_6 != 0 ||
        Number_of_Vol_Hexs != 0)
    {
      strcpy (Error_Message, "2D UGRID grid file contains 3D data");
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
      Coordinate_1_float = fc0;
      Coordinate_2_float = fc0;
      Coordinate_3_float = fc0;

      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&Coordinate_1_float,
                                                 sizeof (float), 1, Grid_File);
        Read_Flag = Read_Flag + (INT_) ug_fread (&Coordinate_2_float,
                                                 sizeof (float), 1, Grid_File);
        Read_Flag = Read_Flag + (INT_) ug_fread (&Coordinate_3_float,
                                                 sizeof (float), 1, Grid_File);

        if (Read_Task_Flag == 2)
        {
          Coordinates[Index][0] = (double) Coordinate_1_float;
          Coordinates[Index][1] = (double) Coordinate_2_float;
        }
      }
    }
    else
    {
      Coordinate_1 = dc0;
      Coordinate_2 = dc0;
      Coordinate_3 = dc0;

      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + (INT_) ug_fread (&Coordinate_1,
                                                 sizeof (double), 1, Grid_File);
        Read_Flag = Read_Flag + (INT_) ug_fread (&Coordinate_2,
                                                 sizeof (double), 1, Grid_File);
        Read_Flag = Read_Flag + (INT_) ug_fread (&Coordinate_3,
                                                 sizeof (double), 1, Grid_File);

        if (Read_Task_Flag == 2)
        {
          Coordinates[Index][0] = Coordinate_1;
          Coordinates[Index][1] = Coordinate_2;
        }
      }
    }

    Number_of_Read_Items = *Number_of_Nodes*3 + Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
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
      strcpy (Error_Message, "error reading 2D UGRID grid file");
      return (1);
    }

    Read_Flag = 0;

    for (Index = 1; Index <= *Number_of_Quads; ++Index)
    {
      Read_Flag = Read_Flag + (INT_) ug_fread (&Connectivity_1,
                                               sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + (INT_) ug_fread (&Connectivity_2,
                                               sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + (INT_) ug_fread (&Connectivity_3,
                                               sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + (INT_) ug_fread (&Connectivity_4,
                                               sizeof (INT_), 1, Grid_File);

      if (Read_Task_Flag == 2)
      {
        Quad_Connectivity[Index][0] = Connectivity_1;
        Quad_Connectivity[Index][1] = Connectivity_2;
        Quad_Connectivity[Index][2] = Connectivity_3;
        Quad_Connectivity[Index][3] = Connectivity_4;
      }
    }

    if (Read_Flag != *Number_of_Quads*4)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
      return (1);
    }

    Number_of_Faces = *Number_of_Trias + *Number_of_Quads;

    Read_Flag = 0;

    for (Index = 1; Index <= Number_of_Faces; ++Index)
    {
      Read_Flag = Read_Flag + (INT_) ug_fread (&Dummy_Index,
                                               sizeof (INT_), 1, Grid_File);
    }

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = Number_of_Faces + Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading 2D UGRID grid file");
      return (1);
    }
  }

  return (0);

}
