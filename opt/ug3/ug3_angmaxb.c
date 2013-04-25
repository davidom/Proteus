#include "UG3_LIB.h"

double ug3_angmaxb
 (INT_ nbface,
  INT_3D * inibf,
  DOUBLE_3D * x)

{

/*
 * Find maximum planar angle for all triangular faces of surface grid.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_angmaxb.c,v 1.8 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, inode1, inode2, inode3;

  double angmax, dc0, dc1, dc45, ds21sr, ds31sr, ds32sr, dx211, dx212, dx213,
         dx311, dx312, dx313, dx321, dx322, dx323, wmin, w1, w2, w3, x11, x12,
         x13, x21, x22, x23, x31, x32, x33;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  wmin = dc1;

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
    dx321 = x31 - x21;
    dx322 = x32 - x22;
    dx323 = x33 - x23;

    ds21sr = dc1 / (dx211 * dx211 + dx212 * dx212 + dx213 * dx213);
    ds31sr = dc1 / (dx311 * dx311 + dx312 * dx312 + dx313 * dx313);
    ds32sr = dc1 / (dx321 * dx321 + dx322 * dx322 + dx323 * dx323);

    w1 =   dx211 * dx311 + dx212 * dx312 + dx213 * dx313;
    w2 = - dx211 * dx321 - dx212 * dx322 - dx213 * dx323;
    w3 =   dx321 * dx311 + dx322 * dx312 + dx323 * dx313;

    w1 = w1 * fabs (w1) * ds21sr * ds31sr;
    w2 = w2 * fabs (w2) * ds21sr * ds32sr;
    w3 = w3 * fabs (w3) * ds32sr * ds31sr;

    wmin = MIN (w1, wmin);
    wmin = MIN (w2, wmin);
    wmin = MIN (w3, wmin);
  }

  angmax = (wmin >= dc0) ? acos (  MIN (sqrt ( wmin), dc1)):
                           acos (- MIN (sqrt (-wmin), dc1));
  angmax = angmax * dc45 / atan (dc1);

  return (angmax);

}
