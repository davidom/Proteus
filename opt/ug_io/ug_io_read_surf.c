#include "UG_IO_LIB.h"

INT_ ug_io_read_surf
 (FILE * Grid_File,
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_1D * Surf_Reconnection_Flag,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Read grid data from a SURF surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_surf.c,v 1.21 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  

  CHAR_UG_MAX Text_Line;

  char *Read_Label;

  INT_ Mode, Node_Index, Read_Flag, Surf_Grid_BC_Flag_, Surf_ID_Flag_,
       Surf_Index, Surf_Quad_Index, Surf_Reconnection_Flag_, Surf_Tria_Index;

  double BL_Thickness_, dc0, Initial_Normal_Spacing_;

  dc0 = 0.0;

  Read_Flag = fscanf (Grid_File, "%i %i %i", Number_of_Surf_Trias,
                                             Number_of_Surf_Quads,
                                             Number_of_Nodes);

  if (Read_Flag == EOF)
  {
    strcpy (Error_Message, "error reading SURF surface grid file");
    return (1);
  }

  if (Read_Task_Flag == 2)
  {
    Read_Label = fgets (Text_Line, UG_MAX_CHAR_STRING_LENGTH, Grid_File);
    Read_Label = fgets (Text_Line, UG_MAX_CHAR_STRING_LENGTH, Grid_File);

    if (Read_Label == NULL)
    {
      strcpy (Error_Message, "error reading SURF surface grid file");
      return (1);
    }

    if (*Number_of_Nodes > 0)
    {
      Node_Index = 1;

      Read_Flag = sscanf (Text_Line, "%lf %lf %lf %lf %lf",
                          &Coordinates[Node_Index][0],
                          &Coordinates[Node_Index][1],
                          &Coordinates[Node_Index][2],
                          &Initial_Normal_Spacing_,
                          &BL_Thickness_);

      Mode = Read_Flag;

      if (Mode < 3 || Mode > 5)
      {
        strcpy (Error_Message, "error reading SURF surface grid file");
        return (1);
      }

      if (Mode == 3)
        Initial_Normal_Spacing_ = dc0;

      if (Mode <= 4)
        BL_Thickness_ = dc0;

      if (Initial_Normal_Spacing != NULL)
        Initial_Normal_Spacing[Node_Index] = Initial_Normal_Spacing_;

      if (BL_Thickness != NULL)
        BL_Thickness[Node_Index] = BL_Thickness_;

      if (Mode == 5)
      {
        for (Node_Index = 2; Node_Index <= *Number_of_Nodes; ++Node_Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf %lf %lf %lf %lf",
                              &Coordinates[Node_Index][0],
                              &Coordinates[Node_Index][1],
                              &Coordinates[Node_Index][2],
                              &Initial_Normal_Spacing_,
                              &BL_Thickness_);

          if (Initial_Normal_Spacing != NULL)
            Initial_Normal_Spacing[Node_Index] = Initial_Normal_Spacing_;

          if (BL_Thickness != NULL)
            BL_Thickness[Node_Index] = BL_Thickness_;
        }
      }

      else if (Mode == 4)
      {
        BL_Thickness_ = dc0;

        for (Node_Index = 2; Node_Index <= *Number_of_Nodes; ++Node_Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf %lf %lf %lf",
                              &Coordinates[Node_Index][0],
                              &Coordinates[Node_Index][1],
                              &Coordinates[Node_Index][2],
                              &Initial_Normal_Spacing_);

          if (Initial_Normal_Spacing != NULL)
            Initial_Normal_Spacing[Node_Index] = Initial_Normal_Spacing_;

          if (BL_Thickness != NULL)
            BL_Thickness[Node_Index] = BL_Thickness_;
        }
      }

      else
      {
        BL_Thickness_ = dc0;
        Initial_Normal_Spacing_ = dc0;

        for (Node_Index = 2; Node_Index <= *Number_of_Nodes; ++Node_Index)
        {
          Read_Flag = fscanf (Grid_File, "%lf %lf %lf",
                              &Coordinates[Node_Index][0],
                              &Coordinates[Node_Index][1],
                              &Coordinates[Node_Index][2]);

          if (Initial_Normal_Spacing != NULL)
            Initial_Normal_Spacing[Node_Index] = Initial_Normal_Spacing_;

          if (BL_Thickness != NULL)
            BL_Thickness[Node_Index] = BL_Thickness_;
        }
      }
    }

    Surf_Index = 0;

    if (*Number_of_Surf_Trias > 0)
    {
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
        {
          Surf_Reconnection_Flag_ = MAX (Surf_Reconnection_Flag_, 0);
          Surf_Reconnection_Flag_ = MIN (Surf_Reconnection_Flag_, 7);

          Surf_Reconnection_Flag[Surf_Index] = Surf_Reconnection_Flag_;
        }

        if (Surf_Grid_BC_Flag != NULL)
          Surf_Grid_BC_Flag[Surf_Index] = Surf_Grid_BC_Flag_;
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading SURF surface grid file");
        return (1);
      }
    }

    if (*Number_of_Surf_Quads > 0)
    {
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
        {
          Surf_Reconnection_Flag_ = MAX (Surf_Reconnection_Flag_, 0);
          Surf_Reconnection_Flag_ = MIN (Surf_Reconnection_Flag_, 7);

          Surf_Reconnection_Flag[Surf_Index] = Surf_Reconnection_Flag_;
        }

        if (Surf_Grid_BC_Flag != NULL)
          Surf_Grid_BC_Flag[Surf_Index] = Surf_Grid_BC_Flag_;
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading SURF surface grid file");
        return (1);
      }
    }
  }

  return (0);

}
