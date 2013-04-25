#include "UG3_LIB.h"

void ug3_set_irfibf
 (INT_ nbface,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * irfibf,
  double angrbfdd,
  double angrbfmxp,
  DOUBLE_3D * x)

{

/*
 * Set reconnection flag based on dihedral angle between adjacent triangular
 * faces of surface grid and turn reconnection off between faces with different
 * surface ID flags.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_set_irfibf.c,v 1.14 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  INT_ irf[3], jrf[3];

  INT_ ibface, ibfn, id, inode1, inode2, inode3, inode4, irf0, jbface, jbfn1,
       jbfn2, jbfn3, jd, jrf0, mrbfmxp;

  double a1231, a1232, a1233, a123s, a1241, a1242, a1243, a124s, a1431, a1432,
         a1433, a143s, a2431, a2432, a2433, a243s, cotmin, cotmins, cosmax,
         cosmaxs, dc0, dc1, dc45, dc180, dx211, dx212, dx213, dx311, dx312,
         dx313, dx321, dx322, dx323, dx411, dx412, dx413, dx421, dx422, dx423,
         dx431, dx432, dx433, w, w1, w2, w1231, w1232, w1233, w1241, w1242,
         w1244, w1431, w1433, w1434, w2432, w2433, w2434, x11, x12, x13, x21,
         x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;
  dc180 = 180.0;

  if (irfibf == NULL) return;
  
  if (angrbfmxp > dc0 && angrbfmxp < dc180)
  {
    mrbfmxp = 1;

    cotmin = cos (angrbfmxp * atan (dc1) / dc45)
           / sin (angrbfmxp * atan (dc1) / dc45);

    cotmins = cotmin * fabs (cotmin);
  }
  else
    mrbfmxp = 0;

  cosmax = - cos (angrbfdd * atan (dc1) / dc45);

  cosmaxs = cosmax * fabs (cosmax);

  id = 1;
  jd = 1;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    irf0 = irfibf[ibface];

    if (irf0 < 7)
    {
      if (idibf != NULL) id = idibf[ibface];

      irf[2] = irf0 / 4;
      irf0 = irf0 - 4 * irf[2];
      irf[1] = irf0 / 2;
      irf[0] = irf0 - 2 * irf[1];

      for (ibfn = 0; ibfn <= 2; ++ibfn)
      {
        if (irf[ibfn] == 0)
        {
          jbface = ibfibf[ibface][ibfn];

          if (jbface > 0)
          {
            jbfn1 = (ibfibf[jbface][0] == ibface) ? 0:
                    (ibfibf[jbface][1] == ibface) ? 1: 2;
            jbfn2 = (jbfn1 < 2) ? jbfn1+1: 0;
            jbfn3 = (jbfn2 < 2) ? jbfn2+1: 0;

            jrf0 = irfibf[jbface];

            jrf[2] = jrf0 / 4;
            jrf0 = jrf0 - 4 * jrf[2];
            jrf[1] = jrf0 / 2;
            jrf[0] = jrf0 - 2 * jrf[1];

            if (jrf[jbfn1] == 0)
            {
              if (idibf != NULL) jd = idibf[jbface];

              if (id == jd)
              {
                inode1 = inibf[jbface][jbfn1];
                inode2 = inibf[jbface][jbfn2];
                inode3 = inibf[jbface][jbfn3];
                inode4 = inibf[ibface][ibfn];

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

                a1231 = dx212 * dx313 - dx213 * dx312;
                a1232 = dx213 * dx311 - dx211 * dx313;
                a1233 = dx211 * dx312 - dx212 * dx311;
                a2431 = dx422 * dx323 - dx423 * dx322;
                a2432 = dx423 * dx321 - dx421 * dx323;
                a2433 = dx421 * dx322 - dx422 * dx321;

                a123s = a1231 * a1231 + a1232 * a1232 + a1233 * a1233;
                a243s = a2431 * a2431 + a2432 * a2432 + a2433 * a2433;

                w = a2431 * a1231 + a2432 * a1232 + a2433 * a1233;
                w = w * fabs (w) - cosmaxs * a243s * a123s;

                if (w >= dc0)
                {
                  a1241 = dx212 * dx413 - dx213 * dx412;
                  a1242 = dx213 * dx411 - dx211 * dx413;
                  a1243 = dx211 * dx412 - dx212 * dx411;
                  a1431 = dx412 * dx313 - dx413 * dx312;
                  a1432 = dx413 * dx311 - dx411 * dx313;
                  a1433 = dx411 * dx312 - dx412 * dx311;

                  a124s = a1241 * a1241 + a1242 * a1242 + a1243 * a1243;
                  a143s = a1431 * a1431 + a1432 * a1432 + a1433 * a1433;

                  w = a1241 * a1431 + a1242 * a1432 + a1243 * a1433;
                  w = w * fabs (w) - cosmaxs * a124s * a143s;

                  if (mrbfmxp && w >= dc0)
                  {
                    w1231 =   dx211 * dx311 + dx212 * dx312 + dx213 * dx313;
                    w1232 = - dx321 * dx211 - dx322 * dx212 - dx323 * dx213;
                    w1233 =   dx311 * dx321 + dx312 * dx322 + dx313 * dx323;
                    w2432 =   dx421 * dx321 + dx422 * dx322 + dx423 * dx323;
                    w2433 = - dx321 * dx431 - dx322 * dx432 - dx323 * dx433;
                    w2434 =   dx431 * dx421 + dx432 * dx422 + dx433 * dx423;
                    w1241 =   dx211 * dx411 + dx212 * dx412 + dx213 * dx413;
                    w1242 = - dx421 * dx211 - dx422 * dx212 - dx423 * dx213;
                    w1244 =   dx411 * dx421 + dx412 * dx422 + dx413 * dx423;
                    w1431 =   dx411 * dx311 + dx412 * dx312 + dx413 * dx313;
                    w1433 = - dx311 * dx431 - dx312 * dx432 - dx313 * dx433;
                    w1434 =   dx431 * dx411 + dx432 * dx412 + dx433 * dx413;

                    w = MIN (w1231, w1232); 
                    w = MIN (w1233, w); 
                    w = w * fabs (w) / a123s;

                    w1 = w;

                    w = MIN (w2432, w2433);
                    w = MIN (w2434, w);
                    w = w * fabs (w) / a243s;

                    w1 = MIN (w1, w); 

                    w = MIN (w1241, w1242);
                    w = MIN (w1244, w);
                    w = w * fabs (w) / a124s;

                    w2 = w;

                    w = MIN (w1431, w1433);
                    w = MIN (w1434, w);
                    w = w * fabs (w) / a143s;

                    w2 = MIN (w2, w); 

                    w = w2 - w1;

                    if (w < dc0) w = w2 - cotmins;
                  }
                }

                if (w < dc0)
                {
                  irf[ibfn] = 1;

                  jrf[jbfn1] = 1;

                  irfibf[jbface] = jrf[0] + 2 * jrf[1] + 4 * jrf[2];
                }
              }
              else
              {
                irf[ibfn] = 1;

                jrf[jbfn1] = 1;

                irfibf[jbface] = jrf[0] + 2 * jrf[1] + 4 * jrf[2];
              }
            }
            else
              irf[ibfn] = 1;
          }
          else
            irf[ibfn] = 1;
        }
      }

      irfibf[ibface] = irf[0] + 2 * irf[1] + 4 * irf[2];
    }
  }

  return;
}
