#include "UG3_LIB.h"

void ug3_chkbcp1
 (INT_ msetbc,
  INT_ nbface,
  INT_1D * ibcibf,
  INT_1D * idibf,
  INT_3D * inibf,
  double angbcpmax,
  DOUBLE_3D * x)

{

/*
 * Check to see that normal vectors are planar for all automatically set
 * intersecting boundary-layer surfaces.
 *  
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkbcp1.c,v 1.2 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ibc, id, idmax, idmin, ierr, inode1, inode2, inode3, jd;

  double a1231, a1232, a1233, b1, b2, b3, cosbcpmax, cosbcpmaxs, dc0, dc1, dc45,
         dx211, dx212, dx213, dx311, dx312, dx313, w, x11, x12, x13, x21, x22,
         x23, x31, x32, x33;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  if (msetbc != 1 || angbcpmax == dc0 || ibcibf == NULL || idibf == NULL)
    return;

  cosbcpmax = cos (angbcpmax * atan (dc1) / dc45);

  cosbcpmaxs = cosbcpmax * fabs (cosbcpmax);

  ibface = 1;

  do
  {
    ibc = ibcibf[ibface];

    ++ibface;
  }
  while (ibface <= nbface && ! CHK_BL_INT_UG3_GBC (ibc));

  if (! CHK_BL_INT_UG3_GBC (ibc))
    return;

  ierr = 0;

  idmax = idibf[1];
  idmin = idibf[1];

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    id = idibf[ibface];

    idmax = MAX (id, idmax);
    idmin = MIN (id, idmin);
  }

  for (jd = idmin; jd <= idmax; ++jd)
  {
    ierr = 0;

    ibc = 0;

    ibface = 1;

    do
    {
      id = idibf[ibface];

      if (id == jd)
        ibc = ibcibf[ibface];

      ++ibface;
    }
    while (ibface <= nbface && ! CHK_BL_INT_UG3_GBC (ibc));

    if (CHK_BL_INT_UG3_GBC (ibc))
    {
      a1231 = dc0;
      a1232 = dc0;
      a1233 = dc0;

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        id = idibf[ibface];

        ibc = ibcibf[ibface];

        if (id == jd && CHK_BL_INT_UG3_GBC (ibc))
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

          a1231 = a1231 + dx212 * dx313 - dx213 * dx312;
          a1232 = a1232 + dx213 * dx311 - dx211 * dx313;
          a1233 = a1233 + dx211 * dx312 - dx212 * dx311;
        }
      }

      w = dc1 / sqrt (a1231 * a1231 + a1232 * a1232 + a1233 * a1233);

      b1 = w * a1231;
      b2 = w * a1232;
      b3 = w * a1233;

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        id = idibf[ibface];

        ibc = ibcibf[ibface];

        if (id == jd && CHK_BL_INT_UG3_GBC (ibc))
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

          a1231 = dx212 * dx313 - dx213 * dx312;
          a1232 = dx213 * dx311 - dx211 * dx313;
          a1233 = dx211 * dx312 - dx212 * dx311;

          w = a1231 * b1 + a1232 * b2 + a1233 * b3;

          w = w * fabs (w)
            - cosbcpmaxs * (a1231 * a1231 + a1232 * a1232 + a1233 * a1233);

          if (w < dc0)
            ierr = 1;
        }
      }

      if (ierr == 1)
      {
        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          id = idibf[ibface];

          if (id == jd)
            ibcibf[ibface] = STD_UG3_GBC;
        }
      }
    }
  }

  return;

}
