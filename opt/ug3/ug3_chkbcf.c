#include "UG3_LIB.h"

void ug3_chkbcf
 (INT_ nbface,
  INT_1D * ibcibf,
  INT_3D * inibf,
  double cdff,
  DOUBLE_3D * x,
  DOUBLE_1D * df)

{

/*
 * Check frozen boundary surface faces to see if they are actually satisfied
 * boundary surface faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkbcf.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, inode1, inode2, inode3;

  double c1, dc0, dc1d4, df0, df1, df2, df3, dx1, dx2, w, x11, x12, x21, x22,
         x31, x32;

  dc0 = 0.0;
  dc1d4 = 0.25;

  if (cdff > dc0)
  {
    c1 = dc1d4 * cdff * cdff;

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      if (ibcibf[ibface] == PART_UG3_GBC)
      {
        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        df1 = df[inode1];
        df2 = df[inode2];
        df3 = df[inode3];

        x11 = x[inode1][0];
        x12 = x[inode1][1];
        x21 = x[inode2][0];
        x22 = x[inode2][1];
        x31 = x[inode3][0];
        x32 = x[inode3][1];

        dx1 = x21 - x11;
        dx2 = x22 - x12;

        df0 = df1 + df2;

        w = dx1 * dx1 + dx2 * dx2 - c1 * df0 * df0;

        if (w <= dc0)
        {
          dx1 = x31 - x11;
          dx2 = x32 - x12;

          df0 = df1 + df3;

          w = dx1 * dx1 + dx2 * dx2 - c1 * df0 * df0;

          if (w <= dc0)
          {
            dx1 = x31 - x21;
            dx2 = x32 - x22;

            df0 = df2 + df3;

            w = dx1 * dx1 + dx2 * dx2 - c1 * df0 * df0;

            if (w <= dc0)
              ibcibf[ibface] = PART_STD_UG3_GBC;
          }
        }
      }
    }
  }
  else
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      if (ibcibf[ibface] == PART_UG3_GBC)
        ibcibf[ibface] = PART_STD_UG3_GBC;
    }
  }

  return;

}
