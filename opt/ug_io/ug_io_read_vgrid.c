#include "UG_IO_LIB.h"

INT_ ug_io_read_vgrid
 (FILE * Grid_File,
  char Case_Name[],
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_Bnd_Nodes,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Tets,
  INT_ *Number_of_BL_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Read grid data from an VGRID volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_vgrid.c,v 1.5 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *BC_File = NULL;

  CHAR_UG_MAX BC_File_Name, Text_Line;

  char *Read_Label;

  int true_int = 0;

  INT_ Geom_Index, Index, Number_of_Bytes, Number_of_Read_Items,
       Number_of_Total_Nodes, Number_of_Total_Tets, Read_Flag, TMP_int;

  double TMP_double;

  Read_Flag = 0;

  Read_Flag = Read_Flag
            + ug_fread (&Number_of_Bytes, sizeof (INT_), 1, Grid_File);

  Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                    sizeof (INT_), 1, Grid_File);
  Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Tets,
                                    sizeof (INT_), 1, Grid_File);
  Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                    sizeof (INT_), 1, Grid_File);
  Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                    sizeof (INT_), 1, Grid_File);

  if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
    *Number_of_Bnd_Nodes = TMP_int;

  Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                    sizeof (INT_), 1, Grid_File);
  Read_Flag = Read_Flag + ug_fread (Number_of_BL_Vol_Tets,
                                    sizeof (INT_), 1, Grid_File);
  Read_Flag = Read_Flag + ug_fread (&TMP_double,
                                    sizeof (double), 1, Grid_File);

  Number_of_Total_Nodes = *Number_of_Nodes;
  Number_of_Total_Tets = *Number_of_Vol_Tets;

  if (*Number_of_Bnd_Nodes > 0)
  {
    *Number_of_Nodes = *Number_of_Bnd_Nodes;
    *Number_of_Vol_Tets = 0;
  }

  Number_of_Read_Items = 8;

  if (Read_Flag != Number_of_Read_Items)
  {
    strcpy (Error_Message, "error reading VGRID grid file");
    return (1);
  }

  if (Read_Task_Flag == 2)
  {
    Read_Flag = 0;

    if (*Number_of_Bnd_Nodes == 0)
    {
      for (Geom_Index = 0; Geom_Index <= 3; ++Geom_Index)
      {
        for (Index = 1; Index <= Number_of_Total_Tets; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&Vol_Tet_Connectivity[Index][Geom_Index],
                                            sizeof (INT_), 1, Grid_File);
        }
      }
    }
    else
    {
      for (Geom_Index = 0; Geom_Index <= 3; ++Geom_Index)
      {
        for (Index = 1; Index <= Number_of_Total_Tets; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }
    }

    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                      sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = 4*Number_of_Total_Tets + 1;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading VGRID grid file");
      return (1);
    }

    Read_Flag = 0;

    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                      sizeof (INT_), 1, Grid_File);

    for (Geom_Index = 0; Geom_Index <= 2; ++Geom_Index)
    {
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + ug_fread (&Coordinates[Index][Geom_Index],
                                          sizeof (double), 1, Grid_File);
      }

      for (Index = *Number_of_Nodes+1; Index <= Number_of_Total_Nodes; ++Index)
      {
        Read_Flag = Read_Flag + ug_fread (&TMP_double,
                                          sizeof (double), 1, Grid_File);
      }
    }

    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                      sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = 3*Number_of_Total_Nodes + 2;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading VGRID grid file");
      return (1);
    }
  }

  // read the .bc file (tria connectivity)

  sprintf (BC_File_Name,"%s.bc",Case_Name);

  BC_File = ug_fopen (BC_File_Name, "r");

  if (BC_File == NULL)
  {
    strcpy (Error_Message, "unable to open VGRID bc file");
    return (1);
  }

  Read_Flag = fscanf (BC_File, "%i", &true_int);

  if (Read_Task_Flag == 1)
    *Number_of_Surf_Trias = (INT_) true_int;

  Read_Flag = fscanf (BC_File, "%i", &true_int);

  Read_Flag = fscanf (BC_File, "%i", &true_int);

  Read_Flag = fscanf (BC_File, "%i", &true_int);

  Read_Label = fgets (Text_Line, (int) UG_MAX_CHAR_STRING_LENGTH, BC_File);
  Read_Label = fgets (Text_Line, (int) UG_MAX_CHAR_STRING_LENGTH, BC_File);

  if (Read_Flag == EOF)
  {
    strcpy (Error_Message, "error reading VGRID bc file");
    return (1);
  }

  if (Read_Task_Flag == 2)
  {
    for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
    {
      Read_Flag = fscanf (BC_File, "%i", &true_int);

      Read_Flag = fscanf (BC_File, "%i", &true_int);

      if (Surf_ID_Flag != NULL) Surf_ID_Flag[Index] = (INT_) true_int;

      Read_Flag = fscanf (BC_File, "%i", &true_int);

      Surf_Tria_Connectivity[Index][0] = (INT_) true_int;

      Read_Flag = fscanf (BC_File, "%i", &true_int);

      Surf_Tria_Connectivity[Index][1] = (INT_) true_int;

      Read_Flag = fscanf (BC_File, "%i", &true_int);

      Surf_Tria_Connectivity[Index][2] = (INT_) true_int;
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading VGRID bc file");
      return (1);
    }
  }

  ug_fclose (BC_File);

  return (0);

}
