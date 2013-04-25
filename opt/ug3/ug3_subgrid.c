#include "UG3_LIB.h"

void ug3_subgrid
 (INT_ iv,
  INT_ mtask,
  INT_ nbface,
  INT_ *nbfacej,
  INT_ *nbfacek,
  INT_ nelem,
  INT_ *nelemj,
  INT_ nnode,
  INT_ *nnodebj,
  INT_ *nnodej,
  INT_1D * ibcibf,
  INT_1D * idibf,
  INT_4D * ieliel,
  INT_4D * iniel,
  INT_1D * injn,
  INT_1D * irfibf,
  INT_1D * iviel,
  INT_1D * jbcjbf,
  INT_1D * jdjbf,
  INT_3D * jnjbf,
  INT_4D * jnjel,
  INT_1D * jnin,
  INT_1D * jrfjbf,
  INT_1D * mchkbf,
  DOUBLE_3D * x,
  DOUBLE_3D * xj)

{

/*
 * Extract sub-grid for a specified element region.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_subgrid.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibface, ibface1, ibface2, ibface3, ibface4, id, ielem, ielem1,
       ielem2, ielem3, ielem4, inode, inode1, inode2, inode3, inode4, irf,
       jbface, jd, jelem, jnode, kbface;

  if (mtask == 1)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      jnin[inode] = 0;
    }

    jelem = 0;

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (iviel[ielem] == iv)
      {
        ++jelem;

        inode1 = iniel[ielem][0];
        inode2 = iniel[ielem][1];
        inode3 = iniel[ielem][2];
        inode4 = iniel[ielem][3];

        jnin[inode1] = -2;
        jnin[inode2] = -2;
        jnin[inode3] = -2;
        jnin[inode4] = -2;
      }
    }

    *nelemj = jelem;

    for (ibface = 0; ibface <= nbface; ++ibface)
    {
      mchkbf[ibface] = 0;
    }

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (iviel[ielem] == iv)
      {
        ibface1 = -ieliel[ielem][0];
        ibface2 = -ieliel[ielem][1];
        ibface3 = -ieliel[ielem][2];
        ibface4 = -ieliel[ielem][3];

        if (ibface1 > 0) ++(mchkbf[ibface1]);
        if (ibface2 > 0) ++(mchkbf[ibface2]);
        if (ibface3 > 0) ++(mchkbf[ibface3]);
        if (ibface4 > 0) ++(mchkbf[ibface4]);
      }
    }

    jbface = 0;
    kbface = 0;

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (iviel[ielem] == iv)
      {
        ielem1 = ieliel[ielem][0];
        ielem2 = ieliel[ielem][1];
        ielem3 = ieliel[ielem][2];
        ielem4 = ieliel[ielem][3];

        inode1 = iniel[ielem][0];
        inode2 = iniel[ielem][1];
        inode3 = iniel[ielem][2];
        inode4 = iniel[ielem][3];

        if (ielem1 <= 0 || iviel[ielem1] != iv)
        {
          ibface = MAX (-ielem1, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnin[inode2] = -1;
            jnin[inode4] = -1;
            jnin[inode3] = -1;

            mchkbf[ibface] = 0;

            if (ibface > 0) ++kbface;
          }
        }

        if (ielem2 <= 0 || iviel[ielem2] != iv)
        {
          ibface = MAX (-ielem2, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnin[inode1] = -1;
            jnin[inode3] = -1;
            jnin[inode4] = -1;

            mchkbf[ibface] = 0;

            if (ibface > 0) ++kbface;
          }
        }

        if (ielem3 <= 0 || iviel[ielem3] != iv)
        {
          ibface = MAX (-ielem3, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnin[inode1] = -1;
            jnin[inode4] = -1;
            jnin[inode2] = -1;

            mchkbf[ibface] = 0;

            if (ibface > 0) ++kbface;
          }
        }

        if (ielem4 <= 0 || iviel[ielem4] != iv)
        {
          ibface = MAX (-ielem4, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnin[inode1] = -1;
            jnin[inode2] = -1;
            jnin[inode3] = -1;

            mchkbf[ibface] = 0;

            if (ibface > 0) ++kbface;
          }
        }
      }
    }

    *nbfacej = jbface;
    *nbfacek = kbface;

    jnode = 0;

    for (inode = 1; inode <= nnode; ++inode)
    {
      if (jnin[inode] == -1)
        ++jnode;
    }

    *nnodebj = jnode;

    for (inode = 1; inode <= nnode; ++inode)
    {
      if (jnin[inode] == -2)
        ++jnode;
    }

    *nnodej = jnode;
  }

  else
  {
    jnode = 0;

    for (inode = 1; inode <= nnode; ++inode)
    {
      if (jnin[inode] == -1)
      {
        ++jnode;

        jnin[inode] = jnode;

        injn[jnode] = inode;

        xj[jnode][0] = x[inode][0];
        xj[jnode][1] = x[inode][1];
        xj[jnode][2] = x[inode][2];
      }
    }

    for (inode = 1; inode <= nnode; ++inode)
    {
      if (jnin[inode] == -2)
      {
        ++jnode;

        jnin[inode] = jnode;

        injn[jnode] = inode;

        xj[jnode][0] = x[inode][0];
        xj[jnode][1] = x[inode][1];
        xj[jnode][2] = x[inode][2];
      }
    }

    jelem = 0;

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (iviel[ielem] == iv)
      {
        ++jelem;

        inode1 = iniel[ielem][0];
        inode2 = iniel[ielem][1];
        inode3 = iniel[ielem][2];
        inode4 = iniel[ielem][3];

        jnjel[jelem][0] = jnin[inode1];
        jnjel[jelem][1] = jnin[inode2];
        jnjel[jelem][2] = jnin[inode3];
        jnjel[jelem][3] = jnin[inode4];
      }
    }

    if (idibf != NULL)
    {
      id = idibf[1];

      for (ibface = 2; ibface <= nbface; ++ibface)
      {
        id = MAX (id, idibf[ibface]);
      }

      ++id;
    }

    for (ibface = 0; ibface <= nbface; ++ibface)
    {
      mchkbf[ibface] = 0;
    }

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (iviel[ielem] == iv)
      {
        ibface1 = -ieliel[ielem][0];
        ibface2 = -ieliel[ielem][1];
        ibface3 = -ieliel[ielem][2];
        ibface4 = -ieliel[ielem][3];

        if (ibface1 > 0) ++(mchkbf[ibface1]);
        if (ibface2 > 0) ++(mchkbf[ibface2]);
        if (ibface3 > 0) ++(mchkbf[ibface3]);
        if (ibface4 > 0) ++(mchkbf[ibface4]);
      }
    }

    ibc = STD_UG3_GBC;
    jd = id;
    irf = 7;

    jbface = 0;

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (iviel[ielem] == iv)
      {
        ielem1 = ieliel[ielem][0];
        ielem2 = ieliel[ielem][1];
        ielem3 = ieliel[ielem][2];
        ielem4 = ieliel[ielem][3];

        inode1 = iniel[ielem][0];
        inode2 = iniel[ielem][1];
        inode3 = iniel[ielem][2];
        inode4 = iniel[ielem][3];

        if (ielem1 <= 0 || iviel[ielem1] != iv)
        {
          ibface = MAX (-ielem1, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnjbf[jbface][0] = jnin[inode2];
            jnjbf[jbface][1] = jnin[inode4];
            jnjbf[jbface][2] = jnin[inode3];

            if (ibface == 0)
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = id;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;
            }
            else if (mchkbf[ibface] == 2)
            {
              ++jd;

              if (ibcibf != NULL)
                jbcjbf[jbface] = ibcibf[ibface];

              if (idibf != NULL)
                jdjbf[jbface] = jd;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;

              mchkbf[ibface] = 0;
            }
            else
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = idibf[ibface];

              if (irfibf != NULL)
                jrfjbf[jbface] = irfibf[ibface];
            }

            mchkbf[ibface] = 0;
          }
        }

        if (ielem2 <= 0 || iviel[ielem2] != iv)
        {
          ibface = MAX (-ielem2, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnjbf[jbface][0] = jnin[inode1];
            jnjbf[jbface][1] = jnin[inode3];
            jnjbf[jbface][2] = jnin[inode4];

            if (ibface == 0)
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = id;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;
            }
            else if (mchkbf[ibface] == 2)
            {
              ++jd;

              if (ibcibf != NULL)
                jbcjbf[jbface] = ibcibf[ibface];

              if (idibf != NULL)
                jdjbf[jbface] = jd;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;

              mchkbf[ibface] = 0;
            }
            else
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = idibf[ibface];

              if (irfibf != NULL)
                jrfjbf[jbface] = irfibf[ibface];
            }

            mchkbf[ibface] = 0;
          }
        }

        if (ielem3 <= 0 || iviel[ielem3] != iv)
        {
          ibface = MAX (-ielem3, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnjbf[jbface][0] = jnin[inode1];
            jnjbf[jbface][1] = jnin[inode4];
            jnjbf[jbface][2] = jnin[inode2];

            if (ibface == 0)
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = id;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;
            }
            else if (mchkbf[ibface] == 2)
            {
              ++jd;

              if (ibcibf != NULL)
                jbcjbf[jbface] = ibcibf[ibface];

              if (idibf != NULL)
                jdjbf[jbface] = jd;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;

              mchkbf[ibface] = 0;
            }
            else
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = idibf[ibface];

              if (irfibf != NULL)
                jrfjbf[jbface] = irfibf[ibface];
            }

            mchkbf[ibface] = 0;
          }
        }

        if (ielem4 <= 0 || iviel[ielem4] != iv)
        {
          ibface = MAX (-ielem4, 0);

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            jnjbf[jbface][0] = jnin[inode1];
            jnjbf[jbface][1] = jnin[inode2];
            jnjbf[jbface][2] = jnin[inode3];

            if (ibface == 0)
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = id;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;
            }
            else if (mchkbf[ibface] == 2)
            {
              ++jd;

              if (ibcibf != NULL)
                jbcjbf[jbface] = ibcibf[ibface];

              if (idibf != NULL)
                jdjbf[jbface] = jd;

              if (irfibf != NULL)
                jrfjbf[jbface] = irf;

              mchkbf[ibface] = 0;
            }
            else
            {
              if (ibcibf != NULL)
                jbcjbf[jbface] = ibc;

              if (idibf != NULL)
                jdjbf[jbface] = idibf[ibface];

              if (irfibf != NULL)
                jrfjbf[jbface] = irfibf[ibface];
            }

            mchkbf[ibface] = 0;
          }
        }
      }
    }
  }

  return;

}
