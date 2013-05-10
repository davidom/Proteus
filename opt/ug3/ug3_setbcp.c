#include "UG3_LIB.h"

void ug3_setbcp
 (INT_ msetbc,
  INT_ nbface,
  INT_ nsetbcp,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * ibcpibf,
  INT_1D * ibfichk,
  double angbcpmax,
  DOUBLE_3D * x)

{

/*
 * Check for planar surfaces that are not BL generating surfaces and
 * automatically set the grid boundary condition flag for them to that for an
 * intersecting surface. A planar surface is defined as a set of connected
 * faces with the same surface normal vector, a total number of faces greater
 * than or equal to nsetbcp, and at least one outer edge that is adjacent to a
 * BL generating face.
 *  
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_setbcp.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibe, ibcp, ibface, ibfit, ichk, id, inode1, inode2, inode3, jbc,
       jbface, jbcp, jd, kbface, mfound, nchk;

  double a1, a2, a3, b1, b2, b3, cw, cwb, dc0, dc1, dc45, dx211, dx212, dx213,
         dx311, dx312, dx313, w, x11, x12, x13, x21, x22, x23, x31, x32, x33;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  if (angbcpmax == dc0 || msetbc == 0 || ibcibf == NULL || idibf == NULL)
    return;

  cw = cos (angbcpmax * atan (dc1) / dc45);
  cw = cw * fabs (cw);

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    if (CHK_STD_UG3_GBC (ibc))
      ibcpibf[ibface] = 0;
    else
      ibcpibf[ibface] = -2;
  }

  mfound = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    if (ibc < 0)
    {
      for (ibe = 0; ibe <= 2; ++ibe)
      {
        jbface = ibfibf[ibface][ibe];

        jbc = ibcibf[jbface];

        if (CHK_STD_UG3_GBC (jbc))
        {
          mfound = 1;

          ibcpibf[jbface] = -1;
        }
      }
    }
  }

  if (mfound == 1)
  {
    ibfit = 1;

    do
    {
      ibface = 0;

      do
      {
        ++ibface;

        ibcp = ibcpibf[ibface];
      }
      while (ibface < nbface && ibcp != -1);

      if (ibcp == -1)
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

        b1 = dx212 * dx313 - dx213 * dx312;
        b2 = dx213 * dx311 - dx211 * dx313;
        b3 = dx211 * dx312 - dx212 * dx311;

        cwb = cw * (b1 * b1 + b2 * b2 + b3 * b3);

        ichk = 1;

        nchk = 1;

        ibfichk[1] = ibface;

        kbface = 1;

        ibcpibf[ibface] = 1;

        do
        {
          ibface = ibfichk[ichk];

          id = idibf[ibface];

          for (ibe = 0; ibe <= 2; ++ibe)
          {
            jbface = ibfibf[ibface][ibe];

            jbcp = ibcpibf[jbface];

            jd = idibf[jbface];

            if (jd == id && (jbcp == 0 || jbcp == -1))
            {
              inode1 = inibf[jbface][0];
              inode2 = inibf[jbface][1];
              inode3 = inibf[jbface][2];

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

              w = a1 * b1 + a2 * b2 + a3 * b3;

              w = w * fabs (w) - cwb * (a1 * a1 + a2 * a2 + a3 * a3);

              if (w >= dc0)
              {
                ++nchk;

                ibfichk[nchk] = jbface;

                ++kbface;

                ibcpibf[jbface] = 1;
              }
            }
          }

          ++ichk;
        }
        while (ichk <= nchk);

        ibc = (kbface >= nsetbcp) ? BL_INT_UG3_GBC: STD_UG3_GBC;

        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          if (ibcpibf[ibface] == 1)
          {
            ibcibf[ibface] = ibc;

            ibcpibf[ibface] = -2;
          }
        }
      }

      ++ibfit;
    }
    while (ibfit <= nbface && ibcp == -1);
  }

  return;

}
