#include "UG_IO_LIB.h"

void ug_io_free_node
 (DOUBLE_3D * Coordinates,
  DOUBLE_1D * Spacing,
  DOUBLE_4D * Vector1,
  DOUBLE_4D * Vector2)

{

/*
 * Free node data arrays.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_free_node.c,v 1.4 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  ug_free (Coordinates);
  ug_free (Spacing);
  ug_free (Vector1);
  ug_free (Vector2);

  return;

}
