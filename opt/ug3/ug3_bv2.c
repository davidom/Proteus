#include "UG3_LIB.h"

void ug3_bv2
 (INT_ nbface,
  INT_ nnode,
  INT_3D * inibf,
  DOUBLE_3D * bv,
  DOUBLE_3D * x)

{

/*
 * Determine a normal vector for each boundary node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_bv2.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, inode, inode1, inode2, inode3;

  double a1, a2, a3, bfv1, bfv2, bfv3, bv1, bv2, bv3, dc0, dc1, dx211, dx212,
         dx213, dx311, dx312, dx313, w, x11, x12, x13, x21, x22, x23, x31, x32,
         x33;

  dc0 = 0.0;
  dc1 = 1.0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    bv[inode][0] = dc0;
    bv[inode][1] = dc0;
    bv[inode][2] = dc0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    x11 = x[inode1][0];
    x12 = x[inode1][1];
    x13 = x[inode1][2];
    x21 = x[inode2][0];
    x22 = x[inode2][1];
    x23 = x[inode2][2];
    x31 = x[inode3][0];
    x32 = x[inode3][1];
    x33 = x[inode3][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;

    a1 = dx212 * dx313 - dx213 * dx312;
    a2 = dx213 * dx311 - dx211 * dx313;
    a3 = dx211 * dx312 - dx212 * dx311;

    w = dc1 / sqrt (a1 * a1 + a2 * a2 + a3 * a3);

    bfv1 = w * a1;
    bfv2 = w * a2;
    bfv3 = w * a3;

    bv[inode1][0] = bv[inode1][0] + bfv1;
    bv[inode1][1] = bv[inode1][1] + bfv2;
    bv[inode1][2] = bv[inode1][2] + bfv3;
    bv[inode2][0] = bv[inode2][0] + bfv1;
    bv[inode2][1] = bv[inode2][1] + bfv2;
    bv[inode2][2] = bv[inode2][2] + bfv3;
    bv[inode3][0] = bv[inode3][0] + bfv1;
    bv[inode3][1] = bv[inode3][1] + bfv2;
    bv[inode3][2] = bv[inode3][2] + bfv3;
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    bv1 = bv[inode][0];
    bv2 = bv[inode][1];
    bv3 = bv[inode][2];

    w = dc1 / sqrt (bv1 * bv1 + bv2 * bv2 + bv3 * bv3);

    bv[inode][0] = w * bv1;
    bv[inode][1] = w * bv2;
    bv[inode][2] = w * bv3;
  }

  return;

}
