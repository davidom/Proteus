#include "UG3_LIB.h"

void ug3_lrmin
 (INT_ ielem0,
  INT_ nelem,
  INT_4D * iniel,
  double * lrmin,
  DOUBLE_3D * x)

{

/*
 * Determine minimum edge length ratio.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_lrmin.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, inode1, inode2, inode3, inode4;

  double dc1, dx211, dx212, dx213, dx311, dx312, dx313, dx321, dx322, dx323,
         dx411, dx412, dx413, dx421, dx422, dx423, dx431, dx432, dx433, dss21,
         dss31, dss32, dss41, dss42, dss43, dssmax, dssmin, lrs, lrsmin, x11,
         x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc1 = 1.0;

  lrsmin = dc1;

  for (ielem = ielem0; ielem <= nelem; ++ielem)
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
    dx321 = x31 - x21;
    dx322 = x32 - x22;
    dx323 = x33 - x23;
    dx411 = x41 - x11;
    dx412 = x42 - x12;
    dx413 = x43 - x13;
    dx421 = x41 - x21;
    dx422 = x42 - x22;
    dx423 = x43 - x23;
    dx431 = x41 - x31;
    dx432 = x42 - x32;
    dx433 = x43 - x33;

    dss21 = dx211 * dx211 + dx212 * dx212 + dx213 * dx213;
    dss31 = dx311 * dx311 + dx312 * dx312 + dx313 * dx313;
    dss32 = dx321 * dx321 + dx322 * dx322 + dx323 * dx323;
    dss41 = dx411 * dx411 + dx412 * dx412 + dx413 * dx413;
    dss42 = dx421 * dx421 + dx422 * dx422 + dx423 * dx423;
    dss43 = dx431 * dx431 + dx432 * dx432 + dx433 * dx433;

    dssmax = MAX (dss21, dss31);
    dssmax = MAX (dss32, dssmax);
    dssmax = MAX (dss41, dssmax);
    dssmax = MAX (dss42, dssmax);
    dssmax = MAX (dss43, dssmax);

    dssmin = MIN (dss21, dss31);
    dssmin = MIN (dss32, dssmin);
    dssmin = MIN (dss41, dssmin);
    dssmin = MIN (dss42, dssmin);
    dssmin = MIN (dss43, dssmin);

    lrs = dssmin / dssmax;

    lrsmin = MIN (lrs, lrsmin);
  }

  *lrmin = sqrt (lrsmin);

  return;

}
