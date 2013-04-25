#include "UG3_LIB.h"

INT_ ug3_reorderid
 (INT_ nbface,
  INT_ *nnodeb,
  INT_ *nnoded,
  INT_1D * ibcibf,
  INT_1D * idibf,
  INT_1D ** idin_ptr,
  INT_3D * inibf,
  INT_1D * irfibf,
  INT_1D * jbfibf,
  INT_1D ** jnin_ptr,
  DOUBLE_1D ** del_ptr,
  DOUBLE_1D ** ds_ptr,
  DOUBLE_3D ** x_ptr)

{

/*
 * Re-order the boundary faces and nodes so that the boundary faces and nodes
 * with the same ID are grouped and consecutively ordered together. Also, create
 * new duplicate boundary nodes such that each boundary face group with the
 * same ID have unique nodes that are not shared by any other ID group.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_reorderid.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *idin, *jnin;

  DOUBLE_1D *del = NULL;
  DOUBLE_1D *ds = NULL;
  DOUBLE_3D *x;

  INT_ ibc, ibface, ibface0, ibface2, id, idmax, idmin, ierr, inode, inode0,
       inode1, inode2, inode3, irf, jbface, jbface0, jd, jnode, jnode0, jnode1,
       jnode2, jnode3, kd, mfound, nnodebi;

  double deli, dsi, x1, x2, x3;

  ibface = 1;

  id = idibf[ibface];

  idmax = id;
  idmin = id;

  for (ibface = 2; ibface <= nbface; ++ibface)
  {
    id = idibf[ibface];

    idmax = MAX (idmax, id);
    idmin = MIN (idmin, id);
  }

  if (idmin == idmax)
    return (0);

  ibface0 = 1;

  for (kd = idmin; kd <= idmax; ++kd)
  {
    ibface = ibface0;

    do
    {
      id = idibf[ibface];

      ++ibface;
    }
    while (ibface <= nbface && id != kd);

    if (id == kd)
    {
      jbface0 = ibface0;

      for (ibface = ibface0; ibface <= nbface; ++ibface)
      {
        jbfibf[ibface] = ibface;

        id = idibf[ibface];

        if (id == kd) ++jbface0;
      }

      mfound = 1;

      ibface2 = nbface;

      ibface = ibface0;

      do
      {
        id = idibf[ibface];

        if (id != kd)
        {
          mfound = 0;

          if (ibface2 > ibface)
          {
            jbface = ibface2;

            do
            {
              jd = idibf[jbface];

              if (jd == kd)
                mfound = 1;

              --jbface;
            }
            while (jbface > ibface && mfound == 0);

            if (mfound == 1)
            {
              ++jbface;

              jbfibf[ibface] = jbface;
              jbfibf[jbface] = ibface;

              ibface2 = jbface - 1;
            }
          }
        }

        ++ibface;
      }
      while (ibface <= nbface && mfound == 1);

      for (ibface = ibface0; ibface <= nbface; ++ibface)
      {
        jbface = jbfibf[ibface];

        if (jbface > ibface)
        {
          id = idibf[ibface];

          idibf[ibface] = idibf[jbface];
          idibf[jbface] = id;

          if (ibcibf != NULL)
          {
            ibc = ibcibf[ibface];

            ibcibf[ibface] = ibcibf[jbface];
            ibcibf[jbface] = ibc;
          }

          if (irfibf != NULL)
          {
            irf = irfibf[ibface];

            irfibf[ibface] = irfibf[jbface];
            irfibf[jbface] = irf;
          }

          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          inibf[ibface][0] = inibf[jbface][0];
          inibf[ibface][1] = inibf[jbface][1];
          inibf[ibface][2] = inibf[jbface][2];
          inibf[jbface][0] = inode1;
          inibf[jbface][1] = inode2;
          inibf[jbface][2] = inode3;
        }
      }

      ibface0 = jbface0;
    }
  }

  idin = *idin_ptr;
  jnin = *jnin_ptr;

  del = *del_ptr;
  ds = *ds_ptr;
  x = *x_ptr;

  for (kd = idmin; kd <= idmax; ++kd)
  {
    ibface = 1;

    do
    {
      id = idibf[ibface];

      ++ibface;
    }
    while (ibface <= nbface && id != kd);

    if (id == kd)
    {
      for (inode = 1; inode <= *nnodeb; ++inode)
      {
        jnin[inode] = inode;
      }

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        id = idibf[ibface];

        if (id != kd)
        {
          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          jnin[inode1] = -1;
          jnin[inode2] = -1;
          jnin[inode3] = -1;
        }
      }

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        id = idibf[ibface];

        if (id == kd)
        {
          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          jnode1 = jnin[inode1];
          jnode2 = jnin[inode2];
          jnode3 = jnin[inode3];

          if (jnode1 == -1) jnin[inode1] = 0;
          if (jnode2 == -1) jnin[inode2] = 0;
          if (jnode3 == -1) jnin[inode3] = 0;
        }
      }

      jnode = *nnodeb;

      for (inode = 1; inode <= *nnodeb; ++inode)
      {
        if (jnin[inode] == 0)
        {
          ++jnode;

          jnin[inode] = jnode;
        }
        else
          jnin[inode] = inode;
      }

      nnodebi = jnode;

      if (nnodebi > *nnoded)
      {
        ierr = 0;

        *idin_ptr = (INT_1D *) ug_realloc (&ierr, *idin_ptr,
                                           (nnodebi+1) * sizeof (INT_1D));

        *jnin_ptr = (INT_1D *) ug_realloc (&ierr, *jnin_ptr,
                                           (nnodebi+1) * sizeof (INT_1D));

        if (*del_ptr != NULL)
          *del_ptr = (DOUBLE_1D *) ug_realloc (&ierr, *del_ptr,
                                               (nnodebi+1) * sizeof (DOUBLE_1D));
        if (*ds_ptr != NULL)
          *ds_ptr = (DOUBLE_1D *) ug_realloc (&ierr, *ds_ptr,
                                              (nnodebi+1) * sizeof (DOUBLE_1D));

        *x_ptr = (DOUBLE_3D *) ug_realloc (&ierr, *x_ptr,
                                           (nnodebi+1) * sizeof (DOUBLE_3D));

        if (ierr > 0)
          return (100310);

        *nnoded = nnodebi;

        idin = *idin_ptr;
        jnin = *jnin_ptr;

        del = *del_ptr;
        ds = *ds_ptr;
        x = *x_ptr;
      }

      for (inode = 1; inode <= *nnodeb; ++inode)
      {
        jnode = jnin[inode];

        if (jnode != inode)
        {
          if (del != NULL)
            del[jnode] = del[inode];

          if (ds != NULL)
            ds[jnode] = ds[inode];

          x[jnode][0] = x[inode][0];
          x[jnode][1] = x[inode][1];
          x[jnode][2] = x[inode][2];
        }
      }

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        id = idibf[ibface];

        if (id == kd)
        {
          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          jnode1 = jnin[inode1];
          jnode2 = jnin[inode2];
          jnode3 = jnin[inode3];

          if (jnode1 != inode1) inibf[ibface][0] = jnode1;
          if (jnode2 != inode2) inibf[ibface][1] = jnode2;
          if (jnode3 != inode3) inibf[ibface][2] = jnode3;
        }
      }

      *nnodeb = nnodebi;
    }
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    id = idibf[ibface];

    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    idin[inode1] = id;
    idin[inode2] = id;
    idin[inode3] = id;
  }

  inode0 = 1;

  for (kd = idmin; kd <= idmax; ++kd)
  {
    inode = inode0;

    do
    {
      id = idin[inode];

      ++inode;
    }
    while (inode <= *nnodeb && id != kd);

    if (id == kd)
    {
      jnode0 = inode0;

      for (inode = 1; inode <= *nnodeb; ++inode)
      {
        jnin[inode] = inode;

        id = idin[inode];

        if (id == kd) ++jnode0;
      }

      mfound = 1;

      inode2 = *nnodeb;

      inode = inode0;

      do
      {
        id = idin[inode];

        if (id != kd)
        {
          mfound = 0;

          if (inode2 > inode)
          {
            jnode = inode2;

            do
            {
              jd = idin[jnode];

              if (jd == kd)
                mfound = 1;

              --jnode;
            }
            while (jnode > inode && mfound == 0);

            if (mfound == 1)
            {
              ++jnode;

              jnin[inode] = jnode;
              jnin[jnode] = inode;

              inode2 = jnode - 1;
            }
          }
        }

        ++inode;
      }
      while (inode <= *nnodeb && mfound == 1);

      for (inode = inode0; inode <= *nnodeb; ++inode)
      {
        jnode = jnin[inode];

        if (jnode > inode)
        {
          id = idin[inode];

          idin[inode] = idin[jnode];
          idin[jnode] = id;

          if (del != NULL)
          {
            deli = del[inode];

            del[inode] = del[jnode];
            del[jnode] = deli;
          }

          if (ds != NULL)
          {
            dsi = ds[inode];

            ds[inode] = ds[jnode];
            ds[jnode] = dsi;
          }

          x1 = x[inode][0];
          x2 = x[inode][1];
          x3 = x[inode][2];

          x[inode][0] = x[jnode][0];
          x[inode][1] = x[jnode][1];
          x[inode][2] = x[jnode][2];
          x[jnode][0] = x1;
          x[jnode][1] = x2;
          x[jnode][2] = x3;
        }
      }

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        inibf[ibface][0] = jnin[inode1];
        inibf[ibface][1] = jnin[inode2];
        inibf[ibface][2] = jnin[inode3];
      }

      inode0 = jnode0;
    }
  }

  return (0);

}
