#include "UG_IO_LIB.h"

INT_ ug_io_malloc_2d_node
 (char Error_Message[],
  INT_ M_Spacing,
  INT_ M_Vector,
  INT_ Number_of_Nodes,
  DOUBLE_2D ** Coordinates,
  DOUBLE_1D ** Spacing,
  DOUBLE_3D ** Vector)

{

/*
 * Malloc 2D node data arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_malloc_2d_node.c,v 1.5 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Error_Flag;

  Error_Flag = 0;

  *Coordinates = (DOUBLE_2D *) ug_malloc (&Error_Flag,
                                          (Number_of_Nodes+1)
                                        * sizeof (DOUBLE_2D));

  if (M_Spacing == 1)
    *Spacing = (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                        (Number_of_Nodes+1)
                                      * sizeof (DOUBLE_1D));
  else
    *Spacing = NULL;

  if (M_Vector == 1)
    *Vector = (DOUBLE_3D *) ug_malloc (&Error_Flag,
                                       (Number_of_Nodes+1)
                                     * sizeof (DOUBLE_3D));
  else
    *Vector = NULL;

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc grid data arrays");
    return (636);
  }

  return (0);

}
