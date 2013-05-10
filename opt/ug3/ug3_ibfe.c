#include "UG3_LIB.h"

void ug3_ibfe
 (INT_ inode1,
  INT_ inode2,
  INT_ inode3,
  INT_ nbfpe,
  INT_ *ibe,
  INT_ *ibface,
  INT_1D * ibeie,
  INT_1D * ibfie,
  INT_3D * inibf,
  DOUBLE_3D * x)

{

/*
 * Determine boundary face to boundary face connectivity for a multiply
 * connected boundary face.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ibfe.c,v 1.8 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibfpe, inode4, jbfpe;

  double b11, b12, b13, b1m, b21, b22, b23, b2m, dc0, dc2, dc4, dx211, dx212,
         dx213, dx311, dx312, dx313, dx411, dx412, dx413, w1, w1max, w2, x11,
         x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc2 = 2.0;
  dc4 = 4.0;

  if (nbfpe == 1)
  {
    jbfpe = 1;
  }
  else
  {
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

    b11 = dx212 * dx313 - dx213 * dx312;
    b12 = dx213 * dx311 - dx211 * dx313;
    b13 = dx211 * dx312 - dx212 * dx311;

    b1m = sqrt (b11 * b11 + b12 * b12 + b13 * b13);

    w1max = -dc4;

    jbfpe = 0;

    for (ibfpe = 1; ibfpe <= nbfpe; ++ibfpe)
    {
      *ibe = ibeie[ibfpe];

      *ibface = ibfie[ibfpe];

      inode4 = inibf[*ibface][*ibe];

      x41 = x[inode4][0];
      x42 = x[inode4][1];
      x43 = x[inode4][2];

      dx411 = x41 - x11;
      dx412 = x42 - x12;
      dx413 = x43 - x13;

      b21 = dx212 * dx413 - dx213 * dx412;
      b22 = dx213 * dx411 - dx211 * dx413;
      b23 = dx211 * dx412 - dx212 * dx411;

      b2m = sqrt (b21 * b21 + b22 * b22 + b23 * b23);

      w1 = (b11 * b21 + b12 * b22 + b13 * b23) / (b1m * b2m);
      w2 = b11 * dx411 + b12 * dx412 + b13 * dx413;

      if (w2 < dc0)
        w1 = -dc2 - w1;

      if (w1 > w1max)
      {
        jbfpe = ibfpe;

        w1max = w1;
      }
    }
  }

  *ibe = ibeie[jbfpe];

  *ibface = ibfie[jbfpe];

  return;
}
