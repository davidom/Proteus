#include "UG_IO_LIB.h"

INT_ ug_io_malloc_2d_grid
 (char Error_Message[],
  INT_ M_BL_Thickness,
  INT_ M_Initial_Normal_Spacing,
  INT_ M_Bnd_Edge_Grid_BC_Flag,
  INT_ M_Bnd_Edge_ID_Flag,
  INT_ Number_of_Bnd_Edges,
  INT_ Number_of_Nodes,
  INT_ Number_of_Quads,
  INT_ Number_of_Trias,
  INT_2D ** Bnd_Edge_Connectivity,
  INT_1D ** Bnd_Edge_Grid_BC_Flag,
  INT_1D ** Bnd_Edge_ID_Flag,
  INT_4D ** Quad_Connectivity,
  INT_3D ** Tria_Connectivity,
  DOUBLE_2D ** Coordinates,
  DOUBLE_1D ** Initial_Normal_Spacing,
  DOUBLE_1D ** BL_Thickness)

{

/*
 * Malloc 2D grid data arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_malloc_2d_grid.c,v 1.8 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Error_Flag, N_Bnd_Edge_Grid_BC_Flag, N_Bnd_Edge_ID_Flag,
       N_BL_Thickness, N_Initial_Normal_Spacing;

  Error_Flag = 0;

  N_Bnd_Edge_Grid_BC_Flag = (M_Bnd_Edge_Grid_BC_Flag == 1) ? 1 : 0;
  N_Bnd_Edge_ID_Flag = (M_Bnd_Edge_ID_Flag == 1) ? 1 : 0;
  N_Initial_Normal_Spacing = (M_Initial_Normal_Spacing == 1) ? 1 : 0;
  N_BL_Thickness = (M_BL_Thickness == 1) ? 1 : 0;

  *Bnd_Edge_Connectivity = (INT_2D *) ug_malloc (&Error_Flag,
                                                 (Number_of_Bnd_Edges+1)
                                               * sizeof (INT_2D));
  *Bnd_Edge_Grid_BC_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                                 (Number_of_Bnd_Edges+1)
                                               * N_Bnd_Edge_Grid_BC_Flag
                                               * sizeof (INT_1D));
  *Bnd_Edge_ID_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                            (Number_of_Bnd_Edges+1)
                                           * N_Bnd_Edge_ID_Flag
                                           * sizeof (INT_1D));
  *Quad_Connectivity = (INT_4D *) ug_malloc (&Error_Flag,
                                             (Number_of_Quads+1)
                                           * sizeof (INT_4D));
  *Tria_Connectivity = (INT_3D *) ug_malloc (&Error_Flag,
                                             (Number_of_Trias+1)
                                           * sizeof (INT_3D));
  *Coordinates = (DOUBLE_2D *) ug_malloc (&Error_Flag,
                                          (Number_of_Nodes+1)
                                        * sizeof (DOUBLE_2D));
  *Initial_Normal_Spacing = (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                                     (Number_of_Nodes+1)
                                                   * N_Initial_Normal_Spacing
                                                   * sizeof (DOUBLE_1D));
  *BL_Thickness = (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                           (Number_of_Nodes+1)
                                         * N_BL_Thickness
                                         * sizeof (DOUBLE_1D));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc grid data arrays");
    return (610);
  }

  return (0);

}
