#include "UG3_LIB.h"

INT_ ug3_qtria
 (INT_ mmiqibf,
  INT_ *nbface,
  INT_ *nquad,
  INT_1D ** ibcibf_ptr,
  INT_1D ** idibf_ptr,
  INT_1D ** ierribf_ptr,
  INT_3D ** inibf_ptr,
  INT_4D * iniq,
  INT_1D ** iqibf_ptr,
  INT_1D ** irfibf_ptr,
  DOUBLE_3D * x)

{

/*
 * Replace quad surface faces with tria surface faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_qtria.c,v 1.29 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ibcibf, *idibf, *iqibf, *irfibf, *nfpn;
  INT_3D *inibf;

  INT_ Error_Flag, ibc, ibface, id, inode, inode1, inode2, inode3, inode4,
       iquad, jbface, kquad, mibcibf, midibf, mierribf, miqibf, mirfibf,
       nbfacem, nfpn1, nfpn2, nfpn3, nfpn4, nfpnmin, nnodem;

  double a1231, a1232, a1233, a2341, a2342, a2343, a3411, a3412, a3413, a4121,
         a4122, a4123, dc0, dc1, dx211, dx212, dx213, dx321, dx322, dx323,
         dx411, dx412, dx413, dx431, dx432, dx433, w, w123, w1min, w1minm, w234,
         w2min, w2minm, w341, w412, wmin, x11, x12, x13, x21, x22, x23,
         x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;

  wmin = 0.001; // 90+/-1.8 deg  (wmin = 1/tan^2[max_angle])

  if (*nquad == 0)
    return (0);

  inibf = *inibf_ptr;

  nnodem = 0;

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    nnodem = MAX (nnodem, inode1);
    nnodem = MAX (nnodem, inode2);
    nnodem = MAX (nnodem, inode3);
  }

  for (iquad = 1; iquad <= *nquad; ++iquad)
  {
    inode1 = iniq[iquad][0];
    inode2 = iniq[iquad][1];
    inode3 = iniq[iquad][2];
    inode4 = iniq[iquad][3];

    nnodem = MAX (nnodem, inode1);
    nnodem = MAX (nnodem, inode2);
    nnodem = MAX (nnodem, inode3);
    nnodem = MAX (nnodem, inode4);
  }

  nbfacem = *nbface + (*nquad) + (*nquad);

  mibcibf = (*ibcibf_ptr == NULL) ? 0 : 1;
  midibf = (*idibf_ptr == NULL) ? 0 : 1;
  mierribf = (*ierribf_ptr == NULL) ? 0 : 1;
  miqibf = (*iqibf_ptr == NULL && mmiqibf != 1) ? 0 : 1;
  mirfibf = (*irfibf_ptr == NULL) ? 0 : 1;

  Error_Flag = 0;

  if (mibcibf == 1)
    *ibcibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *ibcibf_ptr,
                                         (nbfacem+1) * sizeof (INT_1D));
  if (midibf == 1)
    *idibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *idibf_ptr,
                                        (nbfacem+1) * sizeof (INT_1D));
  if (mierribf == 1)
    *ierribf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *ierribf_ptr,
                                          (nbfacem+1) * sizeof (INT_1D));
  if (miqibf == 1)
    *iqibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *iqibf_ptr,
                                        (nbfacem+1) * sizeof (INT_1D));
  if (mirfibf == 1)
    *irfibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *irfibf_ptr,
                                         (nbfacem+1) * sizeof (INT_1D));

  *inibf_ptr = (INT_3D *) ug_realloc (&Error_Flag, *inibf_ptr,
                                      (nbfacem+1) * sizeof (INT_3D));

  nfpn = (INT_1D *) ug_malloc (&Error_Flag, (nnodem+1) * sizeof (INT_1D));
                                         
  if (Error_Flag > 0)
    return (100300);

  ibcibf = *ibcibf_ptr;
  idibf = *idibf_ptr;
  iqibf = *iqibf_ptr;
  irfibf = *irfibf_ptr;

  inibf = *inibf_ptr;

  if (miqibf == 1)
  {
    for (ibface = 1; ibface <= nbfacem; ++ibface)
    {
      iqibf[ibface] = 0;
    }
  }

  if (mibcibf == 1)
  {
    kquad = 0;

    iquad = *nquad;

    do
    {
      if (ibcibf[*nbface+iquad] == FIXED_BL_INT_UG3_GBC)
        kquad = iquad+1;

      --iquad;
    }
    while (iquad >= 1 && kquad == 0);

    // move all surfaces that are not fixed and don't intersect the BL region to
    // the end of quad-face data

    if (kquad)
    {
      for (iquad = 1; iquad <= *nquad; ++iquad)
      {
        ibc = ibcibf[*nbface+iquad];

        if (iquad < kquad && ibc != FIXED_BL_INT_UG3_GBC)
        {
          do
          {
            --kquad;
          }
          while (iquad < kquad && ibcibf[*nbface+kquad] != FIXED_BL_INT_UG3_GBC);

          if (iquad < kquad)
          {
            ibcibf[*nbface+iquad] = ibcibf[*nbface+kquad];
            ibcibf[*nbface+kquad] = ibc;

            if (midibf == 1)
            {
              id = idibf[*nbface+iquad];

              idibf[*nbface+iquad] = idibf[*nbface+kquad];
              idibf[*nbface+kquad] = id;
            }

            if (mirfibf == 1)
            {
              irfibf[*nbface+iquad] = 0;
              irfibf[*nbface+kquad] = 0;
            }

            inode1 = iniq[iquad][0];
            inode2 = iniq[iquad][1];
            inode3 = iniq[iquad][2];
            inode4 = iniq[iquad][3];

            iniq[iquad][0] = iniq[kquad][0];
            iniq[iquad][1] = iniq[kquad][1];
            iniq[iquad][2] = iniq[kquad][2];
            iniq[iquad][3] = iniq[kquad][3];

            iniq[kquad][0] = inode1;
            iniq[kquad][1] = inode2;
            iniq[kquad][2] = inode3;
            iniq[kquad][3] = inode4;
          }
        }
      }
    }

    jbface = nbfacem + 1;

    for (ibface = *nbface+(*nquad); ibface >= *nbface+1; --ibface)
    {
      ibc = ibcibf[ibface];

      --jbface;

      ibcibf[jbface] = ibc;

      --jbface;

      ibcibf[jbface] = ibc;
    }
  }

  if (midibf == 1)
  {
    jbface = nbfacem + 1;

    for (ibface = *nbface+(*nquad); ibface >= *nbface+1; --ibface)
    {
      id = idibf[ibface];

      --jbface;

      idibf[jbface] = id;

      --jbface;

      idibf[jbface] = id;
    }
  }

  for (inode = 1; inode <= nnodem; ++inode)
  {
    nfpn[inode] = 0;
  }

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    ++(nfpn[inode1]);
    ++(nfpn[inode2]);
    ++(nfpn[inode3]);
  }

  for (iquad = 1; iquad <= *nquad; ++iquad)
  {
    inode1 = iniq[iquad][0];
    inode2 = iniq[iquad][1];
    inode3 = iniq[iquad][2];
    inode4 = iniq[iquad][3];

    ++(nfpn[inode1]);
    ++(nfpn[inode2]);
    ++(nfpn[inode3]);
    ++(nfpn[inode4]);
  }

  ibface = *nbface;

  for (iquad = 1; iquad <= *nquad; ++iquad)
  {
    inode1 = iniq[iquad][0];
    inode2 = iniq[iquad][1];
    inode3 = iniq[iquad][2];
    inode4 = iniq[iquad][3];

    nfpn1 = nfpn[inode1];
    nfpn2 = nfpn[inode2];
    nfpn3 = nfpn[inode3];
    nfpn4 = nfpn[inode4];

    nfpnmin = MIN (nfpn1, nfpn2);
    nfpnmin = MIN (nfpnmin, nfpn3);
    nfpnmin = MIN (nfpnmin, nfpn4);

    if (nfpnmin > 1)
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
      x41 = x[inode4][0];
      x42 = x[inode4][1];
      x43 = x[inode4][2];

      dx211 = x21 - x11;
      dx212 = x22 - x12;
      dx213 = x23 - x13;
      dx321 = x31 - x21;
      dx322 = x32 - x22;
      dx323 = x33 - x23;
      dx411 = x41 - x11;
      dx412 = x42 - x12;
      dx413 = x43 - x13;
      dx431 = x41 - x31;
      dx432 = x42 - x32;
      dx433 = x43 - x33;

      a1231 = dx212 * dx323 - dx213 * dx322;
      a1232 = dx213 * dx321 - dx211 * dx323;
      a1233 = dx211 * dx322 - dx212 * dx321;

      a3411 = dx412 * dx433 - dx413 * dx432;
      a3412 = dx413 * dx431 - dx411 * dx433;
      a3413 = dx411 * dx432 - dx412 * dx431;

      a2341 = dx322 * dx433 - dx323 * dx432;
      a2342 = dx323 * dx431 - dx321 * dx433;
      a2343 = dx321 * dx432 - dx322 * dx431;

      a4121 = dx212 * dx413 - dx213 * dx412;
      a4122 = dx213 * dx411 - dx211 * dx413;
      a4123 = dx211 * dx412 - dx212 * dx411;

      w123 = - dx321 * dx211 - dx322 * dx212 - dx323 * dx213;
      w341 =   dx411 * dx431 + dx412 * dx432 + dx413 * dx433;
      w234 = - dx431 * dx321 - dx432 * dx322 - dx433 * dx323;
      w412 =   dx211 * dx411 + dx212 * dx412 + dx213 * dx413;

      w123 = w123 * fabs (w123) / (a1231 * a1231 + a1232 * a1232 + a1233 * a1233);
      w341 = w341 * fabs (w341) / (a3411 * a3411 + a3412 * a3412 + a3413 * a3413);
      w234 = w234 * fabs (w234) / (a2341 * a2341 + a2342 * a2342 + a2343 * a2343);
      w412 = w412 * fabs (w412) / (a4121 * a4121 + a4122 * a4122 + a4123 * a4123);

      w1min = MIN (w123, w341);  
      w2min = MIN (w234, w412);

      w1minm = fabs (w1min);
      w2minm = fabs (w2min);

      w = MAX (w1minm, w2minm);
    }
    else
    {
      w = dc1;

      if (nfpn1 == 1 || nfpn3 == 1)
      {
        w1min = dc1;
        w2min = dc0;
      }
      else
      {
        w1min = dc0;
        w2min = dc1;
      }
    }

    if (w1min >= w2min || w <= wmin)
    {
      ++ibface;

      inibf[ibface][0] = inode1;
      inibf[ibface][1] = inode2;
      inibf[ibface][2] = inode3;

      ++ibface;

      inibf[ibface][0] = inode3;
      inibf[ibface][1] = inode4;
      inibf[ibface][2] = inode1;

      if (miqibf == 1)
      {
        iqibf[ibface-1] = iquad;
        iqibf[ibface] = iquad;
      }

      if (mirfibf == 1)
      {
        irfibf[ibface-1] = 5;
        irfibf[ibface] = 5;
      }
    }

    else
    {
      ++ibface;

      inibf[ibface][0] = inode2;
      inibf[ibface][1] = inode3;
      inibf[ibface][2] = inode4;

      ++ibface;

      inibf[ibface][0] = inode4;
      inibf[ibface][1] = inode1;
      inibf[ibface][2] = inode2;

      if (miqibf == 1)
      {
        iqibf[ibface-1] = iquad;
        iqibf[ibface] = iquad;
      }

      if (mirfibf == 1)
      {
        irfibf[ibface-1] = 5;
        irfibf[ibface] = 5;
      }
    }
  }

  *nbface = nbfacem;

  *nquad = 0;

  ug_free (nfpn);

  return (0);

}
