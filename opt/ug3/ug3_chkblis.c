#include "UG3_LIB.h"

INT_ ug3_chkblis
 (INT_ msetbc,
  INT_ nbface,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * ierribf,
  INT_3D * inibf,
  double angblisimx,
  DOUBLE_3D * x)

{

/*
 * Check included angle between intersecting boundary-layer surfaces and
 * adjacent surfaces.
 *  
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkblis.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ibc, ibfn1, ibfn2, ibfn3, ierr, inode1, inode2, inode3, inode4,
       jbc, jbface;

  double a1231, a1232, a1233, a2431, a2432, a2433, cosblisimx, cosblisimxs, dc0,
         dc1, dc45, dc180, dx211, dx212, dx213, dx311, dx312, dx313, dx321,
         dx322, dx323, dx421, dx422, dx423, w, x11, x12, x13, x21, x22, x23,
         x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;
  dc180 = 180.0;

  if (msetbc == 1 || angblisimx == dc0 || angblisimx >= dc180 || ibcibf == NULL)
    return (0);

  cosblisimx = - cos (angblisimx * atan (dc1) / dc45);

  cosblisimxs = cosblisimx * fabs (cosblisimx);

  ibface = 1;

  do
  {
    ibc = ibcibf[ibface];

    ++ibface;
  }
  while (ibface <= nbface && ! CHK_BL_INT_UG3_GBC (ibc));

  if (! CHK_BL_INT_UG3_GBC (ibc))
    return (0);

  ierr = 0;

  if (ierribf != NULL)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ierribf[ibface] = 0;
    }
  }

  ierr = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    if (CHK_BL_INT_UG3_GBC (ibc))
    {
      for (ibfn1 = 0; ibfn1 <= 2; ++ibfn1)
      {
        jbface = ibfibf[ibface][ibfn1];

        if (jbface > 0)
        {
          jbc = ibcibf[jbface];

          if (CHK_BL_UG3_GBC (jbc))
          {
            ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;
            ibfn3 = (ibfn2 < 2) ? ibfn2+1 : 0;

            inode1 = inibf[ibface][ibfn1];
            inode2 = inibf[ibface][ibfn2];
            inode3 = inibf[ibface][ibfn3];
            inode4 = (ibfibf[jbface][0] == ibface) ? inibf[jbface][0]:
                     (ibfibf[jbface][1] == ibface) ? inibf[jbface][1]:
                                                     inibf[jbface][2];

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
            dx421 = x41 - x21;
            dx422 = x42 - x22;
            dx423 = x43 - x23;

            a1231 = dx212 * dx313 - dx213 * dx312;
            a1232 = dx213 * dx311 - dx211 * dx313;
            a1233 = dx211 * dx312 - dx212 * dx311;

            w = a1231 * dx421 + a1232 * dx422 + a1233 * dx423;

            if (w >= dc0)
            {
              a2431 = dx422 * dx323 - dx423 * dx322;
              a2432 = dx423 * dx321 - dx421 * dx323;
              a2433 = dx421 * dx322 - dx422 * dx321;

              w = a1231 * a2431 + a1232 * a2432 + a1233 * a2433;

              w = cosblisimxs * (a1231 * a1231 + a1232 * a1232 + a1233 * a1233)
                              * (a2431 * a2431 + a2432 * a2432 + a2433 * a2433)
                - w * fabs (w);
            }

            if (w < dc0)
            {
              ierr = 1;

              if (ierribf == NULL)
                return (344);
              else
              {
                ierribf[ibface] = -1;
                ierribf[jbface] = -1;
              }
            }
          }
        }
      }
    }
  }

  if (ierr == 1)
    return (200304);

  return (0);

}
