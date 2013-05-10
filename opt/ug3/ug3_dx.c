#include "UG3_LIB.h"

double ug3_dx
 (INT_ nnode,
  DOUBLE_3D * x)

{

/*
 * Determine maximum dimension for a set of nodes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_dx.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ inode;

  double dx, dx1, dx2, dx3, x1, x1max, x1min, x2, x2max, x2min, x3, x3max,
         x3min;

  inode = 1;

  x1 = x[inode][0];
  x2 = x[inode][1];
  x3 = x[inode][2];

  x1max = x1;
  x2max = x2;
  x3max = x3;
  x1min = x1;
  x2min = x2;
  x3min = x3;

  for (inode = 2; inode <= nnode; ++inode)
  {
    x1 = x[inode][0];
    x2 = x[inode][1];
    x3 = x[inode][2];

    x1max = MAX (x1max, x1);
    x2max = MAX (x2max, x2);
    x3max = MAX (x3max, x3);
    x1min = MIN (x1min, x1);
    x2min = MIN (x2min, x2);
    x3min = MIN (x3min, x3);
  }

  dx1 = x1max - x1min;
  dx2 = x2max - x2min;
  dx3 = x3max - x3min;

  dx = MAX (dx1, dx2);
  dx = MAX (dx, dx3);

  return (dx);

}
