#include "UG3_LIB.h"

double ug3_angmax
 (INT_ ielem00,
  INT_ nelem,
  INT_4D * iniel,
  DOUBLE_3D * x)

{

/*
 * Find maximum dihedral angle for all tetrahedral elements of volume grid.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_angmax.c,v 1.9 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, inode1, inode2, inode3, inode4;

  double angmax, a1241, a1242, a1243, a124sr, a1321, a1322, a1323, a132sr,
         a1431, a1432, a1433, a143sr, a2341, a2342, a2343, a234sr, dc0, dc1,
         dc45, dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412, dx413, w,
         wmax, w21, w31, w32, w41, w42, w43, x11, x12, x13, x21, x22, x23, x31,
         x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  wmax = - dc1;

  for (ielem = ielem00; ielem <= nelem; ++ielem)
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

    a1241 = dx212 * dx413 - dx213 * dx412;
    a1242 = dx213 * dx411 - dx211 * dx413;
    a1243 = dx211 * dx412 - dx212 * dx411;
    a1321 = dx312 * dx213 - dx313 * dx212;
    a1322 = dx313 * dx211 - dx311 * dx213;
    a1323 = dx311 * dx212 - dx312 * dx211;
    a1431 = dx412 * dx313 - dx413 * dx312;
    a1432 = dx413 * dx311 - dx411 * dx313;
    a1433 = dx411 * dx312 - dx412 * dx311;
    a2341 = -a1241 - a1321 - a1431;
    a2342 = -a1242 - a1322 - a1432;
    a2343 = -a1243 - a1323 - a1433;

    a124sr = dc1 / (a1241 * a1241 + a1242 * a1242 + a1243 * a1243);
    a132sr = dc1 / (a1321 * a1321 + a1322 * a1322 + a1323 * a1323);
    a143sr = dc1 / (a1431 * a1431 + a1432 * a1432 + a1433 * a1433);
    a234sr = dc1 / (a2341 * a2341 + a2342 * a2342 + a2343 * a2343);

    w21 = a1241 * a1321 + a1242 * a1322 + a1243 * a1323;
    w31 = a1431 * a1321 + a1432 * a1322 + a1433 * a1323;
    w41 = a1431 * a1241 + a1432 * a1242 + a1433 * a1243;
    w32 = a2341 * a1321 + a2342 * a1322 + a2343 * a1323;
    w42 = a2341 * a1241 + a2342 * a1242 + a2343 * a1243;
    w43 = a2341 * a1431 + a2342 * a1432 + a2343 * a1433;

    w21 = w21 * fabs (w21) * a124sr * a132sr;
    w31 = w31 * fabs (w31) * a143sr * a132sr;
    w41 = w41 * fabs (w41) * a143sr * a124sr;
    w32 = w32 * fabs (w32) * a234sr * a132sr;
    w42 = w42 * fabs (w42) * a234sr * a124sr;
    w43 = w43 * fabs (w43) * a234sr * a143sr;

    wmax = MAX (w21, wmax);
    wmax = MAX (w31, wmax);
    wmax = MAX (w41, wmax);
    wmax = MAX (w32, wmax);
    wmax = MAX (w42, wmax);
    wmax = MAX (w43, wmax);
  }

  w = sqrt (fabs (wmax));
  w = MIN (w, dc1);

  angmax = (wmax >= dc0) ? acos (- w): acos (w);
  angmax = angmax * dc45 / atan (dc1);

  return (angmax);

}
