#include "UG3_LIB.h"

void ug3_tbfd0
 (INT_ mbc,
  INT_ mtask,
  INT_ *nbface,
  INT_ *nnode,
  INT_ *nnodeb,
  INT_ *ntbface,
  INT_ *ntbnode,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * ibfin,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * iqibf,
  INT_1D * irfibf,
  INT_1D * jtbfitbf,
  INT_1D * jnin,
  INT_1D * libfin,
  DOUBLE_1D * del,
  DOUBLE_1D * df,
  DOUBLE_1D * ds,
  DOUBLE_4D * trv,
  DOUBLE_4D * trw,
  DOUBLE_3D * x)

{

/*
 * Perform primary tasks required to create duplicate faces and nodes for
 * transparent boundary surface faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_tbfd0.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibctransp1, ibctransp2, ibe, ibface, ibface1, ibface2, ibface3,
       ibfn1, ibfn2, ibfn3, id, idmax, idmin, inode, inode1, inode2, inode3,
       iquad, iqmax, iqmin, jbctransp1, jbctransp2, jbe, jbface, jbfn1, jbfn2,
       jnode, jnode1, jnode2, jnode3, kbe, kbface, knode1, knode2, knode3, loc,
       loc1, loc2, nid, niq, pass;

  ibc = (mbc == -1) ? -1: 1;

  ibctransp1 = ibc * TRANSP_UG3_GBC;
  ibctransp2 = ibc * TRANSP_INTRNL_UG3_GBC;
  jbctransp1 = ibc * TMP_TRANSP_UG3_GBC;
  jbctransp2 = ibc * TMP_TRANSP_INTRNL_UG3_GBC;

//TASK 1: determine number of faces and nodes required to duplicate transparent
//boundary surface faces

  if (mtask == 1)
  {
    for (inode = 1; inode <= *nnodeb; ++inode)
    {
      jnin[inode] = 0;
    }

//  determine number of transparent boundary surface faces and
//  set flag at each node
//  jnin=0  : don't duplicate
//  jnin=-1 : don't duplicate (edge node of a transparent surface)
//  jnin=1  : duplicate

    *ntbface = 0;

    for (ibface = 1; ibface <= *nbface; ++ibface)
    {
      ibc = ibcibf[ibface];

      if (ibc == ibctransp1 || ibc == ibctransp2)
      {
        ++(*ntbface);

        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        jnin[inode1] = 1;
        jnin[inode2] = 1;
        jnin[inode3] = 1;
      }
    }

    for (ibface = 1; ibface <= *nbface; ++ibface)
    {
      ibc = ibcibf[ibface];

      if (ibc == ibctransp1 || ibc == ibctransp2)
      {
        ibface1 = ibfibf[ibface][0];
        ibface2 = ibfibf[ibface][1];
        ibface3 = ibfibf[ibface][2];

        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        if (ibface1 <= 0)
        {
          jnin[inode2] = -1;
          jnin[inode3] = -1;
        }

        if (ibface2 <= 0)
        {
          jnin[inode1] = -1;
          jnin[inode3] = -1;
        }

        if (ibface3 <= 0)
        {
          jnin[inode1] = -1;
          jnin[inode2] = -1;
        }
      }
    }

//  set flag to "duplicate" at all edge nodes on transparent surfaces that are
//  also shared with normal boundary surfaces
//  jnin=0  : don't duplicate
//  jnin=-1 : don't duplicate (edge node)
//  jnin=1  : duplicate

    if (*ntbface > 0)
    {
      for (ibface = 1; ibface <= *nbface; ++ibface)
      {
        ibc = ibcibf[ibface];

        if (ibc != ibctransp1 && ibc != ibctransp2)
        {
          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          jnin[inode1] = abs (jnin[inode1]);
          jnin[inode2] = abs (jnin[inode2]);
          jnin[inode3] = abs (jnin[inode3]);
        }
      }

//  set flag to "don't duplicate" at all corner edge nodes on transparent
//  surfaces that are also shared with normal boundary surfaces
//  jnin=0  : don't duplicate
//  jnin=-1 : don't duplicate (edge node)
//  jnin=-2 : don't duplicate (corner edge node)
//  jnin=1  : duplicate

      for (ibface = 1; ibface <= *nbface; ++ibface)
      {
        ibc = ibcibf[ibface];

        if (ibc == ibctransp1 || ibc == ibctransp2)
        {
          ibface1 = ibfibf[ibface][0];
          ibface2 = ibfibf[ibface][1];
          ibface3 = ibfibf[ibface][2];

          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          jnode1 = jnin[inode1];
          jnode2 = jnin[inode2];
          jnode3 = jnin[inode3];

          if (ibface1 <= 0)
          {
            if (jnode2 == 1 && jnode3 == -1)
              jnin[inode2] = -2;
            else if (jnode2 == -1 && jnode3 == 1)
              jnin[inode3] = -2;
          }

          if (ibface2 <= 0)
          {
            if (jnode1 == 1 && jnode3 == -1)
              jnin[inode1] = -2;
            else if (jnode1 == -1 && jnode3 == 1)
              jnin[inode3] = -2;
          }

          if (ibface3 <= 0)
          {
            if (jnode1 == 1 && jnode2 == -1)
              jnin[inode1] = -2;
            else if (jnode1 == -1 && jnode2 == 1)
              jnin[inode2] = -2;
          }
        }
      }

//    determine number of transparent boundary surface face nodes
//    and set node map for nodes that are duplicated

      jnode = *nnodeb;

      for (inode = 1; inode <= *nnodeb; ++inode)
      {
        if (jnin[inode] == 1)
        {
          ++jnode;

          jnin[inode] = jnode;
        }
        else
          jnin[inode] = inode;
      }

      *ntbnode = jnode - *nnodeb;
    }
    else
      *ntbnode = 0;
  }

//TASK 2: duplicate transparent boundary surface faces and modify all related
//grid data

  else
  {

//  shift location of node based data in the field so that duplicate boundary
//  data can be inserted in proper order

    for (inode = *nnode; inode > *nnodeb; --inode)
    {
      jnode = inode + (*ntbnode);

      if (del != NULL)  del[jnode] = del[inode];

      if (df != NULL)  df[jnode] = df[inode];

      if (ds != NULL)  ds[jnode] = ds[inode];

      if (trv != NULL)
      {
        trv[jnode][0] = trv[inode][0];
        trv[jnode][1] = trv[inode][1];
        trv[jnode][2] = trv[inode][2];
        trv[jnode][3] = trv[inode][3];
      }

      if (trw != NULL)
      {
        trw[jnode][0] = trw[inode][0];
        trw[jnode][1] = trw[inode][1];
        trw[jnode][2] = trw[inode][2];
        trw[jnode][3] = trw[inode][3];
      }

      x[jnode][0] = x[inode][0];
      x[jnode][1] = x[inode][1];
      x[jnode][2] = x[inode][2];
    }

//  duplicate node based data

    for (inode = 1; inode <= *nnodeb; ++inode)
    {
      jnode = jnin[inode];

      if (jnode > inode)
      {
        if (del != NULL)  del[jnode] = del[inode];

        if (df != NULL)  df[jnode] = df[inode];

        if (ds != NULL)  ds[jnode] = ds[inode];

        if (trv != NULL)
        {
          trv[jnode][0] = trv[inode][0];
          trv[jnode][1] = trv[inode][1];
          trv[jnode][2] = trv[inode][2];
          trv[jnode][3] = trv[inode][3];
        }

        if (trw != NULL)
        {
          trw[jnode][0] = trw[inode][0];
          trw[jnode][1] = trw[inode][1];
          trw[jnode][2] = trw[inode][2];
          trw[jnode][3] = trw[inode][3];
        }

        x[jnode][0] = x[inode][0];
        x[jnode][1] = x[inode][1];
        x[jnode][2] = x[inode][2];
      }
    }

//  determine range of boundary surface face ID flags

    idmax = idibf[1];
    idmin = idibf[1];

    for (ibface = 2; ibface <= *nbface; ++ibface)
    {
      id = idibf[ibface];

      idmax = MAX (idmax, id);
      idmin = MIN (idmin, id);
    }

//  set boundary surface face ID offset for duplicated boundary surface faces

    nid = idmax - idmin + 1;

//  if there are quad boundary surface faces

    if (iqibf)
    {
//    determine range of quad boundary surface face indices

      iqmax = iqibf[1];
      iqmin = iqibf[1];

      for (ibface = 2; ibface <= *nbface; ++ibface)
      {
        iquad = iqibf[ibface];

        iqmax = MAX (iqmax, iquad);
        iqmin = MIN (iqmin, iquad);
      }

//    set boundary surface face ID offset for duplicated boundary surface faces

      niq = iqmax - iqmin + 1;
    }

//  duplicate boundary surface face data and
//  set map for duplicate and original boundary surface faces

    jbface = *nbface;

    for (ibface = 1; ibface <= *nbface; ++ibface)
    {
      ibc = ibcibf[ibface];

      if (ibc == ibctransp1 || ibc == ibctransp2)
      {
        ++jbface;

        ibcibf[jbface] = (ibc == ibctransp1) ? jbctransp1: jbctransp2;

        id = idibf[ibface];

        idibf[jbface] = id + nid;

        if (iqibf != NULL)
        {
          iquad = iqibf[ibface];

          if (iquad)
            iqibf[jbface] = iquad + niq;
          else
            iqibf[jbface] = 0;
        }

        irfibf[ibface] = 7;
        irfibf[jbface] = 7;

        jtbfitbf[ibface] = jbface;
        jtbfitbf[jbface] = ibface;

        ibfibf[jbface][0] = ibfibf[ibface][0];
        ibfibf[jbface][1] = ibfibf[ibface][2];
        ibfibf[jbface][2] = ibfibf[ibface][1];

        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        inibf[jbface][0] = jnin[inode1];
        inibf[jbface][1] = jnin[inode3];
        inibf[jbface][2] = jnin[inode2];
      }
      else
        jtbfitbf[ibface] = 0;
    }

//  correct boundary surface face neighbor map for duplicated boundary surface
//  faces

    for (ibface = (*nbface)+1; ibface <= (*nbface)+(*ntbface); ++ibface)
    {
      ibface1 = ibfibf[ibface][0];
      ibface2 = ibfibf[ibface][1];
      ibface3 = ibfibf[ibface][2];

      if (ibface1 > 0) ibfibf[ibface][0] = jtbfitbf[ibface1];
      if (ibface2 > 0) ibfibf[ibface][1] = jtbfitbf[ibface2];
      if (ibface3 > 0) ibfibf[ibface][2] = jtbfitbf[ibface3];
    }

//  correct boundary surface face neighbor map for duplicated boundary surface
//  faces on edges of transparent surfaces

    for (jbface = 1; jbface <= *nbface; ++jbface)
    {
      if (jtbfitbf[jbface] > 0)
      {
        for (jbe = 0; jbe <= 2; ++jbe)
        {
          if (ibfibf[jbface][jbe] == 0)
          {
            jbfn1 = (jbe < 2) ? jbe+1: 0;
            jbfn2 = (jbfn1 < 2) ? jbfn1+1: 0;

            jnode1 = inibf[jbface][jbfn1];
            jnode2 = inibf[jbface][jbfn2];

            loc1 = libfin[jnode1];
            loc2 = libfin[jnode1+1] - 1;

            pass = 1;

            do
            {
              ibe = -1;

              loc = loc1;

              do
              {
                ibface = ibfin[loc];

                if (ibface != jbface && jtbfitbf[ibface] == 0)
                {
                  inode1 = inibf[ibface][0];
                  inode2 = inibf[ibface][1];
                  inode3 = inibf[ibface][2];

                  if (pass == 1)
                    ibe = (inode1 == jnode2 && inode2 == jnode1) ? 2:
                          (inode2 == jnode2 && inode3 == jnode1) ? 0:
                          (inode3 == jnode2 && inode1 == jnode1) ? 1: -1;
                  else
                    ibe = (inode1 == jnode1 && inode2 == jnode2) ? 2:
                          (inode2 == jnode1 && inode3 == jnode2) ? 0:
                          (inode3 == jnode1 && inode1 == jnode2) ? 1: -1;
                }

                ++loc;
              }
              while (loc <= loc2 && ibe == -1);

              if (ibe >= 0)
              {
                if (pass == 1)
                {
                  ibfibf[jbface][jbe] = ibface;
                  ibfibf[ibface][ibe] = jbface;
                }
                else
                {
                  kbface = jtbfitbf[jbface];

                  if (jbe == 0)
                  {
                    kbe = 0;

                    knode1 = inibf[kbface][2];
                    knode2 = inibf[kbface][1];
                  }
                  else if (jbe == 1)
                  {
                    kbe = 2;

                    knode1 = inibf[kbface][1];
                    knode2 = inibf[kbface][0];
                  }
                  else
                  {
                    kbe = 1;

                    knode1 = inibf[kbface][0];
                    knode2 = inibf[kbface][2];
                  }

                  if (ibe == 0)
                  {
                    inibf[ibface][1] = knode1;
                    inibf[ibface][2] = knode2;
                  }
                  else if (ibe == 1)
                  {
                    inibf[ibface][2] = knode1;
                    inibf[ibface][0] = knode2;
                  }
                  else
                  {
                    inibf[ibface][0] = knode1;
                    inibf[ibface][1] = knode2;
                  }

                  ibfibf[kbface][kbe] = ibface;
                  ibfibf[ibface][ibe] = kbface;
                }
              }

              else
              {
                kbface = jtbfitbf[jbface];

                knode1 = inibf[kbface][0];
                knode2 = inibf[kbface][1];
                knode3 = inibf[kbface][2];

                kbe = (knode1 == jnode2 && knode2 == jnode1) ? 2:
                      (knode2 == jnode2 && knode3 == jnode1) ? 0:
                      (knode3 == jnode2 && knode1 == jnode1) ? 1: -1;

                if (kbe >= 0)
                {
                  ibfibf[jbface][jbe] = kbface;
                  ibfibf[kbface][kbe] = jbface;
                }

                pass = 2;
              }

              ++pass;
            }
            while (pass <= 2 && ibe >= 0);
          }
        }
      }
    }

//  correct boundary surface face node connectivity for normal boundary surface
//  faces that share a node with a transparent surface face

    for (jbface = 1; jbface <= *nbface; ++jbface)
    {
      if (jtbfitbf[jbface] == 0)
      {
        for (jbe = 0; jbe <= 2; ++jbe)
        {
          ibface = ibfibf[jbface][jbe];

          if (ibface > *nbface)
          {
            ibface = jbface;

            ibfn1 = (jbe < 2) ? jbe+1: 0;
            
            inode = inibf[ibface][ibfn1];

            if (inode > *nnodeb)
            {
              ibfn2 = (ibfn1 < 2) ? ibfn1+1: 0;
              ibfn3 = (ibfn2 < 2) ? ibfn2+1: 0;

              inode3 = inibf[ibface][ibfn3];

              kbface = ibface;
              ibface = ibfibf[ibface][ibfn2];

              loc = 0;

              while (loc <= *nbface && ibface != jbface && ibface != 0)
              {
                ibfn1 = (ibfibf[ibface][0] == kbface) ? 1:
                        (ibfibf[ibface][1] == kbface) ? 2: 0;
                ibfn2 = (ibfn1 < 2) ? ibfn1+1: 0;
                ibfn3 = (ibfn2 < 2) ? ibfn2+1: 0;

                inode1 = inibf[ibface][ibfn1];
                inode2 = inode3;
                inode3 = inibf[ibface][ibfn3];

                if (inode1 != inode)
                  inibf[ibface][ibfn1] = inode;

                kbface = ibface;
                ibface = ibfibf[ibface][ibfn2];

                ++loc;
              }
            }
          }
        }
      }
    }

//  set new number of nodes, boundary nodes, and boundary faces

    *nnode = *nnode + *ntbnode;

    *nnodeb = *nnodeb + *ntbnode;

    *nbface = *nbface + *ntbface;
  }

  return;

}
