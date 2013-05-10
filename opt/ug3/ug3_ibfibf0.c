#include "UG3_LIB.h"

void ug3_ibfibf0
 (INT_ msetbc,
  INT_ nbface,
  INT_1D * ibcibf,
  INT_1D * ibfin,
  INT_3D * inibf,
  INT_1D * libfin,
  INT_1D * ibfichk,
  INT_1D * mchkbf,
  INT_3D * ibfibf)

{

/*
 * Determine initial boundary face to boundary face connectivity.
 * Also automatically set the grid boundary condition flag for
 * embedded/transparent surfaces. If a surface has at least one edge that is
 * free then automatically set the grid boundary condition flag for that
 * surface to an embedded/transparent surface. An embedded surface is defined as
 * a set of connected faces that have one or more open outer edges and any other
 * outer edges connected to one or more faces of another surface. 
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ibfibf0.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibe, ibface, ibfit, ibfpe, ichk, ifree, imultc, inode1, inode2,
       inode3, ireorder,  jbe, jbface, jbfn1, jbfn2, jnode1, jnode2, kbe,
       kbface, loc, loc1, loc2, mfound, nbfit, nbfpe, nchk;

  nbfit = nbface;

  // initialize boundary face flag
  // initialize face neighbor connectivity

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    mchkbf[ibface] = 0;

    ibfibf[ibface][0] = 0;
    ibfibf[ibface][1] = 0;
    ibfibf[ibface][2] = 0;
  }

  ifree = 0;

  imultc = 0;

  ireorder = 0;

  // start of loop over boundary faces

  for (jbface = 1; jbface <= nbface; ++jbface)
  {
    // start of loop over face neighbors

    for (jbe = 0; jbe <= 2; ++jbe)
    {
      // continue if no face neighbor has been set

      if (! ibfibf[jbface][jbe])
      {
        ibfpe = 0;

        jbfn1 = (jbe < 2) ? jbe+1 : 0;
        jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;

        jnode1 = inibf[jbface][jbfn1];
        jnode2 = inibf[jbface][jbfn2];

        loc1 = libfin[jnode1];
        loc2 = libfin[jnode1 + 1] - 1;

        // start of loop over neighbor faces attached to one of the edge nodes

        for (loc = loc1; loc <= loc2; ++loc)
        {
          ibface = ibfin[loc];

          if (ibface != jbface)
          {
            inode1 = inibf[ibface][0];
            inode2 = inibf[ibface][1];
            inode3 = inibf[ibface][2];

            // set index if face neighbor is found

            ibe = (inode1 == jnode2 && inode2 == jnode1) ? 2 :
                  (inode2 == jnode2 && inode3 == jnode1) ? 0 :
                  (inode3 == jnode2 && inode1 == jnode1) ? 1 : -1;

            // set index and if face neighbor is found and
            // set reorder flag if order is reversed

            if (ibe == -1)
            {
              ibe = (inode1 == jnode1 && inode2 == jnode2) ? 2 :
                    (inode2 == jnode1 && inode3 == jnode2) ? 0 :
                    (inode3 == jnode1 && inode1 == jnode2) ? 1 : -1;

              if (ibe >= 0)
                ++ireorder;
            }

            // count number of neighbor faces attached to the face edge

            if (ibe >= 0)
            {
              ++ibfpe;

              kbe = ibe;

              kbface = ibface;
            }
          }

          // end of loop over neighbor faces attached to one of the edge nodes

        }

        nbfpe = ibfpe;

        // if number of neighbor faces is one (faces are simply connected)
        // then set face neighbor connectivity

        if (nbfpe == 1)
        {
          ibe = kbe;

          ibface = kbface;

          ibfibf[jbface][jbe] = ibface;
          ibfibf[ibface][ibe] = jbface;
        }

        // if there are no neighbor faces then set boundary face flag

        else if (! nbfpe)
        {
          ++ifree;

          mchkbf[jbface] = 1;
        }

        // if number of neighbor faces is greater than one (non-manifold/
        // multiply connected faces) then set boundary face flag and
        // set face neighbor connectivity

        else if (nbfpe > 1)
        {
          ++imultc;

          ibfibf[jbface][jbe] = -nbface;

          mchkbf[jbface] = -1;
        }
      }

      // end of loop over face eighbors

    }

    // end of loop over boundary faces

  }

  // if there are free edges then set embedded/transparent surfaces

  if (ifree >= 1)
  {
    ibfit = 1;

    // start of iterative loop to search for a faces attached to those with
    // a free edge

    do
    {
      // skip check for faces that are only connected to surfaces with free
      // edges on first iteration

      if (ibfit >= 2)
      {
        // start of loop over boundary faces

        for (jbface = 1; jbface <= nbface; ++jbface)
        {
          // if boundary face flag is not set then check to see if the face
          // is only connected to surfaces with free edges (face flag set)

          if (mchkbf[jbface] == -1)
          {
            mfound = 0;

            jbe = 0;

            // start of loop over face neighbors

            do
            {
              // continue if edge is multiply connected

              if (ibfibf[jbface][jbe] == -nbface)
              {
                mfound = 1;

                jbfn1 = (jbe < 2) ? jbe+1 : 0;
                jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;

                jnode1 = inibf[jbface][jbfn1];
                jnode2 = inibf[jbface][jbfn2];

                loc1 = libfin[jnode1];
                loc2 = libfin[jnode1 + 1] - 1;

                loc = loc1;

                // start of loop over neighbor faces attached to one of the
                // edge nodes

                do
                {
                  ibface = ibfin[loc];

                  if (ibface != jbface)
                  {
                    inode1 = inibf[ibface][0];
                    inode2 = inibf[ibface][1];
                    inode3 = inibf[ibface][2];

                    // if a face neighbor is found with a boundary face flag
                    // that is not part of a surface with a free edge then
                    // set loop flag for termination

                    if (((inode1 == jnode2 && inode2 == jnode1) ||
                         (inode2 == jnode2 && inode3 == jnode1) ||
                         (inode3 == jnode2 && inode1 == jnode1) ||
                         (inode1 == jnode1 && inode2 == jnode2) ||
                         (inode2 == jnode1 && inode3 == jnode2) ||
                         (inode3 == jnode1 && inode1 == jnode2)) &&
                        mchkbf[ibface] <= 0)
                      mfound = 0;
                  }

                  // end of loop over neighbor faces attached to one of the
                  // edge nodes
                  // if loop termination flag is set then end loop

                  ++loc;
                }
                while (loc <= loc2 && mfound == 1);
              }

              // end of loop over face eighbors
              // if loop termination flag is set then end loop

              ++jbe;
            }
            while (jbe <= 2 && mfound == 0);

            // set boundary face flag if the given face is only connected to
            // surfaces with free edges

            if (mfound == 1)
              mchkbf[jbface] = 1;
          }

          // end of loop over boundary faces
        }
      }

      mfound = 0;

      ibface = 1;

      // find an unchecked starting face with a free edge

      do
      {
        mfound = mchkbf[ibface];

        ++ibface;
      }
      while (ibface <= nbface && mfound != 1);

      --ibface;

      // if a starting face with a free edge was found then set boundary face
      // flag for all faces that are simply connected to the starting face

      nchk = 0;

      if (mfound == 1)
      {
        // initialize list of boundary faces to check

        ++nchk;

        ibfichk[nchk] = ibface;

        mchkbf[ibface] = 2;

        ichk = 1;

        // start of loop over boundary faces to check

        do
        {
          // set boundary face to check

          ibface = ibfichk[ichk];

          // start of loop over face neighbors

          for (ibe = 0; ibe <= 2; ++ibe)
          {
            jbface = ibfibf[ibface][ibe];

            // if face neighbor is found and it has not been checked then
            // add it to the list of boundary faces to check

            if (jbface > 0)
            {
              if (mchkbf[jbface] != 2)
              {
                mchkbf[jbface] = 2;

                ++nchk;

                ibfichk[nchk] = jbface;
              }
            }

            // set edge to free edge if it is a multiply connected edge

            else if (jbface == 0)
              ibfibf[ibface][ibe] = 0;

            // end of loop over face neighbors

          }

          // end of loop over boundary faces
          // if no boundary faces to check were added then end loop

          ++ichk;
        }
        while (ichk <= nchk);
      }

      // end of iterative loop to search for a face with a free edge

      ++ibfit;
    }
    while (ibfit <= nbfit && nchk > 0);

    ibc = (msetbc == 1) ? TRANSP_UG3_GBC: 
          (msetbc == 2) ? TRANSP_SRC_UG3_GBC:
                          TRANSP_INTRNL_UG3_GBC;

    // if the boundary face flag is set for a given face then
    // set grid BC flag to embedded/transparent for that face

    if (ibcibf != NULL)
    {
      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (mchkbf[ibface] >= 1)
        {
          if (ibcibf[ibface] >= 0)
            ibcibf[ibface] = ibc;
          else
            ibcibf[ibface] = -ibc;
        }
      }
    }
  }

  // re-set face neighbors at multiply connected edges

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    if (ibfibf[ibface][0] < 0) ibfibf[ibface][0] = 0;
    if (ibfibf[ibface][1] < 0) ibfibf[ibface][1] = 0;
    if (ibfibf[ibface][2] < 0) ibfibf[ibface][2] = 0;
  }

  return;

}
