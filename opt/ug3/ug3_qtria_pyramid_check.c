#include "UG3_LIB.h"

INT_ ug3_qtria_pyramid_check
 (INT_ mmsg,
  INT_ *nbface,
  INT_ nelemc5,
  INT_ *nnode,
  INT_ nquad,
  INT_1D * ibcibf,
  INT_1D * idibf,
  INT_1D * ierribf,
  INT_3D * inibf,
  INT_5D * inielc5,
  INT_4D * iniq,
  INT_1D * iqibf,
  INT_1D * irfibf,
  double cdfnqp,
  double tol,
  DOUBLE_1D * ds,
  DOUBLE_4D * trv,
  DOUBLE_3D * x)

{

/*
 * Check tria-faces created to replace quad-faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_qtria_pyramid_check.c,v 1.5 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_1D *ibfichk = NULL;
  INT_1D *ibfin = NULL;
  INT_1D *ibfdel = NULL;
  INT_1D *jnin = NULL;
  INT_1D *libfin = NULL;
  INT_1D *mchkbf = NULL;
  INT_1D *nmerge = NULL;
  INT_3D *ibfibf = NULL;

  INT_ ibface, ibfacei, ibface2, ibface3, ibfn2, ibfn3, 
       idel, ielemc5, ierr,
       inode, inode1, inode2, inode3, inode4, inode5, 
       ipass, iquad, 
       jbface, jbfacei, 
       jbface2, jbface3,
       jbfn1, jbfn2, jbfn3,
       jnode, jnode1, jnode2, jnode3, jnode4, 
       kbface, 
       loc, loc1, loc2,
       merr, mmultc, mreorder,
       nbfacei, nbfpnt, nbfpntd, nnodei;

  double cdfnqps, cdx, cmod1, cmod2, dc0, dc1, dc1d2, dc1d4, dx, 
         dx1, dx2, dx3, 
         dx011, dx012, dx013, dx01s, 
         dx211, dx212, dx213, 
         dx311, dx312, dx313, 
         dx321, dx322, dx323, 
         dx411, dx412, dx413, dx41s, 
         x1max, x2max, x3max, 
         x1min, x2min, x3min, 
         voltol, w, w1, w2, 
         x01, x02, x03, 
         x11, x12, x13, 
         x21, x22, x23, 
         x31, x32, x33, 
         x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc1d2 = 0.5;
  dc1d4 = 0.25;

  cmod1 = 0.8;
  cmod2 = dc1 - cmod1;

  cdfnqps = cdfnqp * cdfnqp;

  if (mmsg == 2)
  {
    sprintf (Text, "Checking Pyramids  : Quad-, Tria-BFaces=%10i%10i", nquad, *nbface);
    ug_message (Text);
    sprintf (Text, "Checking Pyramids  : Nodes             =%10i", *nnode);
    ug_message (Text);
  }

  merr = 1;
  mmultc = 1;
  mreorder = 0;
  nbfpntd = 0;

  // allocate space for work arrays

  ierr = 0;

  ibfdel = (INT_1D *) ug_malloc (&ierr, (*nbface+1) * sizeof (INT_1D));
  ibfibf = (INT_3D *) ug_malloc (&ierr, (*nbface+1) * sizeof (INT_3D));
  ibfin = (INT_1D *) ug_malloc (&ierr, (nbfpntd+1) * sizeof (INT_1D));
  jnin = (INT_1D *) ug_malloc (&ierr, (*nnode+1) * sizeof (INT_1D));
  libfin = (INT_1D *) ug_malloc (&ierr, (*nnode+2) * sizeof (INT_1D));
  nmerge = (INT_1D *) ug_malloc (&ierr, (*nnode+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (ibfdel);
    ug_free (ibfibf);
    ug_free (ibfin);
    ug_free (jnin);
    ug_free (libfin);
    ug_free (nmerge);
    return (100304);
  }

  // set tria-face per node map

  ierr = ug3_ibfin (*nbface, &nbfpntd, *nnode, &nbfpnt,
                    inibf, &ibfin, libfin);

  if (ierr > 0)
  {
    ug_free (ibfdel);
    ug_free (ibfibf);
    ug_free (ibfin);
    ug_free (jnin);
    ug_free (libfin);
    ug_free (nmerge);
    return (ierr);
  }

  // set tria-face neighbor connectivity map

  ierr = ug3_ibfibf (merr, mmsg, mmultc, mreorder, *nbface,
                     ibcibf, ibfin, inibf, irfibf, libfin, ibfichk, mchkbf,
                     ibfibf, ierribf, x);

  if (ierr > 0)
  {
    ug_free (ibfdel);
    ug_free (ibfibf);
    ug_free (ibfin);
    ug_free (jnin);
    ug_free (libfin);
    ug_free (nmerge);
    return (ierr);
  }

  // initialize tria-face deletion flag

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    ibfdel[ibface] = 0;
  }

  // initialize node map and merged node counter

  for (inode = 1; inode <= *nnode; ++inode)
  {
    jnin[inode] = 0;

    nmerge[inode] = 1;
  }

  // check tria-faces created to replace quad-faces

  idel = 0;

  for (ipass = 1; ipass <= 2; ++ipass)
  {
    for (ibface = 1; ibface <= *nbface; ++ibface)
    {
      // check face if it was created to replace a quad-face
      // and if it has not been deleted

      iquad = -iqibf[ibface];

      if (iquad > 0 && ibfdel[ibface] == 0)
      {
        jbface = ibfibf[ibface][2];

        // check face on pass 1 if the neighbor was created to replace a
        // quad-face and it has not been checked yet or
        // check face on pass 2 if the neighbor was not created to replace a
        // quad-face 

        if ((ipass == 1 && jbface > ibface && iqibf[jbface] < 0) || 
            (ipass == 2 && iqibf[jbface] >= 0))
        {
          jbfn1 = (ibfibf[jbface][0] == ibface) ? 0:
                  (ibfibf[jbface][1] == ibface) ? 1: 2;

          inode1 = inibf[ibface][2];
          inode2 = inibf[ibface][0];
          inode3 = inibf[ibface][1];
          inode4 = inibf[jbface][jbfn1];

          x11 = x[inode1][0];
          x12 = x[inode1][1];
          x13 = x[inode1][2];
          x21 = x[inode2][0];
          x22 = x[inode2][1];
          x23 = x[inode2][2];
          x31 = x[inode3][0];
          x32 = x[inode3][1];
          x33 = x[inode3][2];
          x41 = x[inode4][0];
          x42 = x[inode4][1];
          x43 = x[inode4][2];

          dx321 = x31 - x21;
          dx322 = x32 - x22;
          dx323 = x33 - x23;
          dx411 = x41 - x11;
          dx412 = x42 - x12;
          dx413 = x43 - x13;

          // check distance between the node created to replace a quad-face and
          // its opposing neighbor
          // if the distance is less than allowable then delete the node

          cdx = cdfnqps * (dx321 * dx321 + dx322 * dx322 + dx323 * dx323);

          w = dx411 * dx411 + dx412 * dx412 + dx413 * dx413 - cdx;

          if (w >= dc0)
          {
            x1max = MAX (x11, x21);
            x1max = MAX (x31, x1max);
            x1max = MAX (x41, x1max);
            x2max = MAX (x12, x22);
            x2max = MAX (x32, x2max);
            x2max = MAX (x42, x2max);
            x3max = MAX (x13, x23);
            x3max = MAX (x33, x3max);
            x3max = MAX (x43, x3max);
            x1min = MIN (x11, x21);
            x1min = MIN (x31, x1min);
            x1min = MIN (x41, x1min);
            x2min = MIN (x12, x22);
            x2min = MIN (x32, x2min);
            x2min = MIN (x42, x2min);
            x3min = MIN (x13, x23);
            x3min = MIN (x33, x3min);
            x3min = MIN (x43, x3min);

            dx1 = x1max - x1min;
            dx2 = x2max - x2min;
            dx3 = x3max - x3min;

            dx = MAX (dx1, dx2);
            dx = MAX (dx3, dx);

            dx211 = x21 - x11;
            dx212 = x22 - x12;
            dx213 = x23 - x13;
            dx311 = x31 - x11;
            dx312 = x32 - x12;
            dx313 = x33 - x13;

            // check volume formed between neighbor faces
            // if the volume is below tolerance then delete the node

            voltol = tol * dx * dx * dx;

            w = dx411 * (dx212 * dx313 - dx213 * dx312)
              + dx412 * (dx213 * dx311 - dx211 * dx313)
              + dx413 * (dx211 * dx312 - dx212 * dx311) - voltol;
          }

          // on pass 1 delete the node created to replace a quad-face

          if (ipass == 1 && w < dc0)
          {
            ++idel;

            // average the opposing neighbor node with the node to be deleted

            w1 = (double) nmerge[inode4];
            w2 = dc1 / (w1 + dc1);
            w1 = w1 / (w1 + dc1);

            x[inode4][0] = w1 * x11 + w2 * x41;
            x[inode4][1] = w1 * x12 + w2 * x42;
            x[inode4][2] = w1 * x13 + w2 * x43;

            ++(nmerge[inode4]);

            // set node map for node to be deleted

            jnin[inode1] = -inode4;

            // for all tria-faces attached to the node to be deleted
            // replace node index with that of the opposing neighbor node

            loc1 = libfin[inode1];
            loc2 = libfin[inode1+1]-1;

            for (loc = loc1; loc <= loc2; ++loc)
            {
              kbface = ibfin[loc];

              if (ibfdel[kbface] == 0)
              {
                if (inibf[kbface][0] == inode1)
                  inibf[kbface][0] = inode4;
                else if (inibf[kbface][1] == inode1)
                  inibf[kbface][1] = inode4;
                else
                  inibf[kbface][2] = inode4;
              }
            }

            // label tria-faces to be deleted

            ibfdel[ibface] = 1;
            ibfdel[jbface] = 1;

            // check neighbor tria-faces for possible deletion

            jbfn2 = (jbfn1 > 0) ? jbfn1-1: 2;

            ibfacei = ibface;
            jbfacei = jbface;
            ibface2 = ibfibf[ibface][0];
            jbface2 = ibfibf[jbface][jbfn2];

            // if the neighbor tria-faces are direct neighbors of each other
            // then they must be deleted and their neighbors checked

            do
            {
              jbfn2 = (ibfibf[jbface2][0] == ibface2) ? 2:
                      (ibfibf[jbface2][1] == ibface2) ? 0:
                      (ibfibf[jbface2][2] == ibface2) ? 1: -1;

              if (jbfn2 >= 0)
              {
                ibfacei = ibface2;
                jbfacei = jbface2;

                // label tria-faces to be deleted

                ibfdel[ibfacei] = 1;
                ibfdel[jbfacei] = 1;

                ibfn2 = (ibfibf[ibfacei][0] == jbfacei) ? 1:
                        (ibfibf[ibfacei][1] == jbfacei) ? 2: 0;

                ibface2 = ibfibf[ibfacei][ibfn2];
                jbface2 = ibfibf[jbfacei][jbfn2];
              }
            }
            while (jbfn2 >= 0);

            // reset tria-face neighbor connectivity

            if (ibfibf[ibface2][0] == ibfacei)
              ibfibf[ibface2][0] = jbface2;
            else if (ibfibf[ibface2][1] == ibfacei)
              ibfibf[ibface2][1] = jbface2;
            else
              ibfibf[ibface2][2] = jbface2;

            if (ibfibf[jbface2][0] == jbfacei)
              ibfibf[jbface2][0] = ibface2;
            else if (ibfibf[jbface2][1] == jbfacei)
              ibfibf[jbface2][1] = ibface2;
            else
              ibfibf[jbface2][2] = ibface2;

            // check neighbor tria-faces for possible deletion

            jbfn3 = (jbfn1 < 2) ? jbfn1+1: 0;

            ibfacei = ibface;
            jbfacei = jbface;
            ibface3 = ibfibf[ibface][1];
            jbface3 = ibfibf[jbface][jbfn3];

            // if the neighbor tria-faces are direct neighbors of each other
            // then they must be deleted and their neighbors checked

            do
            {
              jbfn3 = (ibfibf[jbface3][0] == ibface3) ? 1:
                      (ibfibf[jbface3][1] == ibface3) ? 2:
                      (ibfibf[jbface3][2] == ibface3) ? 0: -1;

              if (jbfn3 >= 0)
              {
                ibfacei = ibface3;
                jbfacei = jbface3;

                // label tria-faces to be deleted

                ibfdel[ibfacei] = 1;
                ibfdel[jbfacei] = 1;

                ibfn3 = (ibfibf[ibfacei][0] == jbfacei) ? 2:
                        (ibfibf[ibfacei][1] == jbfacei) ? 0: 1;

                ibface3 = ibfibf[ibfacei][ibfn3];
                jbface3 = ibfibf[jbfacei][jbfn3];
              }
            }
            while (jbfn3 >= 0);

            // reset tria-face neighbor connectivity

            if (ibfibf[ibface3][0] == ibfacei)
              ibfibf[ibface3][0] = jbface3;
            else if (ibfibf[ibface3][1] == ibfacei)
              ibfibf[ibface3][1] = jbface3;
            else
              ibfibf[ibface3][2] = jbface3;

            if (ibfibf[jbface3][0] == jbfacei)
              ibfibf[jbface3][0] = ibface3;
            else if (ibfibf[jbface3][1] == jbfacei)
              ibfibf[jbface3][1] = ibface3;
            else
              ibfibf[jbface3][2] = ibface3;
          }

          // on pass 2 modify the node created to replace a quad-face

          else if (ipass == 2 && w < dc0)
          {
            jnode1 = iniq[iquad][0];
            jnode2 = iniq[iquad][1];
            jnode3 = iniq[iquad][2];
            jnode4 = iniq[iquad][3];

            // average of quad coordinates

            x01 = dc1d4 * (x[jnode1][0] + x[jnode2][0] + x[jnode3][0] + x[jnode4][0]);
            x02 = dc1d4 * (x[jnode1][1] + x[jnode2][1] + x[jnode3][1] + x[jnode4][1]);
            x03 = dc1d4 * (x[jnode1][2] + x[jnode2][2] + x[jnode3][2] + x[jnode4][2]);

            // continue modifying node until position is acceptable

            do
            {
              x11 = cmod1 * x11 + cmod2 * x01;
              x12 = cmod1 * x12 + cmod2 * x02;
              x13 = cmod1 * x13 + cmod2 * x03;

              dx011 = x01 - x11;
              dx012 = x02 - x12;
              dx013 = x03 - x13;
              dx411 = x41 - x11;
              dx412 = x42 - x12;
              dx413 = x43 - x13;

              dx01s = dx011 * dx011 + dx012 * dx012 + dx013 * dx013;
              dx41s = dx411 * dx411 + dx412 * dx412 + dx413 * dx413;

              // check distance between the node created to replace a quad-face
              // and its opposing neighbor
              // if the distance is less than allowable then continue modifying
              // the node

              w = dx41s - cdx;

              if (w >= dc0 || dx41s >= dx01s)
              {
                dx211 = x21 - x11;
                dx212 = x22 - x12;
                dx213 = x23 - x13;
                dx311 = x31 - x11;
                dx312 = x32 - x12;
                dx313 = x33 - x13;

                // check volume formed between neighbor faces
                // if the volume is below tolerance then continue modifying the
                // node

                w = dx411 * (dx212 * dx313 - dx213 * dx312)
                  + dx412 * (dx213 * dx311 - dx211 * dx313)
                  + dx413 * (dx211 * dx312 - dx212 * dx311) - voltol;
              }
            }
            while (w < dc0);

            x[inode1][0] = x11;
            x[inode1][1] = x12;
            x[inode1][2] = x13;
          }
        }
      }
    }
  }

  // reorder data if any nodes and tria-faces were deleted

  if (idel)
  {
    // remove nodes that were deleted and reorder node data

    jnode = 0;

    for (inode = 1; inode <= *nnode; ++inode)
    {
      if (jnin[inode] == 0)
      {
        ++jnode;

        jnin[inode] = jnode;

        x[jnode][0] = x[inode][0];
        x[jnode][1] = x[inode][1];
        x[jnode][2] = x[inode][2];

        if (ds)
          ds[jnode] = ds[inode];

        if (trv)
        {
          trv[jnode][0] = trv[inode][0];
          trv[jnode][1] = trv[inode][1];
          trv[jnode][2] = trv[inode][2];
          trv[jnode][3] = trv[inode][3];
        }
      }
    }

    nnodei = *nnode;

    *nnode = jnode;

    // set reordered node map for nodes replaced by their opposing neighbor node

    for (inode = 1; inode <= nnodei; ++inode)
    {
      jnode = jnin[inode];

      while (jnode < 0)
      {
        jnode = jnin[-jnode];
      }

      jnin[inode] = jnode;
    }

    // remove tria-faces that were deleted and reorder tria-face data

    jbface = 0;

    for (ibface = 1; ibface <= *nbface; ++ibface)
    {
      if (ibfdel[ibface] == 0)
      {
        ++jbface;

        if (jbface < ibface)
        {
          ibcibf[jbface] = ibcibf[ibface];
          idibf[jbface] = idibf[ibface];
          iqibf[jbface] = iqibf[ibface];
          irfibf[jbface] = irfibf[ibface];
        }

        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        inibf[jbface][0] = jnin[inode1];
        inibf[jbface][1] = jnin[inode2];
        inibf[jbface][2] = jnin[inode3];
      }
    }

    nbfacei = *nbface;

    *nbface = jbface;

    for (iquad = 1; iquad <= nquad; ++iquad)
    {
      ibcibf[*nbface+iquad] = ibcibf[nbfacei+iquad];
      idibf[*nbface+iquad] = idibf[nbfacei+iquad];
      irfibf[*nbface+iquad] = irfibf[nbfacei+iquad];
    }

    // reset quad-face node indicies

    for (iquad = 1; iquad <= nquad; ++iquad)
    {
      inode1 = iniq[iquad][0];
      inode2 = iniq[iquad][1];
      inode3 = iniq[iquad][2];
      inode4 = iniq[iquad][3];

      iniq[iquad][0] = jnin[inode1];
      iniq[iquad][1] = jnin[inode2];
      iniq[iquad][2] = jnin[inode3];
      iniq[iquad][3] = jnin[inode4];
    }

    // reset pyramid-element node indicies

    for (ielemc5 = 1; ielemc5 <= nelemc5; ++ielemc5)
    {
      inode1 = inielc5[ielemc5][0];
      inode2 = inielc5[ielemc5][1];
      inode3 = inielc5[ielemc5][2];
      inode4 = inielc5[ielemc5][3];
      inode5 = inielc5[ielemc5][4];

      inielc5[ielemc5][0] = jnin[inode1];
      inielc5[ielemc5][1] = jnin[inode2];
      inielc5[ielemc5][2] = jnin[inode3];
      inielc5[ielemc5][3] = jnin[inode4];
      inielc5[ielemc5][4] = jnin[inode5];
    }
  }

  ug_free (ibfdel);
  ug_free (ibfibf);
  ug_free (ibfin);
  ug_free (libfin);
  ug_free (jnin);
  ug_free (nmerge);

  if (mmsg == 2)
  {
    sprintf (Text, "Checking Pyramids  : Tria-BFaces       =%10i", *nbface);
    ug_message (Text);
    sprintf (Text, "Checking Pyramids  : Nodes             =%10i", *nnode);
    ug_message (Text);
  }

  return (0);
}
