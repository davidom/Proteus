#include "UG3_LIB.h"

void ug3_sgins
 (INT_ iv,
  INT_ nbface,
  INT_ nbfacej,
  INT_ *nelem,
  INT_ nelemj,
  INT_ nelemji,
  INT_ *nnode,
  INT_ nnodebj,
  INT_ nnodej,
  INT_ nnodeji,
  INT_4D * ieliel,
  INT_1D * ieljbf,
  INT_1D * ieljel,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_1D * injn,
  INT_1D * iviel,
  INT_1D * jeljbf,
  INT_4D * jeljel,
  INT_1D * jnin,
  INT_4D * jnjel,
  INT_1D * mchkbf,
  DOUBLE_3D * x,
  DOUBLE_3D * xj)

{

/*
 * Insert re-generated sub-grid into grid. Assumes sub-grid region was
 * originally extracted using routine ug3_subgrid.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_sgins.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ibface1, ibface2, ibface3, ibface4, ielem, ielem1, ielem2,
       ielem3, ielem4, inode, inode1, inode2, inode3, inode4, jbface, jbfacen,
       jelem, jelem1, jelem2, jelem3, jelem4, jnode, jnodei, jnode1, jnode2,
       jnode3, jnode4, knode, kelem, kelem1, kelem2, kelem3, kelem4, nbfn,
       nelbf, nelemjm, nnodejm;

  nbfn = nbface + nbfacej;

  nelbf = *nelem + nbface;

  nelemjm = MIN (nelemj, nelemji);

  nnodejm = MIN (nnodej, nnodeji);

  jnode = nnodebj;
  knode = 0;

  for (inode = 1; inode <= *nnode; ++inode)
  {
    jnodei = jnin[inode];

    if (jnodei > nnodebj)
    {
      ++jnode;

      if (jnode <= nnodejm)
      {
        ++knode;

        jnin[inode] = knode;
        injn[jnode] = knode;

        x[knode][0] = xj[jnode][0];
        x[knode][1] = xj[jnode][1];
        x[knode][2] = xj[jnode][2];
      }
    }
    else
    {
      ++knode;

      jnin[inode] = knode;

      if (jnodei > 0)
        injn[jnodei] = knode;

      if (inode > knode)
      {
        x[knode][0] = x[inode][0];
        x[knode][1] = x[inode][1];
        x[knode][2] = x[inode][2];
      }
    }
  }

  *nnode = knode;

  if (nnodej > nnodeji)
  {
    inode = *nnode;

    for (jnode = nnodeji+1; jnode <= nnodej; ++jnode)
    {
      ++inode;

      injn[jnode] = inode;
      jnin[inode] = inode;

      x[inode][0] = xj[jnode][0];
      x[inode][1] = xj[jnode][1];
      x[inode][2] = xj[jnode][2];
    }

    *nnode = inode;
  }

  for (ibface = 0; ibface <= nbface; ++ibface)
  {
    mchkbf[ibface] = 0;
  }

  for (ielem = 1; ielem <= *nelem; ++ielem)
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

  for (ielem = 1; ielem <= *nelem; ++ielem)
  {
    if (iviel[ielem] == iv)
    {
      ielem1 = ieliel[ielem][0];
      ielem2 = ieliel[ielem][1];
      ielem3 = ieliel[ielem][2];
      ielem4 = ieliel[ielem][3];

      if (ielem1 <= 0 || iviel[ielem1] != iv)
      {
        if (ielem1 > 0)
        {
          ++jbface;

          ieljbf[jbface] = ielem1;

          jbfacen = -jbface-nbfn;

               if (ieliel[ielem1][0] == ielem) ieliel[ielem1][0] = jbfacen;
          else if (ieliel[ielem1][1] == ielem) ieliel[ielem1][1] = jbfacen;
          else if (ieliel[ielem1][2] == ielem) ieliel[ielem1][2] = jbfacen;
          else                                 ieliel[ielem1][3] = jbfacen;
        }
        else
        {
          ibface = -ielem1;

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            ieljbf[jbface] = ielem1-nelbf;

            mchkbf[ibface] = 0;
          }
        }
      }

      if (ielem2 <= 0 || iviel[ielem2] != iv)
      {
        if (ielem2 > 0)
        {
          ++jbface;

          ieljbf[jbface] = ielem2;

          jbfacen = -jbface-nbfn;

               if (ieliel[ielem2][0] == ielem) ieliel[ielem2][0] = jbfacen;
          else if (ieliel[ielem2][1] == ielem) ieliel[ielem2][1] = jbfacen;
          else if (ieliel[ielem2][2] == ielem) ieliel[ielem2][2] = jbfacen;
          else                                 ieliel[ielem2][3] = jbfacen;
        }
        else
        {
          ibface = -ielem2;

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            ieljbf[jbface] = ielem2-nelbf;

            mchkbf[ibface] = 0;
          }
        }
      }

      if (ielem3 <= 0 || iviel[ielem3] != iv)
      {
        if (ielem3 > 0)
        {
          ++jbface;

          ieljbf[jbface] = ielem3;

          jbfacen = -jbface-nbfn;

               if (ieliel[ielem3][0] == ielem) ieliel[ielem3][0] = jbfacen;
          else if (ieliel[ielem3][1] == ielem) ieliel[ielem3][1] = jbfacen;
          else if (ieliel[ielem3][2] == ielem) ieliel[ielem3][2] = jbfacen;
          else                                 ieliel[ielem3][3] = jbfacen;
        }
        else
        {
          ibface = -ielem3;

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            ieljbf[jbface] = ielem3-nelbf;

            mchkbf[ibface] = 0;
          }
        }
      }

      if (ielem4 <= 0 || iviel[ielem4] != iv)
      {
        if (ielem4 > 0)
        {
          ++jbface;

          ieljbf[jbface] = ielem4;

          jbfacen = -jbface-nbfn;

               if (ieliel[ielem4][0] == ielem) ieliel[ielem4][0] = jbfacen;
          else if (ieliel[ielem4][1] == ielem) ieliel[ielem4][1] = jbfacen;
          else if (ieliel[ielem4][2] == ielem) ieliel[ielem4][2] = jbfacen;
          else                                 ieliel[ielem4][3] = jbfacen;
        }
        else
        {
          ibface = -ielem4;

          if (ibface == 0 || mchkbf[ibface] >= 1)
          {
            ++jbface;

            ieljbf[jbface] = ielem4-nelbf;

            mchkbf[ibface] = 0;
          }
        }
      }
    }
  }

  for (jelem = 1; jelem <= nelemj; ++jelem)
  {
    jelem1 = jeljel[jelem][0];
    jelem2 = jeljel[jelem][1];
    jelem3 = jeljel[jelem][2];
    jelem4 = jeljel[jelem][3];

    if (jelem1 < 0)
    {
      jbface = -jelem1;

      if (jelem != jeljbf[jbface])
      {
        ielem = ieljbf[jbface];

        if (ielem < 0)
          jeljel[jelem][0] = ielem;
      }
    }

    if (jelem2 < 0)
    {
      jbface = -jelem2;

      if (jelem != jeljbf[jbface])
      {
        ielem = ieljbf[jbface];

        if (ielem < 0)
          jeljel[jelem][1] = ielem;
      }
    }

    if (jelem3 < 0)
    {
      jbface = -jelem3;

      if (jelem != jeljbf[jbface])
      {
        ielem = ieljbf[jbface];

        if (ielem < 0)
          jeljel[jelem][2] = ielem;
      }
    }

    if (jelem4 < 0)
    {
      jbface = -jelem4;

      if (jelem != jeljbf[jbface])
      {
        ielem = ieljbf[jbface];

        if (ielem < 0)
          jeljel[jelem][3] = ielem;
      }
    }
  }

  for (jbface = 1; jbface <= nbfacej; ++jbface)
  {
    ielem = ieljbf[jbface];

    if (ielem == 0)
      jeljbf[jbface] = 0;

    else if (ielem < 0)
    {
      jelem = jeljbf[jbface];

           if (jeljel[jelem][0] == -jbface) jeljel[jelem][0] = ielem;
      else if (jeljel[jelem][1] == -jbface) jeljel[jelem][1] = ielem;
      else if (jeljel[jelem][2] == -jbface) jeljel[jelem][2] = ielem;
      else                                  jeljel[jelem][3] = ielem;

      ieljbf[jbface] = 0;
      jeljbf[jbface] = 0;
    }

    else //if (ielem > 0)
    {
      jelem = jeljbf[jbface];

      jbfacen = -jbface-nbfn;

           if (jeljel[jelem][0] == -jbface) jeljel[jelem][0] = jbfacen;
      else if (jeljel[jelem][1] == -jbface) jeljel[jelem][1] = jbfacen;
      else if (jeljel[jelem][2] == -jbface) jeljel[jelem][2] = jbfacen;
      else                                  jeljel[jelem][3] = jbfacen;
    }
  }

  ieljel[0] = 0;

  jelem = 0;

  for (ielem = 1; ielem <= *nelem; ++ielem)
  {
    if (iviel[ielem] == iv)
    {
      ++jelem;

      if (jelem <= nelemjm)
      {
        jnode1 = jnjel[jelem][0];
        jnode2 = jnjel[jelem][1];
        jnode3 = jnjel[jelem][2];
        jnode4 = jnjel[jelem][3];

        iniel[ielem][0] = injn[jnode1];
        iniel[ielem][1] = injn[jnode2];
        iniel[ielem][2] = injn[jnode3];
        iniel[ielem][3] = injn[jnode4];

        ieliel[ielem][0] = jeljel[jelem][0];
        ieliel[ielem][1] = jeljel[jelem][1];
        ieliel[ielem][2] = jeljel[jelem][2];
        ieliel[ielem][3] = jeljel[jelem][3];

        ieljel[jelem] = ielem;
      }
      else
        iniel[ielem][0] = -1;
    }
    else
    {
      inode1 = iniel[ielem][0];
      inode2 = iniel[ielem][1];
      inode3 = iniel[ielem][2];
      inode4 = iniel[ielem][3];

      iniel[ielem][0] = jnin[inode1];
      iniel[ielem][1] = jnin[inode2];
      iniel[ielem][2] = jnin[inode3];
      iniel[ielem][3] = jnin[inode4];
    }
  }

  if (nelemj > nelemji)
  {
    ielem = *nelem;

    for (jelem = nelemji+1; jelem <= nelemj; ++jelem)
    {
      ++ielem;

      jnode1 = jnjel[jelem][0];
      jnode2 = jnjel[jelem][1];
      jnode3 = jnjel[jelem][2];
      jnode4 = jnjel[jelem][3];

      iviel[ielem] = iv;

      iniel[ielem][0] = injn[jnode1];
      iniel[ielem][1] = injn[jnode2];
      iniel[ielem][2] = injn[jnode3];
      iniel[ielem][3] = injn[jnode4];

      ieliel[ielem][0] = jeljel[jelem][0];
      ieliel[ielem][1] = jeljel[jelem][1];
      ieliel[ielem][2] = jeljel[jelem][2];
      ieliel[ielem][3] = jeljel[jelem][3];

      ieljel[jelem] = ielem;
    }

    *nelem = ielem;
  }

  for (jelem = 1; jelem <= nelemj; ++jelem)
  {
    ielem = ieljel[jelem];

    jelem1 = ieliel[ielem][0];
    jelem2 = ieliel[ielem][1];
    jelem3 = ieliel[ielem][2];
    jelem4 = ieliel[ielem][3];

    ielem1 = (jelem1 > 0) ? ieljel[jelem1]: jelem1;
    ielem2 = (jelem2 > 0) ? ieljel[jelem2]: jelem2;
    ielem3 = (jelem3 > 0) ? ieljel[jelem3]: jelem3;
    ielem4 = (jelem4 > 0) ? ieljel[jelem4]: jelem4;

    ieliel[ielem][0] = ielem1;
    ieliel[ielem][1] = ielem2;
    ieliel[ielem][2] = ielem3;
    ieliel[ielem][3] = ielem4;
  }

  for (jbface = 1; jbface <= nbfacej; ++jbface)
  {
    jelem = jeljbf[jbface];

    jeljbf[jbface] = ieljel[jelem];
  }

  if (nelemj < nelemji)
  {
    kelem = 0;

    for (ielem = 1; ielem <= *nelem; ++ielem)
    {
      if (iniel[ielem][0] > 0)
      {
        ++kelem;

        ieljel[ielem] = kelem;

        if (ielem > kelem)
        {
          iviel[kelem] = iviel[ielem];

          iniel[kelem][0] = iniel[ielem][0];
          iniel[kelem][1] = iniel[ielem][1];
          iniel[kelem][2] = iniel[ielem][2];
          iniel[kelem][3] = iniel[ielem][3];

          ieliel[kelem][0] = ieliel[ielem][0];
          ieliel[kelem][1] = ieliel[ielem][1];
          ieliel[kelem][2] = ieliel[ielem][2];
          ieliel[kelem][3] = ieliel[ielem][3];
        }
      }
    }

    *nelem = kelem;

    for (kelem = 1; kelem <= *nelem; ++kelem)
    {
      ielem1 = ieliel[kelem][0];
      ielem2 = ieliel[kelem][1];
      ielem3 = ieliel[kelem][2];
      ielem4 = ieliel[kelem][3];

      kelem1 = (ielem1 > 0) ? ieljel[ielem1]: ielem1;
      kelem2 = (ielem2 > 0) ? ieljel[ielem2]: ielem2;
      kelem3 = (ielem3 > 0) ? ieljel[ielem3]: ielem3;
      kelem4 = (ielem4 > 0) ? ieljel[ielem4]: ielem4;

      ieliel[kelem][0] = kelem1;
      ieliel[kelem][1] = kelem2;
      ieliel[kelem][2] = kelem3;
      ieliel[kelem][3] = kelem4;
    }

    for (jbface = 1; jbface <= nbfacej; ++jbface)
    {
      ielem = ieljbf[jbface];

      ieljbf[jbface] = ieljel[ielem];

      ielem = jeljbf[jbface];

      jeljbf[jbface] = ieljel[ielem];
    }
  }

  for (jbface = 1; jbface <= nbfacej; ++jbface)
  {
    jbfacen = -jbface-nbfn;

    ielem = ieljbf[jbface];
    kelem = jeljbf[jbface];

    if (ielem > 0)
    {
           if (ieliel[ielem][0] == jbfacen) ieliel[ielem][0] = kelem;
      else if (ieliel[ielem][1] == jbfacen) ieliel[ielem][1] = kelem;
      else if (ieliel[ielem][2] == jbfacen) ieliel[ielem][2] = kelem;
      else                                  ieliel[ielem][3] = kelem;
    }

    if (kelem > 0)
    {
           if (ieliel[kelem][0] == jbfacen) ieliel[kelem][0] = ielem;
      else if (ieliel[kelem][1] == jbfacen) ieliel[kelem][1] = ielem;
      else if (ieliel[kelem][2] == jbfacen) ieliel[kelem][2] = ielem;
      else                                  ieliel[kelem][3] = ielem;
    }
  }

  for (ielem = 1; ielem <= *nelem; ++ielem)
  {
    ielem1 = ieliel[ielem][0];
    ielem2 = ieliel[ielem][1];
    ielem3 = ieliel[ielem][2];
    ielem4 = ieliel[ielem][3];

    ieliel[ielem][0] = (ielem1 < -nelbf) ? ielem1+nelbf: ielem1;
    ieliel[ielem][1] = (ielem2 < -nelbf) ? ielem2+nelbf: ielem2;
    ieliel[ielem][2] = (ielem3 < -nelbf) ? ielem3+nelbf: ielem3;
    ieliel[ielem][3] = (ielem4 < -nelbf) ? ielem4+nelbf: ielem4;
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

  return;

}
