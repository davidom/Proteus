#include "UG3_LIB.h"

INT_ ug3_ibfin
 (INT_ nbface,
  INT_ *nbfpntd,
  INT_ nnode,
  INT_ *nbfpnt,
  INT_3D * inibf,
  INT_1D ** ibfin_ptr,
  INT_1D * libfin)

{

/*
 * Create a list of boundary faces surrounding a node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ibfin.c,v 1.8 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D * ibfin;

  INT_ ibface, ierr, inode, inode1, inode2, inode3, loc, nbfpn;

  for (inode = 1; inode <= nnode; ++inode)
  {
    libfin[inode] = 0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    ++(libfin[inode1]);
    ++(libfin[inode2]);
    ++(libfin[inode3]);
  }

  *nbfpnt = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    *nbfpnt = *nbfpnt + libfin[inode];
  }

  if (*nbfpnt > *nbfpntd)
  {
    *nbfpntd = *nbfpnt;

    ierr = 0;

    *ibfin_ptr = (INT_1D *) ug_realloc (&ierr, *ibfin_ptr, 
                                        (*nbfpntd+1) * sizeof (INT_1D));

    if (ierr > 0)
      return (100302);
  }

  ibfin = *ibfin_ptr;

  loc = 1;

  nbfpn = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    loc = loc + nbfpn;

    nbfpn = libfin[inode];

    libfin[inode] = loc;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    loc = libfin[inode1];

    ibfin[loc] = ibface;

    ++loc;

    libfin[inode1] = loc;

    loc = libfin[inode2];

    ibfin[loc] = ibface;

    ++loc;

    libfin[inode2] = loc;

    loc = libfin[inode3];

    ibfin[loc] = ibface;

    ++loc;

    libfin[inode3] = loc;
  }

  for (inode = nnode + 1; inode >= 2; --inode)
  {
    libfin[inode] = libfin[inode - 1];
  }

  libfin[1] = 1;

  return (0);

}
