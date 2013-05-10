#include "UG3_LIB.h"

void ug3_x2x3
 (INT_ nnode,
  double sv1,
  double sv2,
  double sv3,
  double tv1,
  double tv2,
  double tv3,
  double x01,
  double x02,
  double x03,
  DOUBLE_2D * x2d,
  DOUBLE_3D * x3d)

{

/*
 * Transform to 3d coordinates from 2d plane orientated coordinates.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_x2x3.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */  

  INT_ inode;

  double x2d1, x2d2;

  for (inode = 1; inode <= nnode; ++inode)
  {
    x2d1 = x2d[inode][0];
    x2d2 = x2d[inode][1]; 

    x3d[inode][0] = x01 + sv1 * x2d1 + tv1 * x2d2;
    x3d[inode][1] = x02 + sv2 * x2d1 + tv2 * x2d2;
    x3d[inode][2] = x03 + sv3 * x2d1 + tv3 * x2d2;
  }

  return;

}
