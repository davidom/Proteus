#include "UG3_LIB.h"

INT_ ug3_bnd_nodes
 (INT_ mremove,
  INT_ *nnodeb,
  INT_ nnode,
  INT_ nbface,
  INT_ nquad,
  INT_3D * inibf,
  INT_4D * iniq,
  INT_1D ** node_map_ptr,
  DOUBLE_1D * del,
  DOUBLE_1D * ds,
  DOUBLE_3D * x)

{

/*
 * Determine node index map for boundary nodes and optionally either remove
 * interior nodes from grid data or continue node index map with interior nodes
 * following boundary nodes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_bnd_nodes.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *node_map;

  INT_ ibface, ierr, inode, inode1, inode2, inode3, inode4, inodeb, inodev,
       iquad, nnodebi;

  ierr = 0;

  *node_map_ptr = (INT_1D *) ug_malloc (&ierr, (nnode+1) * sizeof (INT_1D));

  if (ierr > 0)
    return (100314);

  node_map = *node_map_ptr;

  for (inode = 1; inode <= nnode; ++inode)
  {
    node_map[inode] = 0;
  }

  nnodebi = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    nnodebi = MAX (inode1, nnodebi);
    nnodebi = MAX (inode2, nnodebi);
    nnodebi = MAX (inode3, nnodebi);

    node_map[inode1] = 1;
    node_map[inode2] = 1;
    node_map[inode3] = 1;
  }

  for (iquad = 1; iquad <= nquad; ++iquad)
  {
    inode1 = iniq[iquad][0];
    inode2 = iniq[iquad][1];
    inode3 = iniq[iquad][2];
    inode4 = iniq[iquad][3];

    nnodebi = MAX (inode1, nnodebi);
    nnodebi = MAX (inode2, nnodebi);
    nnodebi = MAX (inode3, nnodebi);
    nnodebi = MAX (inode4, nnodebi);

    node_map[inode1] = 1;
    node_map[inode2] = 1;
    node_map[inode3] = 1;
    node_map[inode4] = 1;
  }

  inodeb = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    if (node_map[inode] == 1)
    {
      ++inodeb;

      node_map[inode] = inodeb;
    }
  }

  *nnodeb = inodeb;

  if (mremove == -1)
  {
    inodev = *nnodeb;

    for (inode = 1; inode <= nnode; ++inode)
    {
      if (node_map[inode] == 0)
      {
        ++inodev;

        node_map[inode] = inodev;
      }
    }
  }
  else if (mremove == 1)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      inodeb = node_map[inode];

      if (inodeb > 0)
      {
        x[inodeb][0] = x[inode][0];
        x[inodeb][1] = x[inode][1];
        x[inodeb][2] = x[inode][2];

        if (ds != NULL)
          ds[inodeb] = ds[inode];

        if (del != NULL)
          del[inodeb] = del[inode];
      }
    }

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      inibf[ibface][0] = node_map[inode1];
      inibf[ibface][1] = node_map[inode2];
      inibf[ibface][2] = node_map[inode3];
    }

    for (iquad = 1; iquad <= nquad; ++iquad)
    {
      inode1 = iniq[iquad][0];
      inode2 = iniq[iquad][1];
      inode3 = iniq[iquad][2];
      inode4 = iniq[iquad][3];

      iniq[iquad][0] = node_map[inode1];
      iniq[iquad][1] = node_map[inode2];
      iniq[iquad][2] = node_map[inode3];
      iniq[iquad][3] = node_map[inode4];
    }

    ug_free (node_map);

    *node_map_ptr = NULL;
  }

  if (*nnodeb == nnodebi)
    return (-1);

  return (0);

}
