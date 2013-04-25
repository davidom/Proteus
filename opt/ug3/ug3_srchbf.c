#include "UG3_LIB.h"

INT_ ug3_srchbf
 (INT_ *ibface,
  INT_ id,
  INT_ isrch,
  INT_ mglobal,
  INT_ nbface,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * msrch,
  double smin,
  double smin2,
  double x01,
  double x02,
  double x03,
  double *s1,
  double *s2,
  double *s3,
  DOUBLE_3D * x)

{

/*
 * Find the boundary surface face that contains the node x01, x02, x03.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_srchbf.c,v 1.8 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface1, ibface2, ibface3, ierr, inode1, inode2, inode3, isave, jbface,
       kbface, mfound;

  double a0121, a0122, a0123, a0231, a0232, a0233, a0311, a0312, a0313, a1231,
         a1232, a1233, dc0, dc1, dx101, dx102, dx103, dx201, dx202, dx203,
         dx211, dx212, dx213, dx301, dx302, dx303, dx311, dx312, dx313, smin2m,
         smin3m, w, w0, w0j, w0k,  w1, w1j, w1k, w2, w2j, w2k, w3, w3j, w3k,
         wmin, wr, x11, x12, x13, x21, x22, x23, x31, x32, x33;

  dc0 = 0.0;
  dc1 = 1.0;

  isave = *ibface;

  jbface = *ibface;

  do
  {
    *ibface = jbface;

    msrch[*ibface] = isrch;

    inode1 = inibf[*ibface][0];
    inode2 = inibf[*ibface][1];
    inode3 = inibf[*ibface][2];

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

    dx101 = x11 - x01;
    dx102 = x12 - x02;
    dx103 = x13 - x03;
    dx201 = x21 - x01;
    dx202 = x22 - x02;
    dx203 = x23 - x03;
    dx301 = x31 - x01;
    dx302 = x32 - x02;
    dx303 = x33 - x03;

    a0121 = dx102 * dx203 - dx103 * dx202;
    a0122 = dx103 * dx201 - dx101 * dx203;
    a0123 = dx101 * dx202 - dx102 * dx201;
    a0231 = dx202 * dx303 - dx203 * dx302;
    a0232 = dx203 * dx301 - dx201 * dx303;
    a0233 = dx201 * dx302 - dx202 * dx301;
    a0311 = dx302 * dx103 - dx303 * dx102;
    a0312 = dx303 * dx101 - dx301 * dx103;
    a0313 = dx301 * dx102 - dx302 * dx101;

    w0 = a1231 * a1231 + a1232 * a1232 + a1233 * a1233;
    w1 = a1231 * a0231 + a1232 * a0232 + a1233 * a0233;
    w2 = a1231 * a0311 + a1232 * a0312 + a1233 * a0313;
    w3 = a1231 * a0121 + a1232 * a0122 + a1233 * a0123;

    wmin = MIN (w1, w2);
    wmin = MIN (wmin, w3);

    w = wmin + smin * w0;

    mfound = (w >= dc0) ? 1 : -2;

    if (mfound < -1 && w1 < dc0)
    {
      jbface = ibfibf[*ibface][0];

      mfound = (jbface <= 0) ? -3:
               (idibf != NULL && idibf[jbface] != id) ? -3:
               (msrch[jbface] == isrch) ? -2 : -1;
    }

    if (mfound < -1 && w2 < dc0)
    {
      jbface = ibfibf[*ibface][1];

      mfound = (jbface <= 0) ? -3:
               (idibf != NULL && idibf[jbface] != id) ? -3:
               (msrch[jbface] == isrch) ? -2 : -1;
    }

    if (mfound < -1 && w3 < dc0)
    {
      jbface = ibfibf[*ibface][2];

      mfound = (jbface <= 0) ? -3:
               (idibf != NULL && idibf[jbface] != id) ? -3:
               (msrch[jbface] == isrch) ? -2 : -1;
    }
  }
  while (mfound == -1);

  if (mfound == -3)
  {
    w = wmin + smin2 * w0;

    mfound = (w >= dc0) ? 1 : -2;
  }

  if (mglobal >= 1 && mfound == -2)
  {
    jbface = 0;
    kbface = 0;

    smin2m = smin2;
    smin3m = dc0;

    *ibface = 0;

    do
    {
      ++(*ibface);

      if (msrch[*ibface] != isrch && (idibf == NULL || idibf[*ibface] == id))
      {
        msrch[*ibface] = isrch;

        inode1 = inibf[*ibface][0];
        inode2 = inibf[*ibface][1];
        inode3 = inibf[*ibface][2];

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

        dx101 = x11 - x01;
        dx102 = x12 - x02;
        dx103 = x13 - x03;
        dx201 = x21 - x01;
        dx202 = x22 - x02;
        dx203 = x23 - x03;
        dx301 = x31 - x01;
        dx302 = x32 - x02;
        dx303 = x33 - x03;

        a0121 = dx102 * dx203 - dx103 * dx202;
        a0122 = dx103 * dx201 - dx101 * dx203;
        a0123 = dx101 * dx202 - dx102 * dx201;
        a0231 = dx202 * dx303 - dx203 * dx302;
        a0232 = dx203 * dx301 - dx201 * dx303;
        a0233 = dx201 * dx302 - dx202 * dx301;
        a0311 = dx302 * dx103 - dx303 * dx102;
        a0312 = dx303 * dx101 - dx301 * dx103;
        a0313 = dx301 * dx102 - dx302 * dx101;

        w0 = a1231 * a1231 + a1232 * a1232 + a1233 * a1233;
        w1 = a1231 * a0231 + a1232 * a0232 + a1233 * a0233;
        w2 = a1231 * a0311 + a1232 * a0312 + a1233 * a0313;
        w3 = a1231 * a0121 + a1232 * a0122 + a1233 * a0123;

        wmin = MIN (w1, w2);
        wmin = MIN (wmin, w3);

        w = wmin + smin * w0;

        if (w >= dc0)
          mfound = 1;

        else
        {
          if (smin2 > smin)
          {
            w = wmin + smin2 * w0;

            if (w >= dc0)
            {
              w = wmin + smin2m * w0;

              ibface1 = ibfibf[*ibface][0];
              ibface2 = ibfibf[*ibface][1];
              ibface3 = ibfibf[*ibface][2];

              if (((w1 == wmin && (ibface1 < 0 ||
                    (idibf != NULL && idibf[ibface1] != id))) ||
                   (w2 == wmin && (ibface2 < 0 ||
                    (idibf != NULL && idibf[ibface2] != id))) ||
                   (w3 == wmin && (ibface3 < 0 ||
                    (idibf != NULL && idibf[ibface3] != id)))) &&
                  w >= dc0)
              {
                jbface = *ibface;

                w0j = w0;
                w1j = w1;
                w2j = w2;
                w3j = w3;

                smin2m = fabs (wmin) / w0;
              }
            }
          }

          if (mglobal == 2 && wmin < dc0 && w < dc0)
          {
            w = (smin3m > dc0) ? wmin + smin3m * w0: dc0;

            ibface1 = ibfibf[*ibface][0];
            ibface2 = ibfibf[*ibface][1];
            ibface3 = ibfibf[*ibface][2];

            if (((w1 == wmin && (ibface1 < 0 ||
                  (idibf != NULL && idibf[ibface1] != id))) ||
                 (w2 == wmin && (ibface2 < 0 ||
                  (idibf != NULL && idibf[ibface2] != id))) ||
                 (w3 == wmin && (ibface3 < 0 ||
                  (idibf != NULL && idibf[ibface3] != id)))) &&
                w >= dc0)
            {
              kbface = *ibface;

              w0k = w0;
              w1k = w1;
              w2k = w2;
              w3k = w3;

              smin3m = fabs (wmin) / w0;
            }
          }
        }
      }
    }
    while (*ibface < nbface && mfound == -2);

    if (mfound == -2 && jbface > 0)
    {
      mfound = 1;

      *ibface = jbface;

      w0 = w0j;
      w1 = w1j;
      w2 = w2j;
      w3 = w3j;
    }

    else if (mfound == -2 && kbface > 0)
    {
      mfound = 2;

      *ibface = kbface;

      w0 = w0k;
      w1 = w1k;
      w2 = w2k;
      w3 = w3k;
    }
  }

  if (mfound >= 1)
  {
    wr = dc1 / w0;

    *s1 = wr * w1;
    *s2 = wr * w2;
    *s3 = wr * w3;
  }
  else
    *ibface = isave;

  ierr = (mfound == 1) ?  0:
         (mfound == 2) ? -1: 1;

  return (ierr);

}
