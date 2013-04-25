#include "UG_IO_LIB.h"

void ug_io_free_grid
 (INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_1D * Surf_Reconnection_Flag,
  INT_4D * Surf_Quad_Connectivity,
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

/*
 * Free grid data arrays.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_free_grid.c,v 1.12 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  ug_free (Surf_Grid_BC_Flag);
  ug_free (Surf_ID_Flag);
  ug_free (Surf_Quad_Connectivity);
  ug_free (Surf_Tria_Connectivity);
  ug_free (Surf_Reconnection_Flag);
  ug_free (Vol_Hex_Connectivity);
  ug_free (Vol_ID_Flag);
  ug_free (Vol_Pent_5_Connectivity);
  ug_free (Vol_Pent_6_Connectivity);
  ug_free (Vol_Tet_Connectivity);
  ug_free (Coordinates);
  ug_free (Initial_Normal_Spacing);
  ug_free (BL_Thickness);

  return;

}
