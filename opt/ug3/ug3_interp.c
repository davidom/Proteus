#include "UG3_LIB.h"

INT_ ug3_interp
 (INT_ mmsg,
  INT_ nelem,
  INT_ nelemn,
  INT_ nnode,
  INT_ nnoden,
  INT_ nscalar,
  INT_ nvector,
  INT_4D * ieliel,
  INT_4D * iniel,
  INT_4D * inieln,
  INT_1D * iw,
  INT_1D * msrch,
  double smin,
  double smin2,
  DOUBLE_1D * u_scalar,
  DOUBLE_1D * u_scalarn,
  DOUBLE_3D * u_vector,
  DOUBLE_3D * u_vectorn,
  DOUBLE_3D * x,
  DOUBLE_3D * xn)

{

/*
 * Interpolate data at new locations from existing data and grid.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_interp.c,v 1.21 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, ierr, iit, inode, inode1, inode2, inode3, inode4, ipass, iscalar,
       isrch, ivector, iw1, iw2, iw3, iw4, iwi, iwrite, jelem, jw1, jw2, jw3,
       jw4, jwsum, knode, knodei, kw1, kw2, kw3, kw4, method, mglobal, loc,
       locn, nit, npass, nsrch;

  double cjw1, cjw2, cjw3, cjw4, ckw1, ckw2, ckw3, ckw4, dc0, dc1, dc10, s1, s2,
         s3, s4, smin2m, u1, u11, u12, u13, u1sum, u2, u21, u22, u23, u2sum, u3,
         u31, u32, u33, u3sum, u4, u41, u42, u43, usum, w, x01, x02, x03;

  dc0 = 0.0;
  dc1 = 1.0;
  dc10 = 10.0;

  if (mmsg == 2)
  {
    ug_message (" ");
    ug_message ("UG3      : Interpolating Data");
    ug_message (" ");
  }

  nit = nelemn;

  npass = nnoden;

  w = log10 ((double) nnoden) - dc1;
  w = MAX (w, dc0);
  w = pow (dc10, w);

  iwrite = 2 * NINT (w);

  for (inode = 1; inode <= nnoden; ++inode)
  {
    iw[inode] = 0;
  }

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    msrch[ielem] = 0;
  }

  ielem = 1;

  knode = 0;

  method = 1;

  mglobal = 1;

  nsrch = 0;

  smin2m = smin;

  ipass = 0;

  do
  {
    ++ipass;

    knodei = knode;

    if (method >= 2)
    {
      for (inode = 1; inode <= nnoden; ++inode)
      {
        if (iw[inode] < 0)
          iw[inode] = 0;
      }

      iit = 0;

      do
      {
        for (ielem = 1; ielem <= nelemn; ++ielem)
        {
          inode1 = inieln[ielem][0];
          inode2 = inieln[ielem][1];
          inode3 = inieln[ielem][2];
          inode4 = inieln[ielem][3];

          iw1 = abs (iw[inode1]);
          iw2 = abs (iw[inode2]);
          iw3 = abs (iw[inode3]);
          iw4 = abs (iw[inode4]);

          jelem = MAX (iw1, iw2);
          jelem = MAX (jelem, iw3);
          jelem = MAX (jelem, iw4);

          if (iw1 == 0) iw[inode1] = -jelem;
          if (iw2 == 0) iw[inode2] = -jelem;
          if (iw3 == 0) iw[inode3] = -jelem;
          if (iw4 == 0) iw[inode4] = -jelem;
        }

        inode = 1;

        do
        {
          iwi = iw[inode];

          ++inode;
        }
        while (inode <= nnoden && iwi != 0);

        ++iit;
      }
      while (iit < nit && iwi == 0);
    }

    for (inode = 1; inode <= nnoden; ++inode)
    {
      if (method >= 2)
        ielem = -iw[inode];

      if (ielem > 0)
      {
        isrch = inode + nsrch;

        x01 = xn[inode][0];
        x02 = xn[inode][1];
        x03 = xn[inode][2];

        ierr = ug3_srch (&ielem, isrch, mglobal, nelem, ieliel, iniel, msrch,
                         smin, smin2m, x01, x02, x03, &s1, &s2, &s3, &s4, x);

        if (ierr == 0)
        {
          ++knode;

          if (method == 1)
            mglobal = 0;

          iw[inode] = ielem;

          inode1 = iniel[ielem][0];
          inode2 = iniel[ielem][1];
          inode3 = iniel[ielem][2];
          inode4 = iniel[ielem][3];

          loc = 0;

          locn = 0;

          for (iscalar = 1; iscalar <= nscalar; ++iscalar)
          {
            u_scalarn[inode+locn] = s1 * u_scalar[inode1+loc]
                                  + s2 * u_scalar[inode2+loc]
                                  + s3 * u_scalar[inode3+loc]
                                  + s4 * u_scalar[inode4+loc];

            loc = loc + nnode + 1;

            locn = locn + nnoden + 1;
          }

          loc = 0;

          locn = 0;

          for (ivector = 1; ivector <= nvector; ++ivector)
          {
            u_vectorn[inode+locn][0] = s1 * u_vector[inode1+loc][0]
                                     + s2 * u_vector[inode2+loc][0]
                                     + s3 * u_vector[inode3+loc][0]
                                     + s4 * u_vector[inode4+loc][0];
            u_vectorn[inode+locn][1] = s1 * u_vector[inode1+loc][1]
                                     + s2 * u_vector[inode2+loc][1]
                                     + s3 * u_vector[inode3+loc][1]
                                     + s4 * u_vector[inode4+loc][1];
            u_vectorn[inode+locn][2] = s1 * u_vector[inode1+loc][2]
                                     + s2 * u_vector[inode2+loc][2]
                                     + s3 * u_vector[inode3+loc][2]
                                     + s4 * u_vector[inode4+loc][2];

            loc = loc + nnode + 1;

            locn = locn + nnoden + 1;
          }

          if (mmsg == 2 && knode % iwrite == 0)
          {
            sprintf (Text, "UG3      : Pass, Nodes       =%10i%10i",
                     ipass, knode);
            ug_message (Text);
          }
        }
        else
        {
          iw[inode] = -ielem;

          if (method == 1)
          {
            locn = 0;

            for (iscalar = 1; iscalar <= nscalar; ++iscalar)
            {
              u_scalarn[inode+locn] = dc0;

              locn = locn + nnoden + 1;
            }

            locn = 0;

            for (ivector = 1; ivector <= nvector; ++ivector)
            {
              u_vectorn[inode+locn][0] = dc0;
              u_vectorn[inode+locn][1] = dc0;
              u_vectorn[inode+locn][2] = dc0;

              locn = locn + nnoden + 1;
            }
          }
        }
      }
    }

    if (knode < nnoden && (method == 1 || knode == knodei))
    {
      ++method;

      ipass = 0;

      nsrch = nsrch + nnoden;

      if (method == 3)
        smin2m = smin2;

      else if (method == 4)
        mglobal = 1;

      else if (method == 5)
        mglobal = 2;

      if (mmsg == 2)
      {
        sprintf (Text, "UG3      : Pass, Nodes       =%10i%10i", ipass, knode);
        ug_message (Text);

        ug_message (" ");

        if (method == 2)
          ug_message ("UG3      : Interpolating Data With Local Starting Search");
        else if (method == 3)
          ug_message ("UG3      : Interpolating Data With Boundary Tolerance");
        else if (method == 4)
          ug_message ("UG3      : Interpolating Data With Boundary Tolerance & Global Search");
        else if (method == 5)
          ug_message ("UG3      : Interpolating Data With Boundary Tolerance & Global/Boundary Search");

        ug_message (" ");
      }
    }
  }
  while (ipass < npass && method <= 5 && knode < nnoden);

  if (knode < nnoden)
  {
    if (mmsg == 2)
    {
      ug_message (" ");
      ug_message ("UG3      : Averaging Nearby Data");
      ug_message (" ");
    }

    for (inode = 1; inode <= nnoden; ++inode)
    {
      iw[inode] = MAX (MIN (iw[inode], 1), 0);
    }

    ipass = 0;

    do
    {
      ++ipass;

      for (ielem = 1; ielem <= nelemn; ++ielem)
      {
        inode1 = inieln[ielem][0];
        inode2 = inieln[ielem][1];
        inode3 = inieln[ielem][2];
        inode4 = inieln[ielem][3];

        iw1 = iw[inode1];
        iw2 = iw[inode2];
        iw3 = iw[inode3];
        iw4 = iw[inode4];

        iwi = MIN (iw1, iw2);
        iwi = MIN (iwi, iw3);
        iwi = MIN (iwi, iw4);

        jw1 = MAX (iw1, 0);
        jw2 = MAX (iw2, 0);
        jw3 = MAX (iw3, 0);
        jw4 = MAX (iw4, 0);

        jwsum = jw1 + jw2 + jw3 + jw4;

        if (iwi <= 0 && jwsum > 0)
        {
          kw1 = 1 - jw1;
          kw2 = 1 - jw2;
          kw3 = 1 - jw3;
          kw4 = 1 - jw4;

          iw[inode1] = iw1 - kw1 * jwsum;
          iw[inode2] = iw2 - kw2 * jwsum;
          iw[inode3] = iw3 - kw3 * jwsum;
          iw[inode4] = iw4 - kw4 * jwsum;

          cjw1 = (double) jw1;
          cjw2 = (double) jw2;
          cjw3 = (double) jw3;
          cjw4 = (double) jw4;

          ckw1 = (double) kw1;
          ckw2 = (double) kw2;
          ckw3 = (double) kw3;
          ckw4 = (double) kw4;

          locn = 0;

          for (iscalar = 1; iscalar <= nscalar; ++iscalar)
          {
            u1 = u_scalarn[inode1+locn];
            u2 = u_scalarn[inode2+locn];
            u3 = u_scalarn[inode3+locn];
            u4 = u_scalarn[inode4+locn];

            usum = cjw1 * u1 + cjw2 * u2 + cjw3 * u3 + cjw4 * u4;

            u_scalarn[inode1+locn] = u1 + ckw1 * usum;
            u_scalarn[inode2+locn] = u2 + ckw2 * usum;
            u_scalarn[inode3+locn] = u3 + ckw3 * usum;
            u_scalarn[inode4+locn] = u4 + ckw4 * usum;

            locn = locn + nnoden + 1;
          }

          locn = 0;

          for (ivector = 1; ivector <= nvector; ++ivector)
          {
            u11 = u_vectorn[inode1+locn][0];
            u12 = u_vectorn[inode1+locn][1];
            u13 = u_vectorn[inode1+locn][2];
            u21 = u_vectorn[inode2+locn][0];
            u22 = u_vectorn[inode2+locn][1];
            u23 = u_vectorn[inode2+locn][2];
            u31 = u_vectorn[inode3+locn][0];
            u32 = u_vectorn[inode3+locn][1];
            u33 = u_vectorn[inode3+locn][2];
            u41 = u_vectorn[inode4+locn][0];
            u42 = u_vectorn[inode4+locn][1];
            u43 = u_vectorn[inode4+locn][2];

            u1sum = cjw1 * u11 + cjw2 * u21 + cjw3 * u31 + cjw4 * u41 ;
            u2sum = cjw1 * u12 + cjw2 * u22 + cjw3 * u32 + cjw4 * u42 ;
            u3sum = cjw1 * u13 + cjw2 * u23 + cjw3 * u33 + cjw4 * u43 ;

            u_vectorn[inode1+locn][0] = u11 + ckw1 * u1sum;
            u_vectorn[inode1+locn][1] = u12 + ckw1 * u2sum;
            u_vectorn[inode1+locn][2] = u13 + ckw1 * u3sum;
            u_vectorn[inode2+locn][0] = u21 + ckw2 * u1sum;
            u_vectorn[inode2+locn][1] = u22 + ckw2 * u2sum;
            u_vectorn[inode2+locn][2] = u23 + ckw2 * u3sum;
            u_vectorn[inode3+locn][0] = u31 + ckw3 * u1sum;
            u_vectorn[inode3+locn][1] = u32 + ckw3 * u2sum;
            u_vectorn[inode3+locn][2] = u33 + ckw3 * u3sum;
            u_vectorn[inode4+locn][0] = u41 + ckw4 * u1sum;
            u_vectorn[inode4+locn][1] = u42 + ckw4 * u2sum;
            u_vectorn[inode4+locn][2] = u43 + ckw4 * u3sum;

            locn = locn + nnoden + 1;
          }
        }
      }

      for (inode = 1; inode <= nnoden; ++inode)
      {
        iwi = iw[inode];

        if (iwi < 0)
        {
          ++knode;

          w = - dc1 / ((double) iwi);

          iw[inode] = 1;

          locn = 0;

          for (iscalar = 1; iscalar <= nscalar; ++iscalar)
          {
            u_scalarn[inode+locn] = w * u_scalarn[inode+locn];

            locn = locn + nnoden + 1;
          }

          locn = 0;

          for (ivector = 1; ivector <= nvector; ++ivector)
          {
            u_vectorn[inode+locn][0] = w * u_vectorn[inode+locn][0];
            u_vectorn[inode+locn][1] = w * u_vectorn[inode+locn][1];
            u_vectorn[inode+locn][2] = w * u_vectorn[inode+locn][2];

            locn = locn + nnoden + 1;
          }

          if (mmsg == 2 && knode % iwrite == 0)
          {
            sprintf (Text, "UG3      : Pass, Nodes       =%10i%10i",
                     ipass, knode);
            ug_message (Text);
          }
        }
      }
    }
    while (ipass < nit && knode < nnoden);
  }

  if (knode < nnoden)
    return (319);

  if (mmsg == 2)
  {
    sprintf (Text, "UG3      : Pass, Nodes       =%10i%10i", ipass, knode);
    ug_message (Text);
  }

  return (0);

}
