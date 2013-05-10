#include "UG_IO_LIB.h"

void ug_io_free_2d_grid
 (INT_2D * Bnd_Edge_Connectivity,
  INT_1D * Bnd_Edge_Grid_BC_Flag,
  INT_1D * Bnd_Edge_ID_Flag,
  INT_4D * Quad_Connectivity,
  INT_3D * Tria_Connectivity,
  DOUBLE_2D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Free 2D grid data arrays.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_free_2d_grid.c,v 1.5 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  ug_free (Bnd_Edge_Connectivity);
  ug_free (Bnd_Edge_Grid_BC_Flag);
  ug_free (Bnd_Edge_ID_Flag);
  ug_free (Quad_Connectivity);
  ug_free (Tria_Connectivity);
  ug_free (Coordinates);
  ug_free (Initial_Normal_Spacing);
  ug_free (BL_Thickness);

  return;

}
