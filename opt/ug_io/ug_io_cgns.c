#include "UG_IO_LIB.h"

/*
 * Read and write grid data from a CGNS file using external routines.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_cgns.c,v 1.4 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

INT_ (*ext_cgns_ug_io_read_grid) (char[],
                                  char[],
                                  INT_,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_1D *,
                                  INT_1D *,
                                  INT_4D *,
                                  INT_1D *,
                                  INT_3D *,
                                  INT_8D *,
                                  INT_1D *,
                                  INT_5D *,
                                  INT_6D *,
                                  INT_4D *,
                                  DOUBLE_3D *,
                                  DOUBLE_1D *,
                                  DOUBLE_1D *) = 0;

INT_ (*ext_cgns_ug_io_write_grid) (char[],
                                   char[],
                                   char[],
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_1D *,
                                   INT_1D *,
                                   INT_4D *,
                                   INT_1D *,
                                   INT_3D *,
                                   INT_8D *,
                                   INT_1D *,
                                   INT_5D *,
                                   INT_6D *,
                                   INT_4D *,
                                   DOUBLE_3D *,
                                   DOUBLE_1D *,
                                   DOUBLE_1D *) = 0;

INT_ ug_io_read_cgns_grid
 (char Grid_File_Name[],
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_BL_Vol_Tets,
  INT_ *Number_of_Bnd_Nodes,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Hexs,
  INT_ *Number_of_Vol_Pents_5,
  INT_ *Number_of_Vol_Pents_6,
  INT_ *Number_of_Vol_Tets,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_1D * Surf_Reconnection_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D * Vol_Hex_Connectivity,
  INT_1D * Vol_ID_Flag,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

  if (ug_io_cgns_grid_lib_check ())
    return ((*ext_cgns_ug_io_read_grid) (Grid_File_Name, Error_Message,
                                              Read_Task_Flag,
                                              Number_of_BL_Vol_Tets,
                                              Number_of_Bnd_Nodes,
                                              Number_of_Nodes, 
                                              Number_of_Surf_Quads,
                                              Number_of_Surf_Trias,
                                              Number_of_Vol_Hexs,
                                              Number_of_Vol_Pents_5,
                                              Number_of_Vol_Pents_6,
                                              Number_of_Vol_Tets,
                                              Surf_Grid_BC_Flag, Surf_ID_Flag, 
                                              Surf_Quad_Connectivity,
                                              Surf_Reconnection_Flag,
                                              Surf_Tria_Connectivity,
                                              Vol_Hex_Connectivity,
                                              Vol_ID_Flag,
                                              Vol_Pent_5_Connectivity,
                                              Vol_Pent_6_Connectivity,
                                              Vol_Tet_Connectivity,
                                              Coordinates,
                                              Initial_Normal_Spacing,
                                              BL_Thickness));

  strcpy (Error_Message, "unknown input grid file type");

  return (1);
}

INT_ ug_io_write_cgns_grid
 (char Case_Name[],
  char Grid_File_Name[],
  char Error_Message[],
  INT_ Number_of_BL_Vol_Tets,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6,
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_1D * Surf_Reconnection_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D * Vol_Hex_Connectivity,
  INT_1D * Vol_ID_Flag,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

  if (ug_io_cgns_grid_lib_check ())
    return ((*ext_cgns_ug_io_write_grid) (Case_Name, Grid_File_Name,
                                               Error_Message,
                                               Number_of_BL_Vol_Tets,
                                               Number_of_Nodes,
                                               Number_of_Surf_Quads,
                                               Number_of_Surf_Trias,
                                               Number_of_Vol_Hexs,
                                               Number_of_Vol_Pents_5,
                                               Number_of_Vol_Pents_6,
                                               Number_of_Vol_Tets,
                                               Surf_Grid_BC_Flag,
                                               Surf_ID_Flag, 
                                               Surf_Quad_Connectivity,
                                               Surf_Reconnection_Flag,
                                               Surf_Tria_Connectivity,
                                               Vol_Hex_Connectivity,
                                               Vol_ID_Flag, 
                                               Vol_Pent_5_Connectivity,
                                               Vol_Pent_6_Connectivity,
                                               Vol_Tet_Connectivity,
                                               Coordinates,
                                               Initial_Normal_Spacing,
                                               BL_Thickness));

  strcpy (Error_Message, "unknown output grid file type");

  return (1);
}

INT_ ug_io_cgns_grid_lib_check (void)
{
  // return a positive value if the cgns library routines are available

  if (ext_cgns_ug_io_read_grid && ext_cgns_ug_io_write_grid)
    return (1);

  // cgns library routines are not available

  else
    return (0);
}

// register names of external cgns library routines

void ug_io_cgns_register_read_grid
 (INT_ (*ext_routine) (char[],
                       char[],
                       INT_,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_1D *,
                       INT_1D *,
                       INT_4D *,
                       INT_1D *,
                       INT_3D *,
                       INT_8D *,
                       INT_1D *,
                       INT_5D *,
                       INT_6D *,
                       INT_4D *,
                       DOUBLE_3D *,
                       DOUBLE_1D *,
                       DOUBLE_1D *))
{
  ext_cgns_ug_io_read_grid = ext_routine;
  return;
}

void ug_io_cgns_register_write_grid
 (INT_ (*ext_routine) (char[],
                       char[],
                       char[],
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_1D *,
                       INT_1D *,
                       INT_4D *,
                       INT_1D *,
                       INT_3D *,
                       INT_8D *,
                       INT_1D *,
                       INT_5D *,
                       INT_6D *,
                       INT_4D *,
                       DOUBLE_3D *,
                       DOUBLE_1D *,
                       DOUBLE_1D *))
{
  ext_cgns_ug_io_write_grid = ext_routine;
  return;
}


