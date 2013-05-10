#include "UG3_LIB.h"

INT_ ug3_chkplane
 (INT_ id,
  INT_ nbface,
  INT_1D * idibf,
  INT_3D * inibf,
  double angndev,
  DOUBLE_3D * x)

{

/*
 * Check if a given surface grid (id,nbface,idibf,inibf,x) is planar within
 * angndev.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkplane.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */
 
  INT_ ibface, inode1, inode2, inode3, jd;

  double a1, a2, a3, b1, b2, b3, bs, cosndev, cosndevs, dc0, dc1, dc45, dx211,
         dx212, dx213, dx311, dx312, dx313, w, x11, x12, x13, x21, x22, x23,
         x31, x32, x33;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  cosndev = cos (angndev * atan (dc1) / dc45);

  cosndevs = cosndev * fabs (cosndev);

  jd = id;

  ibface = 1;

  do
  {
    if (idibf != NULL) jd = idibf[ibface];

    if (jd == id)
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

      if (ibface == 1)
      {
        bs = a1 * a1 + a2 * a2 + a3 * a3;

        b1 = a1;
        b2 = a2;
        b3 = a3;

        w = dc0;
      }
      else
      {
        w = a1 * b1 + a2 * b2 + a3 * b3;

        w = w * fabs (w) - cosndevs * (a1 * a1 + a2 * a2 + a3 * a3) * bs;
      }
    }

    ++ibface;
  }
  while (ibface <= nbface && w >= dc0);

  if (w < dc0)
    return (-1);

 return (0);

}
