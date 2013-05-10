#include "UG3_LIB.h"

void ug3_idibf
 (INT_ mmsg,
  INT_ msetbc,
  INT_ nbface,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * mchkbf,
  double angbd,
  DOUBLE_3D *x)

{

/*
 * Set boundary surface face IDs by boundary surface face grid BC
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_idibf.c,v 1.8 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ibc, ibcmax, ibcmin, ibface, ibfn1, ibfn2, ibfn3, ichk, ichk1, ichk2, id,
       idmax, idmin, inode1, inode2, inode3, inode4, jbc, jbface, jchk, mfound,
       nid;

  double a1231, a1232, a1233, a2431, a2432, a2433, cosbd, cosbds, dc0, dc1,
         dc45, dx211, dx212, dx213, dx311, dx312, dx313, dx321, dx322, dx323,
         dx421, dx422, dx423, w, x11, x12, x13, x21, x22, x23, x31, x32, x33,
         x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  if (msetbc == 0 || idibf == NULL)
    return;

  cosbd = - cos (angbd * atan (dc1) / dc45);

  cosbds = cosbd * fabs (cosbd);

  idmax = idibf[1];
  idmin = idibf[1];

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    id = idibf[ibface];

    idmax = MAX (id, idmax);
    idmin = MIN (id, idmin);
  }

  if (ibcibf != NULL)
  {
    ibcmax = ibcibf[1];
    ibcmin = ibcibf[1];

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ibc = ibcibf[ibface];

      ibcmax = MAX (ibc, ibcmax);
      ibcmin = MIN (ibc, ibcmin);
    }
  }

  if (idmin == idmax && idmin == 0)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      idibf[ibface] = 1;
    }
  }

  if (idmin < idmax || (angbd == dc0 && ibcmin == ibcmax))
    return;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    idibf[ibface] = 0;

    mchkbf[ibface] = 0;
  }

  ibc = 0;
  jbc = 0;

  ibface = 1;

  id = 0;

  do
  {
    ++id;

    if (ibcibf != NULL)
      jbc = ibcibf[ibface];

    ichk = 1;

    mchkbf[ichk] = ibface;

    idibf[ibface] = id;

    ichk2 = 0;

    do
    {
      ichk1 = ichk2 + 1;
      ichk2 = ichk;

      for (jchk = ichk1; jchk <= ichk2; ++jchk)
      {
        jbface = mchkbf[jchk];

        for (ibfn1 = 0; ibfn1 <= 2; ++ibfn1)
        {
          ibface = ibfibf[jbface][ibfn1];

          if (ibface > 0)
          {
            if (ibcibf != NULL)
              ibc = ibcibf[ibface];

            if (idibf[ibface] == 0 && ibc == jbc)
            {
              ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;
              ibfn3 = (ibfn2 < 2) ? ibfn2+1 : 0;

              inode1 = inibf[jbface][ibfn1];
              inode2 = inibf[jbface][ibfn2];
              inode3 = inibf[jbface][ibfn3];
              inode4 = (ibfibf[ibface][0] == jbface) ? inibf[ibface][0]:
                       (ibfibf[ibface][1] == jbface) ? inibf[ibface][1]:
                                                       inibf[ibface][2];

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
              a2431 = dx422 * dx323 - dx423 * dx322;
              a2432 = dx423 * dx321 - dx421 * dx323;
              a2433 = dx421 * dx322 - dx422 * dx321;

              w = a1231 * a2431 + a1232 * a2432 + a1233 * a2433;

              w = w * fabs (w)
                - cosbds * (a1231 * a1231 + a1232 * a1232 + a1233 * a1233)
                         * (a2431 * a2431 + a2432 * a2432 + a2433 * a2433);

              if (w > dc0)
              {
                ++ichk;

                mchkbf[ichk] = ibface;

                idibf[ibface] = id;
              }
            }
          }
        }
      }
    }
    while (ichk2 >= ichk1);

    mfound = 0;

    ibface = 0;

    do
    {
      ++ibface;

      if (idibf[ibface] == 0)
        mfound = 1;
    }
    while (ibface < nbface && mfound == 0);
  }
  while (mfound == 1);

  nid = id;

  if (mmsg == 2)
  {
    sprintf (Text, "Boundary ID        : IDs               =%10i", nid);
    ug_message (Text);
  }

  return;

}
