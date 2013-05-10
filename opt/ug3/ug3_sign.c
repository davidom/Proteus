#include "UG3_LIB.h"

INT_ ug3_sign
 (INT_ nelem,
  INT_4D * iniel,
  DOUBLE_3D * x)

{

/*
 * Check the element connectivity ordering for RH rule.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_sign.c,v 1.7 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, inode1, inode2, inode3, inode4;

  double dc0, dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412, dx413,
         sign, vol, volmax, volmin, x11, x12, x13, x21, x22, x23, x31, x32, x33,
         x41, x42, x43;

  dc0 = 0.0;

  ielem = 1;

  inode1 = iniel[ielem][0];
  inode2 = iniel[ielem][1];
  inode3 = iniel[ielem][2];
  inode4 = iniel[ielem][3];

  x11 = x[inode1][0];
  x12 = x[inode1][1];
  x13 = x[inode1][2];
  x21 = x[inode2][0];
  x22 = x[inode2][1];
  x23 = x[inode2][2];
  x31 = x[inode3][0];
  x32 = x[inode3][1];
  x33 = x[inode3][2];
  x41 = x[inode4][0];
  x42 = x[inode4][1];
  x43 = x[inode4][2];

  dx211 = x21 - x11;
  dx212 = x22 - x12;
  dx213 = x23 - x13;
  dx311 = x31 - x11;
  dx312 = x32 - x12;
  dx313 = x33 - x13;
  dx411 = x41 - x11;
  dx412 = x42 - x12;
  dx413 = x43 - x13;

  vol = dx211 * (dx312 * dx413 - dx313 * dx412)
      + dx212 * (dx313 * dx411 - dx311 * dx413)
      + dx213 * (dx311 * dx412 - dx312 * dx411);

  volmax = vol;
  volmin = vol;

  for (ielem = 2; ielem <= nelem; ++ielem)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    x11 = x[inode1][0];
    x12 = x[inode1][1];
    x13 = x[inode1][2];
    x21 = x[inode2][0];
    x22 = x[inode2][1];
    x23 = x[inode2][2];
    x31 = x[inode3][0];
    x32 = x[inode3][1];
    x33 = x[inode3][2];
    x41 = x[inode4][0];
    x42 = x[inode4][1];
    x43 = x[inode4][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;
    dx411 = x41 - x11;
    dx412 = x42 - x12;
    dx413 = x43 - x13;

    vol = dx211 * (dx312 * dx413 - dx313 * dx412)
        + dx212 * (dx313 * dx411 - dx311 * dx413)
        + dx213 * (dx311 * dx412 - dx312 * dx411);

    volmax = MAX (volmax, vol);
    volmin = MIN (volmin, vol);
  }

  sign = volmax * volmin;

  if (sign <= dc0)
    return (316);

  if (volmax < dc0)
  {
    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      inode1 = iniel[ielem][0];
      inode3 = iniel[ielem][2];

      iniel[ielem][0] = inode3;
      iniel[ielem][2] = inode1;
    }
  }

  return (0);

}
