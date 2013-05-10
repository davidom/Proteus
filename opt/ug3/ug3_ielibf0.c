#include "UG3_LIB.h"

void ug3_ielibf0
 (INT_ mbfdir,
  INT_ nbface,
  INT_ nelem,
  INT_ nnode,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_1D * ibfjbf,
  INT_1D * ibfjn,
  INT_1D * ielibf)

{

/*
 * Determine for each boundary face the element that contains the face.
 * Determine either the element inside the domain if mbfdir=1 or outside if
 * mbfdir=-1.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ielibf0.c,v 1.7 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ibface1, ibface2, ibface3, ibface4, ibfg, ibfjn1, ibfjn2,
       ibfjn3, ibfjnm, ibfn1, ibfn2, ibfn3, ielem, inode, inode1, inode2,
       inode3, inode4, jbface, jbface1, jbface2, jnode1, jnode2, jnode3, nnodem;

  if (mbfdir == 1)
  {
    ibfn1 = 0;
    ibfn2 = 1;
    ibfn3 = 2;
  }
  else
  {
    ibfn1 = 0;
    ibfn2 = 2;
    ibfn3 = 1;
  }

  nnodem = nnode;

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    nnodem = MAX (nnodem, inode1);
    nnodem = MAX (nnodem, inode2);
    nnodem = MAX (nnodem, inode3);
    nnodem = MAX (nnodem, inode4);
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ielibf[ibface] = 0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibfjbf[ibface] = ibface;
  }

  jbface2 = 0;

  ibfg = 1;

  do
  {
    for (inode = 1; inode <= nnodem; ++inode)
    {
      ibfjn[inode] = 1;
    }

    jbface1 = jbface2 + 1;

    for (jbface = jbface1; jbface <= nbface; ++jbface)
    {
      ibface = ibfjbf[jbface];

      inode1 = inibf[ibface][ibfn1];
      inode2 = inibf[ibface][ibfn2];
      inode3 = inibf[ibface][ibfn3];

      ibfjn1 = ibfjn[inode1];
      ibfjn2 = ibfjn[inode2];
      ibfjn3 = ibfjn[inode3];

      ibfjnm = MIN (ibfjn1, ibfjn2);
      ibfjnm = MIN (ibfjn3, ibfjnm);

      if (ibfjnm == 1)
      {
        ++jbface2;

        ibface2 = ibfjbf[jbface2];

        ibfjbf[jbface2] = ibface;
        ibfjbf[jbface] = ibface2;

        ibfjn[inode1] = 0;
        ibfjn[inode2] = 0;
        ibfjn[inode3] = 0;
      }
    }

    for (inode = 1; inode <= nnodem; ++inode)
    {
      ibfjn[inode] = nbface + inode;
    }

    for (jbface = jbface1; jbface <= jbface2; ++jbface)
    {
      ibface = ibfjbf[jbface];

      inode1 = inibf[ibface][ibfn1];
      inode2 = inibf[ibface][ibfn2];
      inode3 = inibf[ibface][ibfn3];

      ibfjn[inode1] = ibface;
      ibfjn[inode2] = ibface;
      ibfjn[inode3] = ibface;
    }

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      inode1 = iniel[ielem][0];
      inode2 = iniel[ielem][1];
      inode3 = iniel[ielem][2];
      inode4 = iniel[ielem][3];

      ibface1 = ibfjn[inode1];
      ibface2 = ibfjn[inode2];
      ibface3 = ibfjn[inode3];
      ibface4 = ibfjn[inode4];

      if (ibface2 == ibface3 && ibface3 == ibface4)
      {
        ibface = ibface2;
        jnode1 = inode2;
        jnode2 = inode4;
        jnode3 = inode3;
      }
      else
      {
        if (ibface3 == ibface4 && ibface4 == ibface1)
        {
          ibface = ibface3;
          jnode1 = inode1;
          jnode2 = inode3;
          jnode3 = inode4;
        }
        else
        {
          if (ibface4 == ibface1 && ibface1 == ibface2)
          {
            ibface = ibface4;
            jnode1 = inode1;
            jnode2 = inode4;
            jnode3 = inode2;
          }
          else
          {
            if (ibface1 == ibface2 && ibface2 == ibface3)
            {
              ibface = ibface1;
              jnode1 = inode1;
              jnode2 = inode2;
              jnode3 = inode3;
            }
            else
            {
              ibface = 0;
            }
          }
        }
      }

      if (ibface > 0)
      {
        inode1 = inibf[ibface][ibfn1];
        inode2 = inibf[ibface][ibfn2];

        if ((inode1 == jnode1 && inode2 == jnode2) ||
            (inode1 == jnode2 && inode2 == jnode3) ||
            (inode1 == jnode3 && inode2 == jnode1))
          ielibf[ibface] = ielem;
      }
    }
  }
  while (ibfg <= nbface && jbface2 < nbface);

  return;

}
