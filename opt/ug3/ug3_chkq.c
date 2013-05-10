#include "UG3_LIB.h"

INT_ ug3_chkq
 (INT_ ielem00,
  INT_ merr,
  INT_ mmsg,
  INT_ nelem,
  INT_4D * iniel,
  INT_1D * ierriel,
  double angq,
  DOUBLE_3D * x)

{

/*
 * Check dihedral angles for each tetrahedral element of volume grid.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkq.c,v 1.15 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, ierr, inode1, inode2, inode3, inode4;

  double a1241, a1242, a1243, a124s, a1321, a1322, a1323, a132s, a1431, a1432,
         a1433, a143s, a2341, a2342, a2343, a234s, cosq, cosqs, dc0, dc1, dc45,
         dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412, dx413, w, w21,
         w31, w32, w41, w42, w43, x11, x12, x13, x21, x22, x23, x31, x32, x33,
         x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  if (angq == dc0)
    return (0);

  if (mmsg == 2)
  {
    sprintf (Text, "Checking Quality   : Elements          =%10i", nelem);
    ug_message (Text);
  }

  cosq = cos (angq * atan (dc1) / dc45);
  cosqs = fabs (cosq) * cosq;

  ierr = 0;

  ielem = ielem00;

  do
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

    a124s = a1241 * a1241 + a1242 * a1242 + a1243 * a1243;
    a132s = a1321 * a1321 + a1322 * a1322 + a1323 * a1323;
    a143s = a1431 * a1431 + a1432 * a1432 + a1433 * a1433;
    a234s = a2341 * a2341 + a2342 * a2342 + a2343 * a2343;

    w21 = a1241 * a1321 + a1242 * a1322 + a1243 * a1323;
    w31 = a1431 * a1321 + a1432 * a1322 + a1433 * a1323;
    w41 = a1431 * a1241 + a1432 * a1242 + a1433 * a1243;
    w32 = a2341 * a1321 + a2342 * a1322 + a2343 * a1323;
    w42 = a2341 * a1241 + a2342 * a1242 + a2343 * a1243;
    w43 = a2341 * a1431 + a2342 * a1432 + a2343 * a1433;

    w21 = w21 * fabs (w21) + a124s * a132s * cosqs;
    w31 = w31 * fabs (w31) + a143s * a132s * cosqs;
    w41 = w31 * fabs (w31) + a143s * a124s * cosqs;
    w32 = w32 * fabs (w32) + a234s * a132s * cosqs;
    w42 = w42 * fabs (w42) + a234s * a124s * cosqs;
    w43 = w43 * fabs (w43) + a234s * a143s * cosqs;

    w = MAX (w21, w31);
    w = MAX (w41, w);
    w = MAX (w32, w);
    w = MAX (w42, w);
    w = MAX (w43, w);

    if (w > dc0)
    {
      ierr = 1;

      if (merr)
        ierriel[ielem] = -1;
    }

    ++ielem;
  }
  while (ielem <= nelem && (! ierr || merr));

  if (ierr)
  {
    if (merr)
      return (317);
    else
      return (-1);
  }

  if (mmsg == 2)
    ug_message ("Checking Quality   : Quality OK");

  return (0);

}
