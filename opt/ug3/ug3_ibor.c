#include "UG3_LIB.h"

INT_ ug3_ibor
 (INT_ mclosed,
  INT_ mmsg,
  INT_ mopen,
  INT_ nbface,
  INT_ nbo,
  INT_ nnode,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * ibfin,
  INT_1D * iboibf,
  INT_1D * ierribf,
  INT_3D * inibf,
  INT_1D * irfibf,
  INT_1D * libfin,
  DOUBLE_3D * x)

{

/*
 * Re-order the boundary surface grid connectivity if any boundary object does
 * not have RH orientation. 
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ibor.c,v 1.34 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  double bv[3], xmnmx[2][3];

  INT_ ibc, ibface, ibface1, ibface2, ibface3, ibfn, ibo, idir, ierr, imnmx,
       inode, inode2, inode3, irf0, irf1, irf2, irf3, jbo, jnode, loc, loc1,
       loc2, mbvls, mext, mtransp;

  double dc0, xm;

  dc0 = 0.0;

  if (iboibf == NULL)
    return (0);

  inode = 1;

  xmnmx[0][0] = x[inode][0];
  xmnmx[1][0] = x[inode][0];
  xmnmx[0][1] = x[inode][1];
  xmnmx[1][1] = x[inode][1];
  xmnmx[0][2] = x[inode][2];
  xmnmx[1][2] = x[inode][2];

  for (inode = 2; inode <= nnode; ++inode)
  {
    xmnmx[0][0] = MIN (xmnmx[0][0], x[inode][0]);
    xmnmx[1][0] = MAX (xmnmx[1][0], x[inode][0]);
    xmnmx[0][1] = MIN (xmnmx[0][1], x[inode][1]);
    xmnmx[1][1] = MAX (xmnmx[1][1], x[inode][1]);
    xmnmx[0][2] = MIN (xmnmx[0][2], x[inode][2]);
    xmnmx[1][2] = MAX (xmnmx[1][2], x[inode][2]);
  }

  ibc = STD_UG3_GBC;

  jbo = 0;

  for (ibo = 1; ibo <= nbo; ++ibo)
  {
    mtransp = 0;

    if (ibcibf != NULL)
    {
      ibface = 1;

      do
      {
        if (iboibf[ibface] == ibo)
        {
          ibc = ibcibf[ibface];

          mtransp = CHK_TRANSP_UG3_GBC (ibc);
        }

        ++ibface;
      }
      while (ibface <= nbface && mtransp == 0);
    }

    if (mtransp == 0)
    {
      mbvls = -1;

      idir = 0;

      do
      {
        imnmx = 0;

        do
        {
          jnode = 0;

          xm = xmnmx[1-imnmx][idir];

          for (ibface = 1; ibface <= nbface; ++ibface)
          {
            if (iboibf[ibface] == ibo)
            {
              for (ibfn = 0; ibfn <= 2; ++ibfn)
              {
                inode = inibf[ibface][ibfn];

                if ((imnmx == 0 && x[inode][idir] < xm) ||
                    (imnmx == 1 && x[inode][idir] > xm))
                {
                  jnode = inode;

                  xm = x[inode][idir];
                }
              }
            }
          }

          if (jnode > 0)
          {
            mbvls = ug3_bvls (0, 1, 0, inibf, &bv[0], &bv[1], &bv[2], x);

            loc1 = libfin[jnode];
            loc2 = libfin[jnode+1] - 1;

            for (loc = loc1; loc <= loc2; ++loc)
            {
              ibface = ibfin[loc];

              ibc = (ibcibf != NULL) ? ibcibf[ibface]: STD_UG3_GBC;

              if (mtransp == CHK_TRANSP_UG3_GBC (ibc))
                mbvls = ug3_bvls (ibface, 1, 1, inibf,
                                  &bv[0], &bv[1], &bv[2], x);
            }

            mbvls = ug3_bvls (0, 1, 2, inibf, &bv[0], &bv[1], &bv[2], x);

            mbvls = MAX (mbvls, 0);
          }

          ++imnmx;
        }
        while (imnmx <= 1 && mbvls != 0);

        ++idir;
      }
      while (idir <= 2 && mbvls != 0);

      if (mbvls != 0)
        return (312);

      --imnmx;

      --idir;

      mext = (xm == xmnmx[imnmx][idir] && mopen == 0) ? 1 : 0;

      if (mext == 1)
      {
        jbo = ibo;

        if (mmsg == 2)
        {
          sprintf (Text, "Boundary Objects   : External Object   =%10i", ibo);
          ug_message (Text);
        }
      }

      if ((mext != imnmx && bv[idir] < dc0) ||
          (mext == imnmx && bv[idir] > dc0))
      {
        if (mmsg == 2)
        {
          sprintf (Text, "Boundary Objects   : Re-Ordered Object =%10i", ibo);
          ug_message (Text);
        }

        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          if (iboibf[ibface] == ibo)
          {
            ibface2 = ibfibf[ibface][1];
            ibface3 = ibfibf[ibface][2];

            inode2 = inibf[ibface][1];
            inode3 = inibf[ibface][2];

            ibfibf[ibface][1] = ibface3;
            ibfibf[ibface][2] = ibface2;

            inibf[ibface][1] = inode3;
            inibf[ibface][2] = inode2;

            if (irfibf != NULL)
            {
              irf0 = irfibf[ibface];

              irf3 = irf0 / 4;
              irf0 = irf0 - 4 * irf3;
              irf2 = irf0 / 2;
              irf1 = irf0 - irf2 - irf2;

              irfibf[ibface] = irf1 + irf3 + irf3 + 4 * irf2;
            }
          }
        }
      }
    }
  }

  if (jbo == 0 && mopen == 0)
  {
    if (ibcibf == NULL)
      return (307);

    ibo = 1;

    do
    {
      mtransp = 1;

      ibface = 1;

      do
      {
        if (iboibf[ibface] == ibo)
        {
          ibc = ibcibf[ibface];

          mtransp = CHK_TRANSP_UG3_GBC (ibc);
        }

        ++ibface;
      }
      while (ibface <= nbface && mtransp == 1);

      if (mtransp == 1)
      {
        mext = 0;

        idir = 0;

        do
        {
          imnmx = 0;

          do
          {
            ibface = 1;

            do
            {
              if (iboibf[ibface] == ibo)
              {
                ibfn = 0;

                do
                {
                  inode = inibf[ibface][ibfn];

                  mext = (x[inode][idir] == xmnmx[imnmx][idir]) ? 1 : 0;

                  ++ibfn;
                }
                while (ibfn <= 2 && mext == 0);
              }

              ++ibface;
            }
            while (ibface <= nbface && mext == 0);

            ++imnmx;
          }
          while (imnmx <= 1 && mext == 0);

          ++idir;
        }
        while (idir <= 2 && mext == 0);

        if (mext == 1)
        {
          jbo = ibo;

          ierr = 0;

          if (ierribf != NULL)
          {
            for (ibface = 1; ibface <= nbface; ++ibface)
            {
              ierribf[ibface] = 0;
            }
          }

          for (ibface = 1; ibface <= nbface; ++ibface)
          {
            if (iboibf[ibface] == jbo)
            {
              ibface1 = ibfibf[ibface][0];
              ibface2 = ibfibf[ibface][1];
              ibface3 = ibfibf[ibface][2];

              if (ibface1 <= 0 || ibface2 <= 0 || ibface3 <= 0)
              {
                ierr = 1;

                if (ierribf == NULL)
                  return (333);
                else
                  ierribf[ibface] = -1;
              }
            }
          }

          if (ierr == 1)
            return (200317);

          if (ierribf != NULL)
          {
            for (ibface = 1; ibface <= nbface; ++ibface)
            {
              ierribf[ibface] = (iboibf[ibface] == jbo) ? -1: 0;
            }
          }

          if (ierribf == NULL)
            return (345);
          else
            return (200318);
        }
      }

      ++ibo;
    }
    while (ibo <= nbo && jbo == 0);

    return (307);
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibo = iboibf[ibface];

    if (ibo == 1)
      iboibf[ibface] = jbo;
    else if (ibo == jbo)
      iboibf[ibface] = 1;
  }

  if (mclosed)
  {
    ierr = 0;

    if (ierribf != NULL)
    {
      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        ierribf[ibface] = 0;
      }
    }

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ibo = iboibf[ibface];

      if (ibo == 1)
      {
        ibface1 = ibfibf[ibface][0];
        ibface2 = ibfibf[ibface][1];
        ibface3 = ibfibf[ibface][2];

        if (ibface1 <= 0 || ibface2 <= 0 || ibface3 <= 0)
        {
          ierr = 1;

          if (ierribf == NULL)
            return (333);
          else
            ierribf[ibface] = -1;
        }
      }
    }

    if (ierr == 1)
      return (200317);
  }

  return (0);

}

