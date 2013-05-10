#include "UG_IO_LIB.h"

void ug_io_free_2d_node
 (DOUBLE_2D * Coordinates,
  DOUBLE_1D * Spacing,
  DOUBLE_3D * Vector)

{

/*
 * Free 2D node data arrays.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_free_2d_node.c,v 1.4 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  ug_free (Coordinates);
  ug_free (Spacing);
  ug_free (Vector);

  return;

}
