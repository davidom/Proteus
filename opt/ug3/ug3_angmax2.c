#include "UG3_LIB.h"

void ug3_angmax2
 (INT_ nelem,
  INT_4D * iniel,
  INT_1D * mflag,
  double *angmax0,
  double *angmax1,
  double *lsrmax0,
  double *lsrmax1,
  DOUBLE_1D * df,
  DOUBLE_3D * x)

{

/*
 * Determine maximum dihedral angle and length scale ratio for each set of
 * flagged elements (those with a flag of 0 and those with a flag of 1).
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_angmax2.c,v 1.8 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, iflag, inode1, inode2, inode3, inode4, nelemi;

  double a1241, a1242, a1243, a124sr, a1321, a1322, a1323, a132sr, a1431, a1432,
         a1433, a143sr, a2341, a2342, a2343, a234sr, angmax, dc0, dc1, dc2,
         dc45, df1, df2, df21, df3, df31, df32, df4, df41, df42, df43, dx211,
         dx212, dx213, dx311, dx312, dx313, dx321, dx322, dx323, dx411, dx412,
         dx413, dx421, dx422, dx423, dx431, dx432, dx433, lsrmax, w, w21, w31,
         w32, w41, w42, w43, wmax, x11, x12, x13, x21, x22, x23, x31, x32, x33,
         x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc2 = 2.0;
  dc45 = 45.0;

  for (iflag = 0; iflag <= 1; ++iflag)
  {
    wmax = - dc1;

    lsrmax = dc0;

    nelemi = 0;

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (mflag[ielem] == iflag)
      {
        ++nelemi;

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

        if (df != NULL)
        {
          df1 = df[inode1];
          df2 = df[inode2];
          df3 = df[inode3];
          df4 = df[inode4];

          dx321 = x31 - x21;
          dx322 = x32 - x22;
          dx323 = x33 - x23;
          dx421 = x41 - x21;
          dx422 = x42 - x22;
          dx423 = x43 - x23;
          dx431 = x41 - x31;
          dx432 = x42 - x32;
          dx433 = x43 - x33;

          df21 = df2 + df1;
          df31 = df3 + df1;
          df41 = df4 + df1;
          df21 = df2 + df1;
          df32 = df3 + df2;
          df42 = df4 + df2;
          df43 = df4 + df3;

          w21 = (dx211 * dx211 + dx212 * dx212 + dx213 * dx213) / (df21 * df21);
          w31 = (dx311 * dx311 + dx312 * dx312 + dx313 * dx313) / (df31 * df31);
          w41 = (dx411 * dx411 + dx412 * dx412 + dx413 * dx413) / (df41 * df41);
          w32 = (dx321 * dx321 + dx322 * dx322 + dx323 * dx323) / (df32 * df32);
          w42 = (dx421 * dx421 + dx422 * dx422 + dx423 * dx423) / (df42 * df42);
          w43 = (dx431 * dx431 + dx432 * dx432 + dx433 * dx433) / (df43 * df43);

          lsrmax = MAX (lsrmax, w21);
          lsrmax = MAX (lsrmax, w31);
          lsrmax = MAX (lsrmax, w41);
          lsrmax = MAX (lsrmax, w32);
          lsrmax = MAX (lsrmax, w42);
          lsrmax = MAX (lsrmax, w43);
        }
      }
    }

    if (nelemi > 0)
    {
      w = sqrt (fabs (wmax));
      w = MIN (w, dc1);

      angmax = (wmax >= dc0) ? acos (- w): acos (w);
      angmax = angmax * dc45 / atan (dc1);
    }
    else
      angmax = dc0;

    if (iflag == 0) *angmax0 = angmax;
    if (iflag == 1) *angmax1 = angmax;

    if (df != NULL)
    {
      lsrmax = dc2 * sqrt (lsrmax);

      if (iflag == 0) *lsrmax0 = lsrmax;
      if (iflag == 1) *lsrmax1 = lsrmax;
    }
  }

  return;

}
