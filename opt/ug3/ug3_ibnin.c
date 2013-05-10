#include "UG3_LIB.h"

INT_ ug3_ibnin
 (INT_ nbface,
  INT_ nnode,
  INT_ *nbnpnt,
  INT_ *nbnpntd,
  INT_3D * ibfibf,
  INT_3D * inibf,
  INT_1D ** ibnin_ptr,
  INT_1D * libnin)

{

/*
 * Create a list of boundary nodes surrounding a node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ibnin.c,v 1.12 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ibfjn;
  INT_1D *ibnin;

  INT_ ibface, ibface1, ibface2, ibface3, ibfn1, ibfn2, ibfn3, ierr, inode,
       inode1, inode2, inode3, jbface, loc, loc1, loc2, nloc;

  ibfjn = libnin;

  for (inode = 1; inode <= nnode; ++inode)
  {
    ibfjn[inode] = 0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    ibfjn[inode1] = ibface;
    ibfjn[inode2] = ibface;
    ibfjn[inode3] = ibface;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibface1 = ibfibf[ibface][0];
    ibface2 = ibfibf[ibface][1];
    ibface3 = ibfibf[ibface][2];

    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    if (ibface1 == 0)
      ibfjn[inode2] = ibface;

    if (ibface2 == 0)
      ibfjn[inode3] = ibface;

    if (ibface3 == 0)
      ibfjn[inode1] = ibface;
  }

  *nbnpnt = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    jbface = ibfjn[inode];

    if (jbface)
    {
      ibface = jbface;

      nloc = 0;

      do
      {
        ibfn2 = (inibf[ibface][0] == inode) ? 1 :
                (inibf[ibface][1] == inode) ? 2 : 0;

        ibface = ibfibf[ibface][ibfn2];

        ++nloc;
      }
      while (nloc <= nbface && ibface != jbface && ibface != 0);

      *nbnpnt = *nbnpnt + nloc;

      if (ibface == 0) ++(*nbnpnt);
    }
  }

  if (*nbnpnt > *nbnpntd)
  {
    *nbnpntd = *nbnpnt;

    ierr = 0;

    *ibnin_ptr = (INT_1D *) ug_realloc (&ierr, *ibnin_ptr, 
                                        (*nbnpntd+1) * sizeof (INT_1D));

    if (ierr > 0)
      return (100306);
  }

  ibnin = *ibnin_ptr;

  loc2 = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    jbface = ibfjn[inode];

    if (jbface)
    {
      ibface = jbface;

      nloc = 0;

      do
      {
        ibfn2 = (inibf[ibface][0] == inode) ? 1 :
                (inibf[ibface][1] == inode) ? 2 : 0;

        ibface = ibfibf[ibface][ibfn2];

        ++nloc;
      }
      while (nloc <= nbface && ibface != jbface && ibface != 0);

      loc1 = loc2 + 1;
      loc2 = loc2 + nloc;

      for (loc = loc1; loc <= loc2; ++loc)
      {
        ibface = jbface;

        ibfn1 = (inibf[ibface][0] == inode) ? 0 :
                (inibf[ibface][1] == inode) ? 1 : 2;
        ibfn2 = (ibfn1 < 2) ? ibfn1 + 1 : 0;

        ibnin[loc] = inibf[ibface][ibfn2];

        jbface = ibfibf[ibface][ibfn2];
      }

      if (jbface == 0)
      {
        ++loc2;

        loc = loc2;

        ibfn3 = (ibfn2 < 2) ? ibfn2 + 1 : 0;

        ibnin[loc] = inibf[ibface][ibfn3];
      }
    }
    else
    {
      loc1 = loc2 + 1;
      loc2 = loc2;
    }

    libnin[inode] = loc1;
  }

  libnin[nnode+1] = loc2+1;

  return (0);

}
