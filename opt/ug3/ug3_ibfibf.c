#include "UG3_LIB.h"

INT_ ug3_ibfibf
 (INT_ merr,
  INT_ mmsg,
  INT_ mmultc,
  INT_ mreorder,
  INT_ nbface,
  INT_1D * ibcibf,
  INT_1D * ibfin,
  INT_3D * inibf,
  INT_1D * irfibf,
  INT_1D * libfin,
  INT_1D * ibfichk,
  INT_1D * mchkbf,
  INT_3D * ibfibf,
  INT_1D * ierribf,
  DOUBLE_3D * x)

{

/*
 * Determine boundary face to boundary face connectivity. Simply connected (no
 * more than two boundary faces per boundary edge) surfaces are reordered if all
 * faces do not have the same orientation.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ibfibf.c,v 1.47 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_1D *ibeie, *ibfie;

  INT_ ibc, ibe, ibface, ibface1, ibface2, ibface3, ibfit, ibfn1, ibfn2, ibfpe,
       ichk, ierr, imultc, inode1, inode2, inode3, ireorder, irf0, irf1, irf2,
       irf3, itransp, jbc, jbe, jbface, jbface1, jbface2, jbfn1, jbfn2, jbfpe,
       jerr, jnode1, jnode2, jnode3, jreorder, jtransp, kbe, kbface, kerr, loc,
       loc1, loc2, nbfit, nbfpe, nbfped, nchk;

  nbfit = nbface;

  kerr = 0;

  nbfped = 100;

  ibeie = (INT_1D *) ug_malloc (&kerr, nbfped * sizeof (INT_1D));
  ibfie = (INT_1D *) ug_malloc (&kerr, nbfped * sizeof (INT_1D));

  if (kerr)
  {
    ug_free (ibeie);
    ug_free (ibfie);

    return (100308);
  }

  if (merr)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ierribf[ibface] = 0;
    }
  }

  // initialize face neighbor connectivity

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibfibf[ibface][0] = 0;
    ibfibf[ibface][1] = 0;
    ibfibf[ibface][2] = 0;
  }

  ibc = 0;
  jbc = 0;

  ierr = 0;

  imultc = 0;

  jreorder = 0;

  // start of loop over boundary faces

  for (jbface = 1; jbface <= nbface; ++jbface)
  {
    if (ibcibf != NULL)
      jbc = abs (ibcibf[jbface]);

    jtransp = CHK_TRANSP_UG3_GBC (jbc);

    // start of loop over face neighbors

    for (jbe = 0; jbe <= 2; ++jbe)
    {
      // continue if no face neighbor has been set

      if (! ibfibf[jbface][jbe])
      {
        ibfpe = 0;

        ireorder = 0;

        jerr = 0;

        jbfn1 = (jbe < 2) ? jbe+1 : 0;
        jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;

        jnode1 = inibf[jbface][jbfn1];
        jnode2 = inibf[jbface][jbfn2];
        jnode3 = inibf[jbface][jbe];

        loc1 = libfin[jnode1];
        loc2 = libfin[jnode1 + 1] - 1;

        // loop over neighbor faces attached to one of face edge nodes

        for (loc = loc1; loc <= loc2; ++loc)
        {
          ibface = ibfin[loc];

          if (ibface != jbface)
          {
            if (ibcibf != NULL)
              ibc = abs (ibcibf[ibface]);

            itransp = CHK_TRANSP_UG3_GBC (ibc);

            // if face and face neighbor are either both non-transparent or 
            // both transparent then continue with check of face neighbor

            if (jtransp == itransp)
            {
              inode1 = inibf[ibface][0];
              inode2 = inibf[ibface][1];
              inode3 = inibf[ibface][2];

              // set index if face neighbor is found

              ibe = (inode1 == jnode2 && inode2 == jnode1) ? 2 :
                    (inode2 == jnode2 && inode3 == jnode1) ? 0 :
                    (inode3 == jnode2 && inode1 == jnode1) ? 1 : -1;

              kbe = ibe;

              // set index and if face neighbor is found and
              // set reorder flag if order is reversed

              if (ibe == -1)
              {
                ibe = (inode1 == jnode1 && inode2 == jnode2) ? 2 :
                      (inode2 == jnode1 && inode3 == jnode2) ? 0 :
                      (inode3 == jnode1 && inode1 == jnode2) ? 1 : -1;

                if (ibe >= 0)
                  ireorder = 1;
              }

              // count number of neighbor faces attached to the face edge
              // if number of neighbor faces is one (simply connected faces)
              // then set face neighbor
              // if there are no neighbor faces set free edge flag
              // if number of neighbor faces is greater than one (non-manifold
              // multiply connected faces) then set multiply connected flag

              if (ibe >= 0)
              {
                // if duplicate face found then set error flag

                if (inibf[ibface][ibe] == jnode3)
                {
                  if (merr)
                  {
                    ierr = 4;

                    ierribf[ibface] = -1;
                    ierribf[jbface] = -1;
                  }
                  else
                  {
                    ug_free (ibeie);
                    ug_free (ibfie);

                    return (330);
                  }
                }

                ++ibfpe;

                // set face neighbor data for first conected face

                if (ibfpe == 1)
                {
                  ibeie[ibfpe] = ibe;

                  if (kbe >= 0)
                    ibfie[ibfpe] = ibface;
                  else
                    ibfie[ibfpe] = -ibface;
                }

                // set face neighbor data for multiply conected faces
                // if boundary faces are not transparent and
                // if multiply connected option is on

                else if (mmultc && ! itransp)
                {
                  if (ibfpe >= nbfped)
                  {
                    kerr = 0;

                    nbfped = ibfpe + 10;

                    ibeie = (INT_1D *) ug_realloc (&kerr, ibeie,
                                                   nbfped
                                                 * sizeof (INT_1D));
                    ibfie = (INT_1D *) ug_realloc (&kerr, ibfie,
                                                   nbfped
                                                 * sizeof (INT_1D));

                    if (kerr)
                    {
                      ug_free (ibeie);
                      ug_free (ibfie);

                      return (100308);
                    }
                  }

                  ibeie[ibfpe] = ibe;

                  if (kbe >= 0)
                    ibfie[ibfpe] = ibface;
                  else
                    ibfie[ibfpe] = -ibface;
                }

                // set error flag
                // if boundary faces are not transparent and
                // if multiply connected option is off

                else if (! mmultc && ! itransp)
                {
                  if (merr)
                  {
                    jerr = 2;

                    ierribf[ibface] = -1;
                  }
                  else
                  {
                    ug_free (ibeie);
                    ug_free (ibfie);

                    return (308);
                  }
                }
              }
            }
          }
        }

        nbfpe = ibfpe;

        // if the connected faces are transparent then
        // set reconnection flag to off for face

        if (jtransp && irfibf != NULL)
          irfibf[jbface] = 7;

        // if the connected faces are transparent and multiply connected then
        // ignore connection between them and zero number of multiply connected
        // faces

        if (jtransp && nbfpe > 1)
          nbfpe = 0;

        // if multiply connected option and faces are multiply connected then
        // check connection between them

        if (mmultc && nbfpe > 1)
        {
          ++ibfpe;

          // if number of multiply connected faces is not even then set error
          // flag

          if (ibfpe % 2 != 0)
          {
            if (merr)
            {
              ierr = 3;

              ierribf[jbface] = -1;

              for (ibfpe = 1; ibfpe <= nbfpe; ++ibfpe)
              {
                ibface = abs (ibfie[ibfpe]);

                ierribf[ibface] = -1;
              }
            }

            else
            {
              ug_free (ibeie);
              ug_free (ibfie);

              return (309);
            }
          }

          // if number of multiply connected faces is even then turn off
          // reorder flag

          else
            ireorder = 0;

          // add starting face to list of connected faces

          ibfpe = 0;

          for (jbfpe = 1; jbfpe <= nbfpe; ++jbfpe)
          {
            ibface = ibfie[jbfpe];

            if (ibface > 0)
            {
              ++ibfpe;

              ibeie[ibfpe] = ibeie[jbfpe];
              ibfie[ibfpe] = ibface;
            }
          }

          nbfpe = ibfpe;
        }

        // if faces are simply connected then set face neighbor connectivity

        if (nbfpe == 1)
        {
          ibe = ibeie[1];

          ibface = ibfie[1];

          if (ibface < 0)
          {
            ibface = -ibface;

            if (! mreorder)
            {
              if (merr)
              {
                jerr = 1;

                ierribf[ibface] = -1;
              }
              else
              {
                ug_free (ibeie);
                ug_free (ibfie);

                return (306);
              }
            }
          }

          ibfibf[jbface][jbe] = ibface;
          ibfibf[ibface][ibe] = jbface;
        }

        // if faces are multiply connected then set reconnection flag to off
        // and set face neighbor connectivity between starting face and the
        // neighbor face with the smallest included angle

        else if (nbfpe > 1)
        {
          if (! jerr && mmultc)
          {
            ++imultc;

            ug3_ibfe (jnode1, jnode2, jnode3, nbfpe, &ibe, &ibface,
                      ibeie, ibfie, inibf, x);

            ibfibf[jbface][jbe] = ibface;
            ibfibf[ibface][ibe] = jbface;

            if (irfibf != NULL)
            {
              irf0 = irfibf[ibface];

              irf3 = irf0 / 4;
              irf0 = irf0 - 4 * irf3;
              irf2 = irf0 / 2;
              irf1 = irf0 - irf2 - irf2;

                   if (ibe == 0) irf1 = 1;
              else if (ibe == 1) irf2 = 1;
              else if (ibe == 2) irf3 = 1;

              irfibf[ibface] = irf1 + irf2 + irf2 + 4 * irf3;

              irf0 = irfibf[jbface];

              irf3 = irf0 / 4;
              irf0 = irf0 - 4 * irf3;
              irf2 = irf0 / 2;
              irf1 = irf0 - irf2 - irf2;

                   if (jbe == 0) irf1 = 1;
              else if (jbe == 1) irf2 = 1;
              else if (jbe == 2) irf3 = 1;

              irfibf[jbface] = irf1 + irf2 + irf2 + 4 * irf3;
            }
          }
          else if (jerr)
          {
            ierr = jerr;

            ierribf[jbface] = -1;
          }
        }

        if (ireorder)
          jreorder = 1;
      }
    }
  }

  // if error flag was set then exit

  if (ierr >= 1)
  {
    ug_free (ibeie);
    ug_free (ibfie);

    if (ierr == 1)
      return (200303);
    else if (ierr == 2)
      return (200309);
    else if (ierr == 3)
      return (200310);
    else
      return (200313);
  }

  // reorder boundary face connectivity so that all connected faces are
  // orientated in the same manner

  if (jreorder)
  {
    if (mmsg == 2)
    {
      sprintf (Text, "Re-ordering B-Faces: B-Faces           =%10i", nbface);
      ug_message (Text);
    }

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      mchkbf[ibface] = 0;
    }

    ibface = 1;
    jbface = 0;

    do
    {
      ibe = 0;

      do
      {
        jbface = ibfibf[ibface][ibe];

        if (jbface > 0)
        {
          ibfn1 = (ibe < 2) ? ibe+1 : 0;
          ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;

          inode1 = inibf[ibface][ibfn1];
          inode2 = inibf[ibface][ibfn2];

          mchkbf[ibface] = 1;
        }

        ++ibe;
      }
      while (ibe <= 2 && ! jbface);

      ++ibface;
    }
    while (ibface <= nbface && ! jbface);

    --ibface;

    if (jbface > 0)
    {
      ichk = 0;

      ibfit = 0;

      do
      {
        kbface = 0;

        jbe = 0;

        do
        {
          if (ibfibf[jbface][jbe] == ibface)
          {
            mchkbf[jbface] = 1;

            jbfn1 = (jbe < 2) ? jbe+1 : 0;
            jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;

            jbface1 = ibfibf[jbface][jbfn1];
            jbface2 = ibfibf[jbface][jbfn2];

            jnode1 = inibf[jbface][jbfn1];
            jnode2 = inibf[jbface][jbfn2];

            if (inode1 == jnode1)
            {
              ibfibf[jbface][jbfn1] = jbface2;
              ibfibf[jbface][jbfn2] = jbface1;

              inibf[jbface][jbfn1] = jnode2;
              inibf[jbface][jbfn2] = jnode1;

              ibface1 = jbface1;
              jbface1 = jbface2;
              jbface2 = ibface1;

              inode1 = jnode1;
              jnode1 = jnode2;
              jnode2 = inode1;

              if (irfibf != NULL)
              {
                irf0 = irfibf[jbface];

                irf3 = irf0 / 4;
                irf0 = irf0 - 4 * irf3;
                irf2 = irf0 / 2;
                irf1 = irf0 - irf2 - irf2;

                if (jbe == 0)
                  irfibf[jbface] = irf1 + irf3 + irf3 + 4 * irf2;
                else if (jbe == 1)
                  irfibf[jbface] = irf3 + irf2 + irf2 + 4 * irf1;
                else
                  irfibf[jbface] = irf2 + irf1 + irf1 + 4 * irf3;
              }
            }

            if (jbface1 > 0)
            {
              if (! mchkbf[jbface1])
              {
                inode1 = jnode2;
                inode2 = inibf[jbface][jbe];

                ibe = jbe;

                kbface = jbface;
                jbface = jbface1;
              }
            }

            if (jbface2 > 0)
            {
              if (! mchkbf[jbface2])
              {
                if (! kbface)
                {
                  inode1 = inibf[jbface][jbe];
                  inode2 = jnode1;

                  ibe = jbe;

                  kbface = jbface;
                  jbface = jbface2;
                }
                else
                {
                  ++ichk;

                  ibfichk[ichk] = jbface;
                }
              }
            }

            if (! kbface)
              kbface = -1;
          }

          ++jbe;
        }
        while (jbe <= 2 && ! kbface);

        if (kbface > 0)
          ibface = kbface;

        else if (kbface == -1)
        {
          ibface = 0;
          kbface = 0;

          nchk = ichk;

          if (nchk > 0)
          {
            ichk = 1;

            do
            {
              ibface = ibfichk[ichk];

              if (mchkbf[ibface])
              {
                ibe = 0;

                do
                {
                  jbface = ibfibf[ibface][ibe];

                  if (jbface > 0)
                  {
                    if (! mchkbf[jbface])
                    {
                      kbface = ibface;

                      ibfn1 = (ibe < 2) ? ibe+1 : 0;
                      ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;

                      inode1 = inibf[ibface][ibfn1];
                      inode2 = inibf[ibface][ibfn2];
                    }
                  }

                  ++ibe;
                }
                while (ibe <= 2 && ! kbface);
              }

              ++ichk;
            }
            while (ichk <= nchk && ! kbface);
          }

          ichk = 0;

          if (! kbface)
          {
            ibface = 1;

            do
            {
              if (mchkbf[ibface])
              {
                ibe = 0;

                do
                {
                  jbface = ibfibf[ibface][ibe];

                  if (jbface > 0)
                  {
                    if (! mchkbf[jbface])
                    {
                      kbface = ibface;

                      ibfn1 = (ibe < 2) ? ibe+1 : 0;
                      ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;

                      inode1 = inibf[ibface][ibfn1];
                      inode2 = inibf[ibface][ibfn2];
                    }
                  }

                  ++ibe;
                }
                while (ibe <= 2 && ! kbface);
              }

              ++ibface;
            }
            while (ibface <= nbface && ! kbface);
          }

          if (! kbface)
          {
            ibface = 1;
            jbface = 0;

            do
            {
              if (! mchkbf[ibface])
              {
                ibe = 0;

                do
                {
                  jbface = ibfibf[ibface][ibe];

                  if (jbface > 0)
                  {
                    kbface = ibface;

                    ibfn1 = (ibe < 2) ? ibe+1 : 0;
                    ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;

                    inode1 = inibf[ibface][ibfn1];
                    inode2 = inibf[ibface][ibfn2];

                    mchkbf[ibface] = 1;
                  }

                  ++ibe;
                }
                while (ibe <= 2 && ! jbface);
              }

              ++ibface;
            }
            while (ibface <= nbface && ! jbface);
          }

          ibface = kbface;
        }

        else
        {
          ug_free (ibeie);
          ug_free (ibfie);

          if (merr)
          {
            ierribf[ibface] = -1;
            ierribf[jbface] = -1;

            if (imultc)
              return (200311);
            else
              return (200315);
          }
          else
            return (336);
        }

        ++ibfit;
      }
      while (ibfit <= nbfit && ibface > 0);
    }
  }

  ug_free (ibeie);
  ug_free (ibfie);

  // zero connectivity between embedded/transparent faces that are connected
  // and have opposite connectivity orientation
  // this will split a surface that is like a mobius strip

  if (ibcibf != NULL)
  {
    for (jbface = 1; jbface <= nbface; ++jbface)
    {
      jbc = abs (ibcibf[jbface]);

      if (CHK_TRANSP_UG3_GBC (jbc))
      {
        for (jbe = 0; jbe <= 2; ++jbe)
        {
          ibface = ibfibf[jbface][jbe];

          if (ibface > 0)
          {
            jbfn1 = (jbe < 2) ? jbe+1 : 0;
            jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;

            jnode1 = inibf[jbface][jbfn1];
            jnode2 = inibf[jbface][jbfn2];

            inode1 = inibf[ibface][0];
            inode2 = inibf[ibface][1];
            inode3 = inibf[ibface][2];

            if (inode1 == jnode1 && inode2 == jnode2)
            {
              ibfibf[jbface][jbe] = 0;
              ibfibf[ibface][2] = 0;
            }
            else if (inode2 == jnode1 && inode3 == jnode2)
            {
              ibfibf[jbface][jbe] = 0;
              ibfibf[ibface][0] = 0;
            }
            else if (inode3 == jnode1 && inode1 == jnode2)
            {
              ibfibf[jbface][jbe] = 0;
              ibfibf[ibface][2] = 0;
            }
          }
        }
      }
    }

    for (jbface = 1; jbface <= nbface; ++jbface)
    {
      jbc = abs (ibcibf[jbface]);

      if (CHK_TRANSP_UG3_GBC (jbc))
      {
        for (jbe = 0; jbe <= 2; ++jbe)
        {
          ibface = ibfibf[jbface][jbe];

          if (ibface > 0)
          {
            jbfn1 = (jbe < 2) ? jbe+1 : 0;
            jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;

            jnode1 = inibf[jbface][jbfn1];
            jnode2 = inibf[jbface][jbfn2];

            inode1 = inibf[ibface][0];
            inode2 = inibf[ibface][1];
            inode3 = inibf[ibface][2];

            ibface1 = ibfibf[ibface][0];
            ibface2 = ibfibf[ibface][1];
            ibface3 = ibfibf[ibface][2];

            if ((inode1 == jnode2 && inode2 == jnode1 && ibface3 == 0) ||
                (inode2 == jnode2 && inode3 == jnode1 && ibface1 == 0) ||
                (inode3 == jnode2 && inode1 == jnode1 && ibface2 == 0))
              ibfibf[jbface][jbe] = 0;
          }
        }
      }
    }
  }

  return (0);

}
