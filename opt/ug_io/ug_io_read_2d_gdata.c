#include "UG_IO_LIB.h"

INT_ ug_io_read_2d_gdata
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Bnd_Edges,
  INT_ Number_of_Nodes,
  INT_ Number_of_Quads,
  INT_ Number_of_Trias,
  INT_2D * Bnd_Edge_Connectivity,
  INT_1D * Bnd_Edge_Grid_BC_Flag,
  INT_1D * Bnd_Edge_ID_Flag,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Read boundary edge grid data from a 2D planar grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_2d_gdata.c,v 1.2 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Dummy_Index, Index, Number_of_Bytes, Number_of_Read_Items,
       Number_of_Faces, Read_BL_Thickness_Flag, Read_Flag, Read_Grid_BC_Flag,
       Read_Initial_Normal_Spacing_Flag, Special_Read_Items;

  int true_int = 0;

  float fc0, TMP_float;

  double dc0;

  Read_BL_Thickness_Flag = 0;
  Read_Grid_BC_Flag = 0;
  Read_Initial_Normal_Spacing_Flag = 0;

  Number_of_Faces = Number_of_Quads + Number_of_Trias;

  fc0 = 0.0;
  dc0 = 0.0;

  if (File_Format == UG_IO_FORMATTED)
  {
    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D grid file boundary data");
      return (1);
    }

    *Number_of_Bnd_Edges = (INT_) true_int;
  }
  else
  {
    Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                           File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Read_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag
                + (INT_) ug_fread (&Number_of_Bytes, sizeof (INT_), 1,
                                   Grid_File);

    Read_Flag = Read_Flag
              + (INT_) ug_fread (Number_of_Bnd_Edges, sizeof (INT_), 1,
                                 Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag
                + (INT_) ug_fread (&Number_of_Bytes, sizeof (INT_), 1,
                                   Grid_File);
 
    Number_of_Read_Items = 1 + Special_Read_Items + Special_Read_Items;
 
    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading 2D grid file boundary data");
      return (1);
    }
  }

  if (Read_Task_Flag == 2)
  {
    if (File_Format == UG_IO_FORMATTED)
    {
      for (Index = 1; Index <= *Number_of_Bnd_Edges; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        Bnd_Edge_Connectivity[Index][0] = (INT_) true_int;

        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        Bnd_Edge_Connectivity[Index][1] = (INT_) true_int;

        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        if (Bnd_Edge_ID_Flag != NULL)
          Bnd_Edge_ID_Flag[Index] = (INT_) true_int;
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading 2D grid file boundary data");
        return (1);
      }

      if (Number_of_Faces == 0 && Bnd_Edge_Grid_BC_Flag != NULL)
      {
        for (Index = 1; Index <= *Number_of_Bnd_Edges; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%i", &true_int);

          Bnd_Edge_Grid_BC_Flag[Index] = (INT_) true_int;
        }

        if (Read_Flag != EOF)
          Read_Grid_BC_Flag = 1;

        if (Read_Grid_BC_Flag == 1 && Initial_Normal_Spacing != NULL)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%lf",
                                &Initial_Normal_Spacing[Index]);
          }

          if (Read_Flag != EOF)
            Read_Initial_Normal_Spacing_Flag = 1;
        }

        if (Read_Initial_Normal_Spacing_Flag == 1 && BL_Thickness != NULL)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%lf",
                                &BL_Thickness[Index]);
          }

          if (Read_Flag != EOF)
            Read_BL_Thickness_Flag = 1;
        }
      }
    }
    else
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag
                  + (INT_) ug_fread (&Number_of_Bytes,
                                     sizeof (INT_), 1, Grid_File);
 
      for (Index = 1; Index <= *Number_of_Bnd_Edges; ++Index)
      { 
        Read_Flag = Read_Flag
                  + (INT_) ug_fread (&Bnd_Edge_Connectivity[Index][0],
                                     sizeof (INT_), 2, Grid_File);
        Read_Flag = Read_Flag
                  + (INT_) ug_fread (&Dummy_Index,
                                     sizeof (INT_), 1, Grid_File);

        if (Bnd_Edge_ID_Flag != NULL)
          Bnd_Edge_ID_Flag[Index] = Dummy_Index;
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                                 sizeof (INT_), 1, Grid_File);
 
      Number_of_Read_Items = *Number_of_Bnd_Edges*3
                           + Special_Read_Items + Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items)
      {
        strcpy (Error_Message, "error reading 2D grid file boundary data");
        return (1);
      }

      if (Number_of_Faces == 0 && Bnd_Edge_Grid_BC_Flag != NULL)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                                   sizeof (INT_), 1,
                                                   Grid_File);
 
        Read_Flag = Read_Flag + (INT_) ug_fread (&Bnd_Edge_Grid_BC_Flag[1],
                                                 sizeof (INT_1D),
                                                 *Number_of_Bnd_Edges,
                                                 Grid_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                                   sizeof (INT_), 1,
                                                   Grid_File);
 
        Number_of_Read_Items = *Number_of_Bnd_Edges
                             + Special_Read_Items + Special_Read_Items;

        if (Read_Flag == Number_of_Read_Items)
          Read_Grid_BC_Flag = 1;

        if (Read_Grid_BC_Flag == 1 && Initial_Normal_Spacing != NULL)
        {
          Read_Flag = 0;

          if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
              File_Format == UG_IO_UNFORMATTED_SINGLE)
            Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                                     sizeof (INT_), 1,
                                                     Grid_File);
 
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            TMP_float = fc0;

            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag
                        + (INT_) ug_fread (&TMP_float,
                                           sizeof (float), 1, Grid_File);
 
              Initial_Normal_Spacing[Index] = (double)TMP_float;
            }
          }
          else
            Read_Flag = Read_Flag + (INT_) ug_fread (&Initial_Normal_Spacing[1],
                                                     sizeof (DOUBLE_1D),
                                                     Number_of_Nodes,
                                                     Grid_File);

          if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
              File_Format == UG_IO_UNFORMATTED_SINGLE)
            Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                                     sizeof (INT_), 1,
                                                     Grid_File);
 
          Number_of_Read_Items = Number_of_Nodes
                               + Special_Read_Items + Special_Read_Items;

          if (Read_Flag == Number_of_Read_Items)
            Read_Initial_Normal_Spacing_Flag = 1;
        }

        if (Read_Initial_Normal_Spacing_Flag == 1 && BL_Thickness != NULL)
        {
          Read_Flag = 0;

          if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
              File_Format == UG_IO_UNFORMATTED_SINGLE)
            Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                                     sizeof (INT_), 1,
                                                     Grid_File);
 
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            TMP_float = fc0;

            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              Read_Flag = Read_Flag + (INT_) ug_fread (&TMP_float,
                                                       sizeof (float), 1,
                                                       Grid_File);
 
              BL_Thickness[Index] = (double)TMP_float;
            }
          }
          else
            Read_Flag = Read_Flag + (INT_) ug_fread (&BL_Thickness[1],
                                                     sizeof (DOUBLE_1D),
                                                     Number_of_Nodes,
                                                     Grid_File);

          if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
              File_Format == UG_IO_UNFORMATTED_SINGLE)
            Read_Flag = Read_Flag + (INT_) ug_fread (&Number_of_Bytes,
                                                     sizeof (INT_), 1,
                                                     Grid_File);
 
          Number_of_Read_Items = Number_of_Nodes
                               + Special_Read_Items + Special_Read_Items;

          if (Read_Flag == Number_of_Read_Items)
            Read_BL_Thickness_Flag = 1;
        }
      }
    }

    if (Read_Grid_BC_Flag == 0 && Bnd_Edge_Grid_BC_Flag != NULL)
    {
      for (Index = 1; Index <= *Number_of_Bnd_Edges; ++Index)
      {
        Bnd_Edge_Grid_BC_Flag[Index] = -1;
      }
    }

    if (Read_Initial_Normal_Spacing_Flag == 0 && Initial_Normal_Spacing != NULL)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        Initial_Normal_Spacing[Index] = dc0;
      }
    }

    if (Read_BL_Thickness_Flag == 0 && BL_Thickness != NULL)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        BL_Thickness[Index] = dc0;
      }
    }
  }

  return (0);

}
