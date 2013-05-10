#include "UG3_LIB.h"

INT_ ug3_bnd_vol_nodes
 (INT_ *nnodeb,
  INT_ nnode,
  INT_ nbface,
  INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nelemc8,
  INT_ nquad,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inielc8,
  INT_4D * iniq,
  DOUBLE_1D * del,
  DOUBLE_1D * ds,
  DOUBLE_3D * x)

{

/*
 * Reorder nodes so that boundary nodes are first followed by interior nodes
 * and increment all connectivity node indicies by one if the input connectivity
 * uses node index 0.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_bnd_vol_nodes.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  DOUBLE_1D *save = NULL;

  INT_1D *node_map = NULL;

  INT_ i, i0, ibface, ielem, ierr, inode, inode1, inode2, inode3, inode4,
       inode5, inode6, inode7, inode8, iquad, jnode;

  inode = 1;

  ibface = 1;

  while (ibface <= nbface && inode > 0)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    inode = MIN (inode, inode1);
    inode = MIN (inode, inode2);
    inode = MIN (inode, inode3);

    ++ibface;
  }

  iquad = 1;

  while (iquad <= nquad && inode > 0)
  {
    inode1 = iniq[iquad][0];
    inode2 = iniq[iquad][1];
    inode3 = iniq[iquad][2];
    inode4 = iniq[iquad][3];

    inode = MIN (inode, inode1);
    inode = MIN (inode, inode2);
    inode = MIN (inode, inode3);
    inode = MIN (inode, inode4);

    ++iquad;
  }

  ielem = 1;

  while (ielem <= nelem && inode > 0)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    inode = MIN (inode, inode1);
    inode = MIN (inode, inode2);
    inode = MIN (inode, inode3);
    inode = MIN (inode, inode4);

    ++ielem;
  }

  ielem = 1;

  while (ielem <= nelemc5 && inode > 0)
  {
    inode1 = inielc5[ielem][0];
    inode2 = inielc5[ielem][1];
    inode3 = inielc5[ielem][2];
    inode4 = inielc5[ielem][3];
    inode5 = inielc5[ielem][4];

    inode = MIN (inode, inode1);
    inode = MIN (inode, inode2);
    inode = MIN (inode, inode3);
    inode = MIN (inode, inode4);
    inode = MIN (inode, inode5);

    ++ielem;
  }

  ielem = 1;

  while (ielem <= nelemc6 && inode > 0)
  {
    inode1 = inielc6[ielem][0];
    inode2 = inielc6[ielem][1];
    inode3 = inielc6[ielem][2];
    inode4 = inielc6[ielem][3];
    inode5 = inielc6[ielem][4];
    inode6 = inielc6[ielem][5];

    inode = MIN (inode, inode1);
    inode = MIN (inode, inode2);
    inode = MIN (inode, inode3);
    inode = MIN (inode, inode4);
    inode = MIN (inode, inode5);
    inode = MIN (inode, inode6);

    ++ielem;
  }

  ielem = 1;

  while (ielem <= nelemc8 && inode > 0)
  {
    inode1 = inielc8[ielem][0];
    inode2 = inielc8[ielem][1];
    inode3 = inielc8[ielem][2];
    inode4 = inielc8[ielem][3];
    inode5 = inielc8[ielem][4];
    inode6 = inielc8[ielem][5];
    inode7 = inielc8[ielem][6];
    inode8 = inielc8[ielem][7];

    inode = MIN (inode, inode1);
    inode = MIN (inode, inode2);
    inode = MIN (inode, inode3);
    inode = MIN (inode, inode4);
    inode = MIN (inode, inode5);
    inode = MIN (inode, inode6);
    inode = MIN (inode, inode7);
    inode = MIN (inode, inode8);

    ++ielem;
  }

  if (inode == 0)
  {
    i0 = 1;

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      inibf[ibface][0] = inibf[ibface][0] + i0;
      inibf[ibface][1] = inibf[ibface][1] + i0;
      inibf[ibface][2] = inibf[ibface][2] + i0;
    }

    for (iquad = 1; iquad <= nquad; ++iquad)
    {
      iniq[iquad][0] = iniq[iquad][0] + i0;
      iniq[iquad][1] = iniq[iquad][1] + i0;
      iniq[iquad][2] = iniq[iquad][2] + i0;
      iniq[iquad][3] = iniq[iquad][3] + i0;
    }
  }
  else
    i0 = 0;

  ierr = 0;

  save = (DOUBLE_1D *) ug_malloc (&ierr, (nnode+1) * sizeof (DOUBLE_1D));

  if (ierr > 0)
    return (100315);

  ierr = ug3_bnd_nodes (-1, nnodeb, nnode, nbface, nquad,
                        inibf, iniq, &node_map,
                        del, ds, x);

  if (ierr != 0)
  {
    ug_free (save);
    ug_free (node_map);
    if (ierr > 0)
      return (1);
    else
      return (0);
  }

  for (i = 0; i <=2; ++i)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      save[inode] = x[inode][i];
    }

    for (inode = 1; inode <= nnode; ++inode)
    {
      jnode = node_map[inode];

      x[jnode][i] = save[inode];
    }
  }

  if (del != NULL)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      save[inode] = del[inode];
    }

    for (inode = 1; inode <= nnode; ++inode)
    {
      jnode = node_map[inode];

      del[jnode] = save[inode];
    }
  }

  if (ds != NULL)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      save[inode] = ds[inode];
    }

    for (inode = 1; inode <= nnode; ++inode)
    {
      jnode = node_map[inode];

      ds[jnode] = save[inode];
    }
  }

  ug_free (save);

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

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    inode1 = iniel[ielem][0] + i0;
    inode2 = iniel[ielem][1] + i0;
    inode3 = iniel[ielem][2] + i0;
    inode4 = iniel[ielem][3] + i0;

    iniel[ielem][0] = node_map[inode1];
    iniel[ielem][1] = node_map[inode2];
    iniel[ielem][2] = node_map[inode3];
    iniel[ielem][3] = node_map[inode4];
  }

  for (ielem = 1; ielem <= nelemc5; ++ielem)
  {
    inode1 = inielc5[ielem][0] + i0;
    inode2 = inielc5[ielem][1] + i0;
    inode3 = inielc5[ielem][2] + i0;
    inode4 = inielc5[ielem][3] + i0;
    inode5 = inielc5[ielem][4] + i0;

    inielc5[ielem][0] = node_map[inode1];
    inielc5[ielem][1] = node_map[inode2];
    inielc5[ielem][2] = node_map[inode3];
    inielc5[ielem][3] = node_map[inode4];
    inielc5[ielem][4] = node_map[inode5];
  }

  for (ielem = 1; ielem <= nelemc6; ++ielem)
  {
    inode1 = inielc6[ielem][0] + i0;
    inode2 = inielc6[ielem][1] + i0;
    inode3 = inielc6[ielem][2] + i0;
    inode4 = inielc6[ielem][3] + i0;
    inode5 = inielc6[ielem][4] + i0;
    inode6 = inielc6[ielem][5] + i0;

    inielc6[ielem][0] = node_map[inode1];
    inielc6[ielem][1] = node_map[inode2];
    inielc6[ielem][2] = node_map[inode3];
    inielc6[ielem][3] = node_map[inode4];
    inielc6[ielem][4] = node_map[inode5];
    inielc6[ielem][5] = node_map[inode6];
  }

  for (ielem = 1; ielem <= nelemc8; ++ielem)
  {
    inode1 = inielc8[ielem][0] + i0;
    inode2 = inielc8[ielem][1] + i0;
    inode3 = inielc8[ielem][2] + i0;
    inode4 = inielc8[ielem][3] + i0;
    inode5 = inielc8[ielem][4] + i0;
    inode6 = inielc8[ielem][5] + i0;
    inode7 = inielc8[ielem][6] + i0;
    inode8 = inielc8[ielem][7] + i0;

    inielc8[ielem][0] = node_map[inode1];
    inielc8[ielem][1] = node_map[inode2];
    inielc8[ielem][2] = node_map[inode3];
    inielc8[ielem][3] = node_map[inode4];
    inielc8[ielem][4] = node_map[inode5];
    inielc8[ielem][5] = node_map[inode6];
    inielc8[ielem][6] = node_map[inode7];
    inielc8[ielem][7] = node_map[inode8];
  }

  ug_free (node_map);

  return (0);

}
