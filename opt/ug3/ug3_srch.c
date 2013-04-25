#include "UG3_LIB.h"

INT_ ug3_srch
 (INT_ *ielem,
  INT_ isrch,
  INT_ mglobal,
  INT_ nelem,
  INT_4D * ieliel,
  INT_4D * iniel,
  INT_1D * msrch,
  double smin,
  double smin2,
  double x01,
  double x02,
  double x03,
  double *s1,
  double *s2,
  double *s3,
  double *s4,
  DOUBLE_3D * x)

{

/*
 * Find the element that contains the node x01,x02,x03.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_srch.c,v 1.18 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ierr, inode1, inode2, inode3, inode4, isave, jelem, kelem, mfound;

  double a1201, a1202, a1203, a3401, a3402, a3403, dc0, dc1, dx101, dx102,
         dx103, dx201, dx202, dx203, dx211, dx212, dx213, dx301, dx302, dx303,
         dx311, dx312, dx313, dx401, dx402, dx403, dx411, dx412, dx413, smin2m,
         smin3m, vol, vol1, vol1j, vol1k, vol2, vol2j, vol2k, vol3, vol3j,
         vol3k, vol4, vol4j, vol4k, volmin, volr, w, x11, x12, x13, x21, x22,
         x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;

  isave = *ielem;

  jelem = *ielem;

  do
  {
    *ielem = jelem;

    msrch[*ielem] = isrch;

    inode1 = iniel[*ielem][0];
    inode2 = iniel[*ielem][1];
    inode3 = iniel[*ielem][2];
    inode4 = iniel[*ielem][3];

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

    vol = dx211 * (dx312 * dx413 - dx313 * dx412)
        + dx212 * (dx313 * dx411 - dx311 * dx413)
        + dx213 * (dx311 * dx412 - dx312 * dx411);

    dx101 = x11 - x01;
    dx102 = x12 - x02;
    dx103 = x13 - x03;
    dx201 = x21 - x01;
    dx202 = x22 - x02;
    dx203 = x23 - x03;
    dx301 = x31 - x01;
    dx302 = x32 - x02;
    dx303 = x33 - x03;
    dx401 = x41 - x01;
    dx402 = x42 - x02;
    dx403 = x43 - x03;

    a1201 = dx102 * dx203 - dx103 * dx202;
    a1202 = dx103 * dx201 - dx101 * dx203;
    a1203 = dx101 * dx202 - dx102 * dx201;
    a3401 = dx302 * dx403 - dx303 * dx402;
    a3402 = dx303 * dx401 - dx301 * dx403;
    a3403 = dx301 * dx402 - dx302 * dx401;

    vol1 = dx201 * a3401 + dx202 * a3402 + dx203 * a3403;
    vol2 = -dx101 * a3401 - dx102 * a3402 - dx103 * a3403;
    vol3 = dx401 * a1201 + dx402 * a1202 + dx403 * a1203;
    vol4 = -dx301 * a1201 - dx302 * a1202 - dx303 * a1203;

    volmin = MIN (vol2, vol1);
    volmin = MIN (vol3, volmin);
    volmin = MIN (vol4, volmin);

    w = volmin + smin * vol;

    mfound = (w >= dc0) ? 1 : -2;

    if (mfound < -1 && vol1 < dc0)
    {
      jelem = ieliel[*ielem][0];

      mfound = (jelem > 0) ? ((msrch[jelem] != isrch) ? -1 : -2) : -3;
    }

    if (mfound < -1 && vol2 < dc0)
    {
      jelem = ieliel[*ielem][1];

      mfound = (jelem > 0) ? ((msrch[jelem] != isrch) ? -1 : -2) : -3;
    }

    if (mfound < -1 && vol3 < dc0)
    {
      jelem = ieliel[*ielem][2];

      mfound = (jelem > 0) ? ((msrch[jelem] != isrch) ? -1 : -2) : -3;
    }

    if (mfound < -1 && vol4 < dc0)
    {
      jelem = ieliel[*ielem][3];

      mfound = (jelem > 0) ? ((msrch[jelem] != isrch) ? -1 : -2) : -3;
    }
  }
  while (mfound == -1);

  if (mfound == -3)
  {
    w = volmin + smin2 * vol;

    mfound = (w >= dc0) ? 1 : -2;
  }

  if (mglobal >= 1 && mfound == -2)
  {
    jelem = 0;
    kelem = 0;

    smin2m = smin2;
    smin3m = dc0;

    *ielem = 0;

    do
    {
      ++(*ielem);

      if (msrch[*ielem] != isrch)
      {
        msrch[*ielem] = isrch;

        inode1 = iniel[*ielem][0];
        inode2 = iniel[*ielem][1];
        inode3 = iniel[*ielem][2];
        inode4 = iniel[*ielem][3];

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

        vol = dx211 * (dx312 * dx413 - dx313 * dx412)
            + dx212 * (dx313 * dx411 - dx311 * dx413)
            + dx213 * (dx311 * dx412 - dx312 * dx411);

        dx101 = x11 - x01;
        dx102 = x12 - x02;
        dx103 = x13 - x03;
        dx201 = x21 - x01;
        dx202 = x22 - x02;
        dx203 = x23 - x03;
        dx301 = x31 - x01;
        dx302 = x32 - x02;
        dx303 = x33 - x03;
        dx401 = x41 - x01;
        dx402 = x42 - x02;
        dx403 = x43 - x03;

        a1201 = dx102 * dx203 - dx103 * dx202;
        a1202 = dx103 * dx201 - dx101 * dx203;
        a1203 = dx101 * dx202 - dx102 * dx201;
        a3401 = dx302 * dx403 - dx303 * dx402;
        a3402 = dx303 * dx401 - dx301 * dx403;
        a3403 = dx301 * dx402 - dx302 * dx401;

        vol1 = dx201 * a3401 + dx202 * a3402 + dx203 * a3403;
        vol2 = -dx101 * a3401 - dx102 * a3402 - dx103 * a3403;
        vol3 = dx401 * a1201 + dx402 * a1202 + dx403 * a1203;
        vol4 = -dx301 * a1201 - dx302 * a1202 - dx303 * a1203;

        volmin = MIN (vol2, vol1);
        volmin = MIN (vol3, volmin);
        volmin = MIN (vol4, volmin);

        w = volmin + smin * vol;

        if (w >= dc0)
          mfound = 1;

        else
        {
          if (smin2 > smin)
          {
            w = volmin + smin2 * vol;

            if (w >= dc0)
            {
              w = volmin + smin2m * vol;

              if (((vol1 == volmin && ieliel[*ielem][0] < 0) ||
                   (vol2 == volmin && ieliel[*ielem][1] < 0) ||
                   (vol3 == volmin && ieliel[*ielem][2] < 0) ||
                   (vol4 == volmin && ieliel[*ielem][3] < 0)) && w >= dc0)
              {
                jelem = *ielem;

                vol1j = vol1;
                vol2j = vol2;
                vol3j = vol3;
                vol4j = vol4;

                smin2m = fabs (volmin) / vol;
              }
            }
          }

          if (mglobal == 2 && volmin < dc0 && w < dc0)
          {
            w = (smin3m > dc0) ? volmin + smin3m * vol: dc0;

            if (((vol1 == volmin && ieliel[*ielem][0] < 0) ||
                 (vol2 == volmin && ieliel[*ielem][1] < 0) ||
                 (vol3 == volmin && ieliel[*ielem][2] < 0) ||
                 (vol4 == volmin && ieliel[*ielem][3] < 0)) && w >= dc0)
            {
              kelem = *ielem;

              vol1k = vol1;
              vol2k = vol2;
              vol3k = vol3;
              vol4k = vol4;

              smin3m = fabs (volmin) / vol;
            }
          }
        }
      }
    }
    while (*ielem < nelem && mfound == -2);

    if (mfound == -2 && jelem > 0)
    {
      mfound = 1;

      *ielem = jelem;

      vol1 = vol1j;
      vol2 = vol2j;
      vol3 = vol3j;
      vol4 = vol4j;

      vol = vol1 + vol2 + vol3 + vol4;
    }

    else if (mfound == -2 && kelem > 0)
    {
      mfound = 2;

      *ielem = kelem;

      vol1 = vol1k;
      vol2 = vol2k;
      vol3 = vol3k;
      vol4 = vol4k;

      vol = vol1 + vol2 + vol3 + vol4;
    }
  }

  ierr = (mglobal == -1 && mfound == -3) ? -1:
         (mfound == 1) ? 0: 1;

  if (ierr <= 0)
  {
    volr = dc1 / vol;

    *s1 = volr * vol1;
    *s2 = volr * vol2;
    *s3 = volr * vol3;
    *s4 = volr * vol4;
  }
  else
    *ielem = isave;

  return (ierr);

}
