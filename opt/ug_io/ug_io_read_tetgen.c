#include "UG_IO_LIB.h"

INT_ ug_io_read_tetgen
 (char Case_Name[],
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Read grid data from TetGen volume grid files.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_tetgen.c,v 1.7 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Grid_File;

  CHAR_UG_MAX Grid_File_Name;

  INT_ Index, File_Index, Read_Flag, Surf_ID;

  strcpy (Grid_File_Name, "");
  strcat (Grid_File_Name, Case_Name);
  strcat (Grid_File_Name, ".node");

  Grid_File = ug_fopen (Grid_File_Name, "r");
  
  if (Grid_File == NULL)
  {
    strcpy (Error_Message, "unable to open TETGEN .node grid file");
    return (1);
  }

  Read_Flag = fscanf (Grid_File, "%i", Number_of_Nodes);
  Read_Flag = fscanf (Grid_File, "%i", &Index);
  Read_Flag = fscanf (Grid_File, "%i", &Index);
  Read_Flag = fscanf (Grid_File, "%i", &Index);

  if (Read_Task_Flag == 2)
  {
    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%i %lg %lg %lg",
                          &File_Index,
                          &Coordinates[Index][0],
                          &Coordinates[Index][1],
                          &Coordinates[Index][2]);
    }
  }

  ug_fclose (Grid_File);

  if (Read_Flag == EOF)
  {
    strcpy (Error_Message, "error reading TETGEN .node grid file");
    return (1);
  }

  strcpy (Grid_File_Name, "");
  strcat (Grid_File_Name, Case_Name);
  strcat (Grid_File_Name, ".ele");

  Grid_File = ug_fopen (Grid_File_Name, "r");
  
  if (Grid_File != NULL)
  {
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Tets);
    Read_Flag = fscanf (Grid_File, "%i", &Index);
    Read_Flag = fscanf (Grid_File, "%i", &Index);

    if (Read_Task_Flag == 2)
    {
      for (Index = 1; Index <= *Number_of_Vol_Tets; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i %i %i %i %i",
                            &File_Index,
                            &Vol_Tet_Connectivity[Index][0],
                            &Vol_Tet_Connectivity[Index][1],
                            &Vol_Tet_Connectivity[Index][3],
                            &Vol_Tet_Connectivity[Index][2]);
      }
    }

    ug_fclose (Grid_File);

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading TETGEN .ele grid file");
      return (1);
    }
  }
  else
    *Number_of_Vol_Tets = 0;

  strcpy (Grid_File_Name, "");
  strcat (Grid_File_Name, Case_Name);
  strcat (Grid_File_Name, ".face");

  Grid_File = ug_fopen (Grid_File_Name, "r");
  
  if (Grid_File == NULL)
  {
    strcpy (Error_Message, "unable to open TETGEN .face grid file");
    return (1);
  }

  Read_Flag = fscanf (Grid_File, "%i", Number_of_Surf_Trias);
  Read_Flag = fscanf (Grid_File, "%i", &Index);

  if (Read_Task_Flag == 2)
  {
    for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%i %i %i %i %i",
                          &File_Index,
                          &Surf_Tria_Connectivity[Index][0],
                          &Surf_Tria_Connectivity[Index][1],
                          &Surf_Tria_Connectivity[Index][2],
                          &Surf_ID);

      if (Surf_ID_Flag != NULL)
        Surf_ID_Flag[Index] = Surf_ID;
    }
  }

  ug_fclose (Grid_File);

  if (Read_Flag == EOF)
  {
    strcpy (Error_Message, "error reading TETGEN .face grid file");
    return (1);
  }

  return (0);

}
