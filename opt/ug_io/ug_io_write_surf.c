#include "UG_IO_LIB.h"

INT_ ug_io_write_surf
 (FILE * Grid_File,
  char Error_Message[],
  INT_ *Number_of_Bnd_Nodes,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Version_Flag,
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
 * Write grid data to a SURF surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_surf.c,v 1.25 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *Node_Index_Map;

  INT_ Error_Flag, Node_Index, Node_Index1, Node_Index2, Node_Index3,
       Node_Index4, Surf_Index, Surf_Node_Index, Surf_Quad_Index, 
       Surf_Tria_Index, Write_Flag;

  INT_ Surf_Grid_BC_Flag_ = -1;
  INT_ Surf_Reconnection_Flag_ = 0;
  
  double BL_Thickness_ = 0.0;
  double Initial_Normal_Spacing_ = 0.0;

  Error_Flag = ug3_bnd_nodes (0, Number_of_Bnd_Nodes, Number_of_Nodes,
                              Number_of_Surf_Trias, Number_of_Surf_Quads,
                              Surf_Tria_Connectivity, Surf_Quad_Connectivity,
                              &Node_Index_Map,
                              BL_Thickness, Initial_Normal_Spacing,
                              Coordinates);

  if (Error_Flag > 0)
  {
    sprintf (Error_Message, "unable to allocate required memory %i",
             Error_Flag);
    return (1);
  }

  Write_Flag = fprintf (Grid_File, "%i %i %i\n", Number_of_Surf_Trias,
                                                 Number_of_Surf_Quads,
                                                 *Number_of_Bnd_Nodes);

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    strcpy (Error_Message, "error writing SURF surface grid file");
    return (1);
  }

  if (Version_Flag == -1)
  {
    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      Surf_Node_Index = Node_Index_Map[Node_Index];

      if (Surf_Node_Index > 0)
      {
        Write_Flag = fprintf (Grid_File, "%.15g %.15g %.15g\n",
                              Coordinates[Node_Index][0],
                              Coordinates[Node_Index][1],
                              Coordinates[Node_Index][2]);
      }
    }
  }

  else
  {
    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      Surf_Node_Index = Node_Index_Map[Node_Index];

      if (Surf_Node_Index > 0)
      {
        if (Initial_Normal_Spacing != NULL)
          Initial_Normal_Spacing_ = Initial_Normal_Spacing[Node_Index];

        if (BL_Thickness != NULL)
          BL_Thickness_ = BL_Thickness[Node_Index];

        Write_Flag = fprintf (Grid_File, "%.15g %.15g %.15g %.15g %.15g\n",
                              Coordinates[Node_Index][0],
                              Coordinates[Node_Index][1],
                              Coordinates[Node_Index][2],
                              Initial_Normal_Spacing_,
                              BL_Thickness_);
      }
    }
  }

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    strcpy (Error_Message, "error writing SURF surface grid file");
    return (1);
  }

  Surf_Index = 0;

  for (Surf_Tria_Index = 1;
       Surf_Tria_Index <= Number_of_Surf_Trias; ++Surf_Tria_Index)
  {
    ++Surf_Index;

    Node_Index1 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
    Node_Index2 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
    Node_Index3 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

    if (Surf_Reconnection_Flag != NULL)
      Surf_Reconnection_Flag_ = Surf_Reconnection_Flag[Surf_Index];

    if (Surf_Grid_BC_Flag != NULL)
      Surf_Grid_BC_Flag_ = Surf_Grid_BC_Flag[Surf_Index];

    Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i\n",
                          Node_Index_Map[Node_Index1],
                          Node_Index_Map[Node_Index2],
                          Node_Index_Map[Node_Index3],
                          Surf_ID_Flag[Surf_Index],
                          Surf_Reconnection_Flag_,
                          Surf_Grid_BC_Flag_);
  }

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    strcpy (Error_Message, "error writing SURF surface grid file");
    return (1);
  }

  for (Surf_Quad_Index = 1;
       Surf_Quad_Index <= Number_of_Surf_Quads; ++Surf_Quad_Index)
  {
    ++Surf_Index;

    Node_Index1 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
    Node_Index2 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
    Node_Index3 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
    Node_Index4 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

    if (Surf_Reconnection_Flag != NULL)
      Surf_Reconnection_Flag_ = Surf_Reconnection_Flag[Surf_Index];

    if (Surf_Grid_BC_Flag != NULL)
      Surf_Grid_BC_Flag_ = Surf_Grid_BC_Flag[Surf_Index];

    Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i %i\n",
                          Node_Index_Map[Node_Index1],
                          Node_Index_Map[Node_Index2],
                          Node_Index_Map[Node_Index3],
                          Node_Index_Map[Node_Index4],
                          Surf_ID_Flag[Surf_Index],
                          Surf_Reconnection_Flag_,
                          Surf_Grid_BC_Flag_);
  }

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    strcpy (Error_Message, "error writing SURF surface grid file");
    return (1);
  }

  ug_free (Node_Index_Map);

  return (0);

}
