#include "UG3_LIB.h"

void ug3_bv
 (INT_ nbface,
  INT_ nnode,
  INT_3D * inibf,
  DOUBLE_3D * bfv,
  DOUBLE_3D * bv)

{

/*
 * Determine a normal vector for each boundary node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_bv.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, inode, inode1, inode2, inode3;

  double bfv1, bfv2, bfv3, bv1, bv2, bv3, dc0, dc1, w;

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

    bfv1 = bfv[ibface][0];
    bfv2 = bfv[ibface][1];
    bfv3 = bfv[ibface][2];

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
