#include "UG3_LIB.h"

void ug3_xmax
 (INT_ nnode,
  double *x1max,
  double *x1min,
  double *x2max,
  double *x2min,
  double *x3max,
  double *x3min,
  DOUBLE_3D * x)

{

/*
 * Determine min/max coordinates for a given set of nodes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_xmax.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ inode;

  double x1, x2, x3;

  inode = 1;

  x1 = x[inode][0];
  x2 = x[inode][1];
  x3 = x[inode][2];

  *x1max = x1;
  *x2max = x2;
  *x3max = x3;
  *x1min = x1;
  *x2min = x2;
  *x3min = x3;

  for (inode = 2; inode <= nnode; ++inode)
  {
    x1 = x[inode][0];
    x2 = x[inode][1];
    x3 = x[inode][2];

    *x1max = MAX (*x1max, x1);
    *x2max = MAX (*x2max, x2);
    *x3max = MAX (*x3max, x3);
    *x1min = MIN (*x1min, x1);
    *x2min = MIN (*x2min, x2);
    *x3min = MIN (*x3min, x3);
  }

  return;

}
