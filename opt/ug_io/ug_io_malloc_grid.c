#include "UG_IO_LIB.h"

INT_ ug_io_malloc_grid
 (char Error_Message[],
  INT_ M_BL_Thickness,
  INT_ M_Initial_Normal_Spacing,
  INT_ M_Surf_Grid_BC_Flag,
  INT_ M_Surf_ID_Flag,
  INT_ M_Surf_Reconnection_Flag,
  INT_ M_Vol_ID_Flag,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6,
  INT_ Number_of_Vol_Tets,
  INT_1D ** Surf_Grid_BC_Flag,
  INT_1D ** Surf_ID_Flag,
  INT_1D ** Surf_Reconnection_Flag,
  INT_4D ** Surf_Quad_Connectivity,
  INT_3D ** Surf_Tria_Connectivity,
  INT_8D ** Vol_Hex_Connectivity,
  INT_1D ** Vol_ID_Flag,
  INT_5D ** Vol_Pent_5_Connectivity,
  INT_6D ** Vol_Pent_6_Connectivity,
  INT_4D ** Vol_Tet_Connectivity,
  DOUBLE_3D ** Coordinates,
  DOUBLE_1D ** Initial_Normal_Spacing,
  DOUBLE_1D ** BL_Thickness)

{

/*
 * Malloc grid data arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_malloc_grid.c,v 1.20 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Error_Flag, N_BL_Thickness, N_Initial_Normal_Spacing,
       N_Surf_Grid_BC_Flag, N_Surf_ID_Flag, N_Surf_Reconnection_Flag,
       N_Vol_ID_Flag, Number_of_Surf_Faces, Number_of_Vol_Elems;

  Error_Flag = 0;

  N_BL_Thickness = (M_BL_Thickness == 1) ? 1 : 0;
  N_Initial_Normal_Spacing = (M_Initial_Normal_Spacing == 1) ? 1 : 0;
  N_Surf_Grid_BC_Flag = (M_Surf_Grid_BC_Flag == 1) ? 1 : 0;
  N_Surf_ID_Flag = (M_Surf_ID_Flag == 1) ? 1 : 0;
  N_Surf_Reconnection_Flag = (M_Surf_Reconnection_Flag == 1) ? 1 : 0;
  N_Vol_ID_Flag = (M_Vol_ID_Flag == 1) ? 1 : 0;

  Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;

  Number_of_Vol_Elems = Number_of_Vol_Tets + Number_of_Vol_Pents_5
                      + Number_of_Vol_Pents_6 + Number_of_Vol_Hexs;

  *Surf_Grid_BC_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                             (Number_of_Surf_Faces+1)
                                           * N_Surf_Grid_BC_Flag
                                           * sizeof (INT_1D));
  *Surf_ID_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                        (Number_of_Surf_Faces+1)
                                      * N_Surf_ID_Flag
                                      * sizeof (INT_1D));
  *Surf_Reconnection_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                                  (Number_of_Surf_Faces+1)
                                                * N_Surf_Reconnection_Flag
                                                * sizeof (INT_1D));
  *Surf_Quad_Connectivity = (INT_4D *) ug_malloc (&Error_Flag,
                                                  (Number_of_Surf_Quads+1)
                                                * sizeof (INT_4D));
  *Surf_Tria_Connectivity = (INT_3D *) ug_malloc (&Error_Flag,
                                                  (Number_of_Surf_Trias+1)
                                                * sizeof (INT_3D));
  *Vol_Hex_Connectivity = (INT_8D *) ug_malloc (&Error_Flag,
                                                (Number_of_Vol_Hexs+1)
                                              * sizeof (INT_8D));
  *Vol_ID_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                       (Number_of_Vol_Elems+1)
                                     * N_Vol_ID_Flag
                                     * sizeof (INT_1D));
  *Vol_Pent_5_Connectivity = (INT_5D *) ug_malloc (&Error_Flag,
                                                   (Number_of_Vol_Pents_5+1)
                                                 * sizeof (INT_5D));
  *Vol_Pent_6_Connectivity = (INT_6D *) ug_malloc (&Error_Flag,
                                                   (Number_of_Vol_Pents_6+1)
                                                 * sizeof (INT_6D));
  *Vol_Tet_Connectivity = (INT_4D *) ug_malloc (&Error_Flag,
                                                (Number_of_Vol_Tets+1)
                                              * sizeof (INT_4D));
  *Coordinates = (DOUBLE_3D *) ug_malloc (&Error_Flag,
                                          (Number_of_Nodes+1)
                                        * sizeof (DOUBLE_3D));
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
    return (613);
  }

  if (M_Vol_ID_Flag == 1)
    ug_set_int (1, Number_of_Vol_Elems, -123456, *Vol_ID_Flag);

  return (0);

}
