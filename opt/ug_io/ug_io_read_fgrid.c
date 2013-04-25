#include "UG_IO_LIB.h"

INT_ ug_io_read_fgrid
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Bnd_Nodes,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Read grid data from an FGRID or UFAST volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_fgrid.c,v 1.30 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  int true_int = 0;

  INT_ Tria_Node_Index[3];
  INT_ Dim, Index, Number_of_Bytes, Number_of_Read_Items, Number_of_Total_Nodes,
       Read_Flag, Special_Read_Items, TMP_INT_;

  float fc0, TMP_float;

  double TMP_double;

  fc0 = 0.0;

  if (File_Format == UG_IO_FORMATTED)
  {
    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Nodes = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Surf_Trias = (INT_) true_int;

    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    *Number_of_Vol_Tets = (INT_) true_int;

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading FGRID or UFAST grid file");
      return (1);
    }

    if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
    {
      for (Dim = 0; Dim <= 2; ++Dim)
      {
        for (Index = 1; Index <= *Number_of_Nodes; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf", &TMP_double);
        }
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      *Number_of_Bnd_Nodes = 0;

      for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, (INT_) true_int);

        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, (INT_) true_int);

        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, (INT_) true_int);
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }
    }

    Number_of_Total_Nodes = *Number_of_Nodes;

    if (*Number_of_Bnd_Nodes > 0)
      *Number_of_Nodes = *Number_of_Bnd_Nodes;

    if (Read_Task_Flag == 2)
    {
      for (Dim = 0; Dim <= 2; ++Dim)
      {
        for (Index = 1; Index <= *Number_of_Nodes; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf", &Coordinates[Index][Dim]);
        }

        for (Index = *Number_of_Nodes+1;
             Index <= Number_of_Total_Nodes; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf", &TMP_double);
        }
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        Surf_Tria_Connectivity[Index][0] = (INT_) true_int;

        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        Surf_Tria_Connectivity[Index][1] = (INT_) true_int;

        Read_Flag = fscanf (Grid_File, "%i", &true_int);

        Surf_Tria_Connectivity[Index][2] = (INT_) true_int;
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      if (Surf_ID_Flag != NULL)
      {
        for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%i", &true_int);

          Surf_ID_Flag[Index] = (INT_) true_int;
        }
      }
      else
      {
        for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%i", &true_int);
        }
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      if (*Number_of_Bnd_Nodes == 0)
      {
        for (Index = 1; Index <= *Number_of_Vol_Tets; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%i", &true_int);

          Vol_Tet_Connectivity[Index][0] = (INT_) true_int;

          Read_Flag = fscanf (Grid_File, "%i", &true_int);

          Vol_Tet_Connectivity[Index][1] = (INT_) true_int;

          Read_Flag = fscanf (Grid_File, "%i", &true_int);

          Vol_Tet_Connectivity[Index][2] = (INT_) true_int;

          Read_Flag = fscanf (Grid_File, "%i", &true_int);

          Vol_Tet_Connectivity[Index][3] = (INT_) true_int;
        }
      }
      else
      {
        for (Index = 1; Index <= *Number_of_Vol_Tets; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%i", &true_int);
          Read_Flag = fscanf (Grid_File, "%i", &true_int);
          Read_Flag = fscanf (Grid_File, "%i", &true_int);
          Read_Flag = fscanf (Grid_File, "%i", &true_int);
        }
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }
    }
  }
  else
  {
    Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                           File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Read_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag
                + ug_fread (&Number_of_Bytes, sizeof (INT_), 1, Grid_File);

    Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                      sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + ug_fread (Number_of_Surf_Trias,
                                      sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Tets,
                                      sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                        sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = 3 + Special_Read_Items + Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading FGRID or UFAST grid file");
      return (1);
    }

    if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
          File_Format == UG_IO_BINARY_SINGLE)
      {
        for (Dim = 0; Dim <= 2; ++Dim)
        {
          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = Read_Flag
                      + ug_fread (&TMP_float, sizeof (float), 1, Grid_File);
          }
        }
      }
      else
      {
        for (Dim = 0; Dim <= 2; ++Dim)
        {
          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = Read_Flag
                      + ug_fread (&TMP_double, sizeof (double), 1, Grid_File);
          }
        }
      }

      Number_of_Read_Items = *Number_of_Nodes*3 + Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items )
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      *Number_of_Bnd_Nodes = 0;

      Read_Flag = 0;

      for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
      {
        Read_Flag = Read_Flag + ug_fread (&Tria_Node_Index,
                                          sizeof (INT_), 3, Grid_File);

        *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, Tria_Node_Index[0]);
        *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, Tria_Node_Index[1]);
        *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, Tria_Node_Index[2]);
      }

      if (Read_Flag != *Number_of_Surf_Trias*3)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }
    }

    Number_of_Total_Nodes = *Number_of_Nodes;

    if (*Number_of_Bnd_Nodes > 0)
      *Number_of_Nodes = *Number_of_Bnd_Nodes;

    if (Read_Task_Flag == 2)
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
          File_Format == UG_IO_BINARY_SINGLE)
      {
        TMP_float = fc0;

        for (Dim = 0; Dim <= 2; ++Dim)
        {
          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = Read_Flag
                      + ug_fread (&TMP_float, sizeof (float), 1, Grid_File);

            Coordinates[Index][Dim] = (double) TMP_float;
          }

          for (Index = *Number_of_Nodes+1;
               Index <= Number_of_Total_Nodes; ++Index)
          {
            Read_Flag = Read_Flag
                      + ug_fread (&TMP_float, sizeof (float), 1, Grid_File);
          }
        }
      }
      else
      {
        for (Dim = 0; Dim <= 2; ++Dim)
        {
          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = Read_Flag + ug_fread (&Coordinates[Index][Dim],
                                              sizeof (double), 1, Grid_File);
          }

          for (Index = *Number_of_Nodes+1;
               Index <= Number_of_Total_Nodes; ++Index)
          {
            Read_Flag = Read_Flag
                      + ug_fread (&TMP_double, sizeof (double), 1, Grid_File);
          }
        }
      }

      Number_of_Read_Items = Number_of_Total_Nodes*3 + Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      Read_Flag = ug_fread (&Surf_Tria_Connectivity[1][0],
                            sizeof (INT_), *Number_of_Surf_Trias*3, Grid_File);

      if (Read_Flag != *Number_of_Surf_Trias*3)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      if (Surf_ID_Flag != NULL)
        Read_Flag = ug_fread (&Surf_ID_Flag[1], sizeof (INT_1D),
                              *Number_of_Surf_Trias, Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
        {
          Read_Flag = Read_Flag
                    + ug_fread (&TMP_INT_, sizeof (INT_), 1, Grid_File);
        }
      }

      if (Read_Flag != *Number_of_Surf_Trias)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }

      if (*Number_of_Bnd_Nodes == 0)
        Read_Flag = ug_fread (&Vol_Tet_Connectivity[1][0], sizeof (INT_),
                              *Number_of_Vol_Tets*4, Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= *Number_of_Vol_Tets*4; ++Index)
        {
          Read_Flag = Read_Flag
                    + ug_fread (&TMP_INT_, sizeof (INT_), 1, Grid_File);
        }
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);
 
      Number_of_Read_Items = *Number_of_Vol_Tets*4 + Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items)
      {
        strcpy (Error_Message, "error reading FGRID or UFAST grid file");
        return (1);
      }
    }
  }

  return (0);

}
