#include "UG_IO_LIB.h"

INT_ ug_io_malloc_node
 (char Error_Message[],
  INT_ M_Spacing,
  INT_ M_Vector1,
  INT_ M_Vector2,
  INT_ Number_of_Nodes,
  DOUBLE_3D ** Coordinates,
  DOUBLE_1D ** Spacing,
  DOUBLE_4D ** Vector1,
  DOUBLE_4D ** Vector2)

{

/*
 * Malloc node data arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_malloc_node.c,v 1.4 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Error_Flag;

  Error_Flag = 0;

  *Coordinates = (DOUBLE_3D *) ug_malloc (&Error_Flag,
                                          (Number_of_Nodes+1)
                                        * sizeof (DOUBLE_3D));

  if (M_Spacing == 1)
    *Spacing = (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                        (Number_of_Nodes+1)
                                      * sizeof (DOUBLE_1D));
  else
    *Spacing = NULL;

  if (M_Vector1 == 1)
    *Vector1 = (DOUBLE_4D *) ug_malloc (&Error_Flag,
                                        (Number_of_Nodes+1)
                                      * sizeof (DOUBLE_4D));
  else
    *Vector1 = NULL;

  if (M_Vector2 == 1)
    *Vector2 = (DOUBLE_4D *) ug_malloc (&Error_Flag,
                                        (Number_of_Nodes+1)
                                      * sizeof (DOUBLE_4D));
  else
    *Vector2 = NULL;

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc grid data arrays");
    return (617);
  }

  return (0);

}
