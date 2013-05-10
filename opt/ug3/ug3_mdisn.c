#include "UG3_LIB.h"

void ug3_mdisn
 (INT_ mdis,
  INT_ nnode,
  INT_ nbface,
  INT_3D * inibf,
  INT_3D * ibfibf,
  INT_1D * mdisn,
  double angdisn,
  DOUBLE_3D * x)

{

/*
 * Set a flag at discontinuous boundary nodes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_mdisn.c,v 1.9 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ibfn1, ibfn2, ibfn3, inode, inode1, inode2, inode3, inode4,
       jbface, mdisn_;

  double ai1, ai2, ai3, ais, aj1, aj2, aj3, ajs, cosmin, cosmins, dc0, dc1,
         dc45, dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412, dx413, w,
         x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  cosmin = cos (angdisn * atan (dc1) / dc45);
  cosmins = cosmin * fabs (cosmin);

  for (inode = 1; inode <= nnode; ++inode)
  {
    mdisn[inode] = 0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    for (ibfn1 = 0; ibfn1 <= 2; ++ibfn1)
    {
      ibfn2 = (ibfn1 == 2) ? 0 : ibfn1 + 1;
      ibfn3 = (ibfn2 == 2) ? 0 : ibfn2 + 1;

      inode1 = inibf[ibface][ibfn1];
      inode2 = inibf[ibface][ibfn2];
      inode3 = inibf[ibface][ibfn3];

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

      ai1 = dx312 * dx213 - dx313 * dx212;
      ai2 = dx313 * dx211 - dx311 * dx213;
      ai3 = dx311 * dx212 - dx312 * dx211;
      ais = ai1 * ai1 + ai2 * ai2 + ai3 * ai3;

      jbface = ibfibf[ibface][ibfn2];

      if (jbface > 0)
      {
        inode4 = (ibfibf[jbface][0] == ibface) ? inibf[jbface][0] :
                 (ibfibf[jbface][1] == ibface) ? inibf[jbface][1] :
                                                 inibf[jbface][2];

        x41 = x[inode4][0];
        x42 = x[inode4][1];
        x43 = x[inode4][2];

        dx411 = x41 - x11;
        dx412 = x42 - x12;
        dx413 = x43 - x13;

        aj1 = dx412 * dx313 - dx413 * dx312;
        aj2 = dx413 * dx311 - dx411 * dx313;
        aj3 = dx411 * dx312 - dx412 * dx311;
        ajs = aj1 * aj1 + aj2 * aj2 + aj3 * aj3;

        w = ai1 * aj1 + ai2 * aj2 + ai3 * aj3;
        w = w * fabs (w) - cosmins * ais * ajs;

        if (w < dc0)
        {
          w = (mdis == 1) ? - dx411 * ai1 - dx412 * ai2 - dx413 * ai3 :
              (mdis == 2) ?   dx411 * ai1 + dx412 * ai2 + dx413 * ai3 : -dc1;

          if (w < dc0)
          {
            mdisn[inode1] = mdisn[inode1] + 1;
            mdisn[inode3] = mdisn[inode3] + 1;
          }
        }
      }
    }
  }
 
  for (inode = 1; inode <= nnode; ++inode)
  {
    mdisn_ = mdisn[inode];

    mdisn[inode] = (mdisn_ <= 3) ? 0 :
                   (mdisn_ == 4) ? 1 : 2;
  }
 
  return;

}
