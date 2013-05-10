#include "UG3_LIB.h"

INT_ ug3_idielm
 (INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nelemc8,
  INT_ nnode,
  INT_1D * idiel,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inielc8)

{

/*
 * Set volume ID flag in mixed element region.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_idielm.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *idin = NULL;

  INT_ id, id1, id2, id3, id4, id5, id6, id7, id8, idmax, idmin, ielem, ielemc,
       ierr, iit, inode, inode1, inode2, inode3, inode4, inode5, inode6, inode7,
       inode8, nelemc;

  nelemc = nelemc5 + nelemc6 + nelemc8;

  if (nelem == 0 || nelemc == 0) return (0);

  ierr = 0;

  idin = (INT_1D *) ug_malloc (&ierr, (nnode+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (idin);
    return (100313);
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    idin[inode] = 0;
  }

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    id = idiel[ielem];

    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    idin[inode1] = id;
    idin[inode2] = id;
    idin[inode3] = id;
    idin[inode4] = id;
  }

  iit = 1;

  do
  {
    for (ielem = 1; ielem <= nelemc5; ++ielem)
    {
      inode1 = inielc5[ielem][0];
      inode2 = inielc5[ielem][1];
      inode3 = inielc5[ielem][2];
      inode4 = inielc5[ielem][3];
      inode5 = inielc5[ielem][4];

      id1 = idin[inode1];
      id2 = idin[inode2];
      id3 = idin[inode3];
      id4 = idin[inode4];
      id5 = idin[inode5];

      idmax = MAX (id1, id2);
      idmax = MAX (idmax, id3);
      idmax = MAX (idmax, id4);
      idmax = MAX (idmax, id5);

      if (idmax > 0)
      {
        idin[inode1] = idmax;
        idin[inode2] = idmax;
        idin[inode3] = idmax;
        idin[inode4] = idmax;
        idin[inode5] = idmax;
      }
    }

    for (ielem = 1; ielem <= nelemc6; ++ielem)
    {
      inode1 = inielc6[ielem][0];
      inode2 = inielc6[ielem][1];
      inode3 = inielc6[ielem][2];
      inode4 = inielc6[ielem][3];
      inode5 = inielc6[ielem][4];
      inode6 = inielc6[ielem][5];

      id1 = idin[inode1];
      id2 = idin[inode2];
      id3 = idin[inode3];
      id4 = idin[inode4];
      id5 = idin[inode5];
      id6 = idin[inode6];

      idmax = MAX (id1, id2);
      idmax = MAX (idmax, id3);
      idmax = MAX (idmax, id4);
      idmax = MAX (idmax, id5);
      idmax = MAX (idmax, id6);

      if (idmax > 0)
      {
        idin[inode1] = idmax;
        idin[inode2] = idmax;
        idin[inode3] = idmax;
        idin[inode4] = idmax;
        idin[inode5] = idmax;
        idin[inode6] = idmax;
      }
    }

    for (ielem = 1; ielem <= nelemc8; ++ielem)
    {
      inode1 = inielc8[ielem][0];
      inode2 = inielc8[ielem][1];
      inode3 = inielc8[ielem][2];
      inode4 = inielc8[ielem][3];
      inode5 = inielc8[ielem][4];
      inode6 = inielc8[ielem][5];
      inode7 = inielc8[ielem][6];
      inode8 = inielc8[ielem][7];

      id1 = idin[inode1];
      id2 = idin[inode2];
      id3 = idin[inode3];
      id4 = idin[inode4];
      id5 = idin[inode5];
      id6 = idin[inode6];
      id7 = idin[inode7];
      id8 = idin[inode8];

      idmax = MAX (id1, id2);
      idmax = MAX (idmax, id3);
      idmax = MAX (idmax, id4);
      idmax = MAX (idmax, id5);
      idmax = MAX (idmax, id6);
      idmax = MAX (idmax, id7);
      idmax = MAX (idmax, id8);

      if (idmax > 0)
      {
        idin[inode1] = idmax;
        idin[inode2] = idmax;
        idin[inode3] = idmax;
        idin[inode4] = idmax;
        idin[inode5] = idmax;
        idin[inode6] = idmax;
        idin[inode7] = idmax;
        idin[inode8] = idmax;
      }
    }

    idmax = idin[1];
    idmin = idin[1];

    for (inode = 2; inode <= nnode; ++inode)
    {
      idmax = MAX (idmax, idin[inode]);
      idmin = MIN (idmin, idin[inode]);
    }

    ++iit;
  }
  while (iit < nelemc && idmin == 0);

  ielemc = nelem;

  for (ielem = 1; ielem <= nelemc5; ++ielem)
  {
    ++ielemc;

    inode1 = inielc5[ielem][0];

    idiel[ielemc] = idin[inode1];
  }

  for (ielem = 1; ielem <= nelemc6; ++ielem)
  {
    ++ielemc;

    inode1 = inielc6[ielem][0];

    idiel[ielemc] = idin[inode1];
  }

  for (ielem = 1; ielem <= nelemc8; ++ielem)
  {
    ++ielemc;

    inode1 = inielc8[ielem][0];

    idiel[ielemc] = idin[inode1];
  }

  ug_free (idin);

  return (0);

}
