#include "UG3_LIB.h"

INT_ ug3_read_surf_grid_file
 (char File_Name[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_1D * Surf_Reconnection_Flag,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Read grid data from a SURF surface grid file.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_read_surf_grid_file.c,v 1.13 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum 
 */

  CHAR_UG_MAX drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;

  FILE *Grid_File;

  INT_ Node_Index, Read_Flag, Surf_Grid_BC_Flag_, Surf_ID_Flag_, Surf_Index,
       Surf_Quad_Index, Surf_Reconnection_Flag_, Surf_Tria_Index, Try;

  double BL_Thickness_, Initial_Normal_Spacing_;

  if (Read_Task_Flag == 1)
    strcat (File_Name, ".surf");

  if (ug_file_status_monitor_flag())
  {
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "ug3_read_surf_grid_file  : reading SURF file with File_Name=%s Read_Task_Flag=%i", Name_Text, Read_Task_Flag);
    ug_message (Text);
  }

  Grid_File = ug_fopen (File_Name, "r");

  if (Grid_File == NULL)
    return (337);

  Read_Flag = fscanf (Grid_File, "%i %i %i", Number_of_Surf_Trias,
                                             Number_of_Surf_Quads,
                                             Number_of_Nodes);

  if (Read_Flag == EOF)
  {
    ug_fclose (Grid_File);
    return (337);
  }

  if (Read_Task_Flag == 2)
  {
    Try = 1;

    do
    {
      if (Try == 1)
      {
        for (Node_Index = 1; Node_Index <= *Number_of_Nodes; ++Node_Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf %lf %lf %lf %lf",
                              &Coordinates[Node_Index][0],
                              &Coordinates[Node_Index][1],
                              &Coordinates[Node_Index][2],
                              &Initial_Normal_Spacing_,
                              &BL_Thickness_);
        }
      }

      else if (Try == 2)
      {
        ug_rewind (Grid_File);

        Read_Flag = fscanf (Grid_File, "%i %i %i", Number_of_Surf_Trias,
                                                   Number_of_Surf_Quads,
                                                   Number_of_Nodes);

        if (Read_Flag == EOF)
        {
          ug_fclose (Grid_File);
          return (337);
        }

        for (Node_Index = 1; Node_Index <= *Number_of_Nodes; ++Node_Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf %lf %lf %lf",
                              &Coordinates[Node_Index][0],
                              &Coordinates[Node_Index][1],
                              &Coordinates[Node_Index][2],
                              &Initial_Normal_Spacing_);
        }
      }

      else
      {
        ug_rewind (Grid_File);

        Read_Flag = fscanf (Grid_File, "%i %i %i", Number_of_Surf_Trias,
                                                   Number_of_Surf_Quads,
                                                   Number_of_Nodes);

        if (Read_Flag == EOF)
        {
          ug_fclose (Grid_File);
          return (337);
        }

        for (Node_Index = 1; Node_Index <= *Number_of_Nodes; ++Node_Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf %lf %lf",
                              &Coordinates[Node_Index][0],
                              &Coordinates[Node_Index][1],
                              &Coordinates[Node_Index][2]);
        }
      }

      Surf_Index = 0;

      for (Surf_Tria_Index = 1;
           Surf_Tria_Index <= *Number_of_Surf_Trias; ++Surf_Tria_Index)
      {
        ++Surf_Index;

        Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i",
                            &Surf_Tria_Connectivity[Surf_Tria_Index][0],
                            &Surf_Tria_Connectivity[Surf_Tria_Index][1],
                            &Surf_Tria_Connectivity[Surf_Tria_Index][2],
                            &Surf_ID_Flag_,
                            &Surf_Reconnection_Flag_,
                            &Surf_Grid_BC_Flag_);

        if (Surf_ID_Flag != NULL)
           Surf_ID_Flag[Surf_Index] = Surf_ID_Flag_;

        if (Surf_Reconnection_Flag != NULL)
          Surf_Reconnection_Flag[Surf_Index] = Surf_Reconnection_Flag_;

        if (Surf_Grid_BC_Flag != NULL)
          Surf_Grid_BC_Flag[Surf_Index] = Surf_Grid_BC_Flag_;
      }

      for (Surf_Quad_Index = 1;
           Surf_Quad_Index <= *Number_of_Surf_Quads; ++Surf_Quad_Index)
      {
        ++Surf_Index;

        Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i %i",
                            &Surf_Quad_Connectivity[Surf_Quad_Index][0],
                            &Surf_Quad_Connectivity[Surf_Quad_Index][1],
                            &Surf_Quad_Connectivity[Surf_Quad_Index][2],
                            &Surf_Quad_Connectivity[Surf_Quad_Index][3],
                            &Surf_ID_Flag_,
                            &Surf_Reconnection_Flag_,
                            &Surf_Grid_BC_Flag_);

        if (Surf_ID_Flag != NULL)
          Surf_ID_Flag[Surf_Index] = Surf_ID_Flag_;

        if (Surf_Reconnection_Flag != NULL)
          Surf_Reconnection_Flag[Surf_Index] = Surf_Reconnection_Flag_;

        if (Surf_Grid_BC_Flag != NULL)
          Surf_Grid_BC_Flag[Surf_Index] = Surf_Grid_BC_Flag_;
      }

      ++Try;
    }
    while (Try <= 3 && Read_Flag == EOF);

    if (Read_Flag == EOF)
    {
      ug_fclose (Grid_File);
      return (337);
    }

    if (Surf_Reconnection_Flag != NULL)
    {
      for (Surf_Index = 1;
           Surf_Index <= *Number_of_Surf_Trias+*Number_of_Surf_Quads;
           ++Surf_Index)
      {
        Surf_Reconnection_Flag_ = Surf_Reconnection_Flag[Surf_Index];
        Surf_Reconnection_Flag_ = MAX (Surf_Reconnection_Flag_, 0);
        Surf_Reconnection_Flag_ = MIN (Surf_Reconnection_Flag_, 7);

        Surf_Reconnection_Flag[Surf_Index] = Surf_Reconnection_Flag_;
      }
    }
  }

  ug_fclose (Grid_File);

  return (0);

}
