#include "UG3_LIB.h"

void ug3_bfv
 (INT_ nbface,
  INT_3D * inibf,
  DOUBLE_3D * x,
  DOUBLE_3D * bfv)

{

/*
 * Determine a normal vector for each boundary face.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_bfv.c,v 1.5 2013/01/16 23:45:08 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface;

  double bfv1, bfv2, bfv3;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ug3_bfv_i (ibface, inibf, &bfv1, &bfv2, &bfv3, x);

    bfv[ibface][0] = bfv1;
    bfv[ibface][1] = bfv2;
    bfv[ibface][2] = bfv3;
  }

  return;

}

void ug3_bfv_i
 (INT_ ibface,
  INT_3D * inibf,
  double *bfv1,
  double *bfv2,
  double *bfv3,
  DOUBLE_3D * x)

{

/*
 * Determine a normal vector for a given boundary face.
 */

  INT_ inode1, inode2, inode3;

  double a1, a2, a3, dc1, dx211, dx212, dx213, dx311, dx312, dx313, w, x11, x12,
         x13, x21, x22, x23, x31, x32, x33;

  dc1 = 1.0;

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

  *bfv1 = w * a1;
  *bfv2 = w * a2;
  *bfv3 = w * a3;

  return;

}
