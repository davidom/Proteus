#include "UG3_LIB.h"

INT_ ug3_chkbda
 (INT_ merr,
  INT_ mmsg,
  INT_ nbface,
  INT_3D * ibfibf,
  INT_3D * inibf,
  INT_1D * mchkbf,
  INT_1D * ierribf,
  double angrbfsd,
  DOUBLE_3D * x)

{

/*
 * Check dihedral angle between adjacent triangular faces of surface grid.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkbda.c,v 1.11 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  CHAR_133 Text;

  INT_ ibface, ibfn1, ibfn2, ibfn3, inode1, inode2, inode3, inode4, ierr,
       jbface, jbfn1, mchkbf0;

  INT_ mchkbfi[3], mchkbfj[3];

  double a1231, a1232, a1233, a2431, a2432, a2433, cosmax, cosmaxs, d0, dc0,
         dc1, dc45, dx211, dx212, dx213, dx321, dx322, dx323, dx421, dx422,
	 dx423, w, x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  if (angrbfsd == dc0)
    return (0);

  if (mmsg == 2)
  {
    sprintf (Text, "Checking D-Angles  : B-Faces           =%10i", nbface);
    ug_message (Text);
  }

  cosmax = cos (angrbfsd * atan (dc1) / dc45);
  cosmaxs = cosmax * fabs (cosmax);

  if (merr)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ierribf[ibface] = 0;
    }
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    mchkbf[ibface] = 7;
  }

  ierr = 0;

  ibface = 1;

  do
  {
    mchkbf0 = mchkbf[ibface];

    if (mchkbf)
    {
      mchkbfi[2] = mchkbf0 / 4;
      mchkbf0 = mchkbf0 - 4 * mchkbfi[2];
      mchkbfi[1] = mchkbf0 / 2;
      mchkbfi[0] = mchkbf0 - mchkbfi[1] - mchkbfi[1];

      mchkbf[ibface] = 0;

      ibfn1 = 0;

      do
      {
        if (mchkbfi[ibfn1] == 1)
        {
          ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;
          ibfn3 = (ibfn2 < 2) ? ibfn2+1 : 0;

          jbface = ibfibf[ibface][ibfn1];

          if (jbface > 0)
          {
            inode1 = inibf[ibface][ibfn1];
            inode2 = inibf[ibface][ibfn2];
            inode3 = inibf[ibface][ibfn3];

            jbfn1 = (ibfibf[jbface][0] == ibface) ? 0 :
                    (ibfibf[jbface][1] == ibface) ? 1 : 2;

            inode4 = inibf[jbface][jbfn1];

            mchkbfj[2] = mchkbf0 / 4;
            mchkbf0 = mchkbf0 - 4 * mchkbfj[2];
            mchkbfj[1] = mchkbf0 / 2;
            mchkbfj[0] = mchkbf0 - mchkbfj[1] - mchkbfj[1];

            mchkbfj[jbfn1] = 0;

            mchkbf[jbface] = mchkbfj[0] + 2 * mchkbfj[1] + 4 * mchkbfj[2];

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
            dx421 = x41 - x21;
            dx422 = x42 - x22;
            dx423 = x43 - x23;

            a1231 = dx212 * dx323 - dx213 * dx322;
            a1232 = dx213 * dx321 - dx211 * dx323;
            a1233 = dx211 * dx322 - dx212 * dx321;
            a2431 = dx422 * dx323 - dx423 * dx322;
            a2432 = dx423 * dx321 - dx421 * dx323;
            a2433 = dx421 * dx322 - dx422 * dx321;

            w = a1231 * a2431 + a1232 * a2432 + a1233 * a2433;
            w = w * fabs (w)
              + cosmaxs * (a1231 * a1231 + a1232 * a1232 + a1233 * a1233)
                        * (a2431 * a2431 + a2432 * a2432 + a2433 * a2433);

            d0 = dx321 * (a1232 * a2433 - a1233 * a2432)
               + dx322 * (a1233 * a2431 - a1231 * a2433)
               + dx323 * (a1231 * a2432 - a1232 * a2431);

            if (w < dc0 && d0 < dc0)
            {
              ierr = 1;

              if (merr)
              {
                ierribf[ibface] = -1;
                ierribf[jbface] = -1;
              }
            }
          }
        }

        ++ibfn1;
      }
      while (ibfn1 <= 2 && (! ierr || merr));
    }

    ++ibface;
  }
  while (ibface <= nbface && (! ierr || merr));

  if (ierr)
  {
    if (merr)
      return (200300);
    else
      return (332);
  }

  if (mmsg == 2)
    ug_message ("Checking D-Angles  : Angles OK");

  return (0);
}
