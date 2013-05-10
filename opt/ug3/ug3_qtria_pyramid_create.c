#include "UG3_LIB.h"

INT_ ug3_qtria_pyramid_create
 (INT_ mmsg,
  INT_ mquadp,
  INT_ *nbface,
  INT_ *nbfaced,
  INT_ *nelemc5,
  INT_ *nnode,
  INT_ *nnoded,
  INT_ *nquad,
  INT_ *nquadp,
  double cdfqp,
  INT_1D ** ibcibf_ptr,
  INT_1D ** idibf_ptr,
  INT_1D ** ierribf_ptr,
  INT_3D ** inibf_ptr,
  INT_5D ** inielc5_ptr,
  INT_4D ** iniq_ptr,
  INT_1D ** iqibf_ptr,
  INT_1D ** irfibf_ptr,
  DOUBLE_1D ** ds_ptr,
  DOUBLE_4D ** trv_ptr,
  DOUBLE_3D ** x_ptr)

{

/*
 * Create pyramid elements to replace quad-faces with tria-faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_qtria_pyramid_create.c,v 1.8 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_1D *ibcibf, *idibf, *iqibf, *irfibf;
  INT_3D *inibf;
  INT_4D *iniq;
  INT_5D *inielc5;

  DOUBLE_1D *ds;
  DOUBLE_4D *trv;
  DOUBLE_3D *x;

  INT_ ierr, ibc, ibface, id, ielemc5,
       inode, inode1, inode2, inode3, inode4,
       iquad, jbface, jquad,
       jnode1, jnode2, jnode3, 
       knode1, knode2, knode3, 
       nbfacei, nbfacem, nnodei, nnodem;

  double b1, b2, b3, cdh, dc0, dc1d2, dc1d4, ds0,
         dx211, dx212, dx213, 
         dx311, dx312, dx313, 
         dx321, dx322, dx323, 
         dx411, dx412, dx413, 
         dx421, dx422, dx423, 
         dx431, dx432, dx433, 
         w, x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1d2 = 0.5;
  dc1d4 = 0.25;

  *nelemc5= 0;
  *nquad = 0;
  *nquadp = 0;

  if (mquadp == 0 || *iqibf_ptr == NULL)
    return (0);

  cdh = cdfqp * sqrt (dc1d2);

  ibcibf = *ibcibf_ptr;
  idibf = *idibf_ptr;
  iqibf = *iqibf_ptr;
  irfibf = *irfibf_ptr;
  inibf = *inibf_ptr;
  iniq = *iniq_ptr;
  inielc5 = *inielc5_ptr;
  ds = *ds_ptr;
  trv = *trv_ptr;
  x = *x_ptr;

  // count number of quad-faces on fixed surfaces that intersect the BL region
  // and on BL generating surfaces or optionally on all surfaces

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    if (iqibf[ibface] &&
        (ibc == FIXED_BL_INT_UG3_GBC ||CHK_BL_UG3_GBC (ibc) || mquadp == 1))
      ++(*nquadp);
  }

  *nquadp = *nquadp/2;

  if (*nquadp == 0)
    return (0);

  // set total number of quad-faces
  // note that quad-faces on transparent source surfaces have been converted to
  // sources and no longer exist 

  *nquad = 0;

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    *nquad = MAX (*nquad, iqibf[ibface]);
  }

  if (mmsg == 2)
  {
    sprintf (Text, "Creating Pyramids  : Quad-, Tria-BFaces=%10i%10i", *nquad, *nbface);
    ug_message (Text);
    sprintf (Text, "Creating Pyramids  : PyramidQuad-BFaces=%10i", *nquadp);
    ug_message (Text);
    sprintf (Text, "Creating Pyramids  : Nodes             =%10i", *nnode);
    ug_message (Text);
  }

  // set number faces, nodes and pyramids to be created 

  nbfacei = *nbface;
  nbfacem = *nbface + 4 * (*nquadp);

  *nelemc5 = *nquadp;

  nnodei = *nnode;
  nnodem = *nnode + (*nquadp);

  // allocate space for new faces, nodes and pyramids

  ierr = 0;

  if (nbfacem > *nbfaced)
  {
    *nbfaced = nbfacem;

    *ibcibf_ptr = (INT_1D *) ug_realloc (&ierr, *ibcibf_ptr, (*nbfaced+1) * sizeof (INT_1D));
    *idibf_ptr = (INT_1D *) ug_realloc (&ierr, *idibf_ptr, (*nbfaced+1) * sizeof (INT_1D));
    *ierribf_ptr = (INT_1D *) ug_realloc (&ierr, *ierribf_ptr, (*nbfaced+1) * sizeof (INT_1D));
    *iqibf_ptr = (INT_1D *) ug_realloc (&ierr, *iqibf_ptr, (*nbfaced+1) * sizeof (INT_1D));
    *irfibf_ptr = (INT_1D *) ug_realloc (&ierr, *irfibf_ptr, (*nbfaced+1) * sizeof (INT_1D));
    *inibf_ptr = (INT_3D *) ug_realloc (&ierr, *inibf_ptr, (*nbfaced+1) * sizeof (INT_3D));

    ibcibf = *ibcibf_ptr;
    idibf = *idibf_ptr;
    iqibf = *iqibf_ptr;
    irfibf = *irfibf_ptr;
    inibf = *inibf_ptr;
  }

  *inielc5_ptr = (INT_5D *) ug_realloc (&ierr, *inielc5_ptr, (*nelemc5+1) * sizeof (INT_5D));

  inielc5 = *inielc5_ptr;

  *iniq_ptr = (INT_4D *) ug_realloc (&ierr, *iniq_ptr, (*nquad+1) * sizeof (INT_4D));

  iniq = *iniq_ptr;

  if (nnodem > *nnoded)
  {
    *nnoded = nnodem;

    *x_ptr = (DOUBLE_3D *) ug_realloc (&ierr, *x_ptr, (*nnoded+1) * sizeof (DOUBLE_3D));

    x = *x_ptr;
  }

  *ds_ptr = (DOUBLE_1D *) ug_realloc (&ierr, *ds_ptr, (nnodem+1) * sizeof (DOUBLE_1D));
  *trv_ptr = (DOUBLE_4D *) ug_realloc (&ierr, *trv_ptr, (nnodem+1) * sizeof (DOUBLE_4D));

  ds = *ds_ptr;
  trv = *trv_ptr;

  if (ierr > 0)
   return (100317);

  // initialize quad-face connectivity

  for (iquad = 1; iquad <= *nquad; ++iquad)
  {
    iniq[iquad][0] = -1;
    iniq[iquad][1] = -1;
    iniq[iquad][2] = -1;
    iniq[iquad][3] = -1;
  }

  // reset quad-face connectivity as it may have changed for proper ordering

  for (ibface = 1; ibface <= nbfacei; ++ibface)
  {
    iquad = iqibf[ibface];

    if (iquad)
    {
      ibcibf[nbfacei+iquad] = ibcibf[ibface];
      idibf[nbfacei+iquad] = idibf[ibface];

      if (iniq[iquad][3] != 0)
      {
        iniq[iquad][0] = inibf[ibface][0];
        iniq[iquad][1] = inibf[ibface][1];
        iniq[iquad][2] = inibf[ibface][2];
        iniq[iquad][3] = 0;
      }
      else
      {
        jnode1 = iniq[iquad][0];
        jnode2 = iniq[iquad][1];
        jnode3 = iniq[iquad][2];

        knode1 = inibf[ibface][0];
        knode2 = inibf[ibface][1];
        knode3 = inibf[ibface][2];

             if (jnode1 == knode1 && jnode2 == knode3)
        {
          iniq[iquad][0] = jnode2;
          iniq[iquad][1] = jnode3;
          iniq[iquad][2] = jnode1;
          iniq[iquad][3] = knode2;
        }
        else if (jnode1 == knode2 && jnode2 == knode1)
        {
          iniq[iquad][0] = jnode2;
          iniq[iquad][1] = jnode3;
          iniq[iquad][2] = jnode1;
          iniq[iquad][3] = knode3;
        }
        else if (jnode1 == knode3 && jnode2 == knode2)
        {
          iniq[iquad][0] = jnode2;
          iniq[iquad][1] = jnode3;
          iniq[iquad][2] = jnode1;
          iniq[iquad][3] = knode1;
        }

        else if (jnode2 == knode1 && jnode3 == knode3)
        {
          iniq[iquad][0] = jnode3;
          iniq[iquad][1] = jnode1;
          iniq[iquad][2] = jnode2;
          iniq[iquad][3] = knode2;
        }
        else if (jnode2 == knode2 && jnode3 == knode1)
        {
          iniq[iquad][0] = jnode3;
          iniq[iquad][1] = jnode1;
          iniq[iquad][2] = jnode2;
          iniq[iquad][3] = knode3;
        }
        else if (jnode2 == knode3 && jnode3 == knode2)
        {
          iniq[iquad][0] = jnode3;
          iniq[iquad][1] = jnode1;
          iniq[iquad][2] = jnode2;
          iniq[iquad][3] = knode1;
        }

        else if (jnode3 == knode1 && jnode1 == knode3)
        {
          iniq[iquad][0] = jnode1;
          iniq[iquad][1] = jnode2;
          iniq[iquad][2] = jnode3;
          iniq[iquad][3] = knode2;
        }
        else if (jnode3 == knode2 && jnode1 == knode1)
        {
          iniq[iquad][0] = jnode1;
          iniq[iquad][1] = jnode2;
          iniq[iquad][2] = jnode3;
          iniq[iquad][3] = knode3;
        }
        else //if (jnode3 == knode3 && jnode1 == knode2)
        {
          iniq[iquad][0] = jnode1;
          iniq[iquad][1] = jnode2;
          iniq[iquad][2] = jnode3;
          iniq[iquad][3] = knode1;
        }
      }
    }
  }

  // remove tria-faces created to replace each quad-face on a fixed surface that
  // intersects the BL region and on BL generating surfaces or optionally on all
  // surfaces

  jbface = 0;

  for (ibface = 1; ibface <= nbfacei; ++ibface)
  {
    ibc = ibcibf[ibface];

    iquad = iqibf[ibface];

    if (iquad == 0 ||
        (ibc != FIXED_BL_INT_UG3_GBC && ! CHK_BL_UG3_GBC (ibc) && mquadp > 1))
    {
      ++jbface;

      if (jbface < ibface)
      {
        ibcibf[jbface] = ibc;
        idibf[jbface] = idibf[ibface];
        iqibf[jbface] = iquad;
        irfibf[jbface] = irfibf[ibface];

        inibf[jbface][0] = inibf[ibface][0];
        inibf[jbface][1] = inibf[ibface][1];
        inibf[jbface][2] = inibf[ibface][2];
      }
    }
  }

  *nbface = jbface;

  // remove quad-faces converted into sources, reorder remaining quad-faces, and
  // move quad-face BC and ID flags to make room for new tria-faces

  jquad = 0;

  for (iquad = 1; iquad <= *nquad; ++iquad)
  {
    inode4 = iniq[iquad][3];

    if (inode4 > 0)
    {
      ++jquad;

      ibcibf[nbfacei+jquad] = ibcibf[nbfacei+iquad];
      idibf[nbfacei+jquad] = idibf[nbfacei+iquad];

      iniq[jquad][0] = iniq[iquad][0];
      iniq[jquad][1] = iniq[iquad][1];
      iniq[jquad][2] = iniq[iquad][2];
      iniq[jquad][3] = inode4;
    }
  }

  *nquad = jquad;

  nbfacem = *nbface + 4*(*nquadp);

  for (iquad = *nquad; iquad >= 1; --iquad)
  {
    ibcibf[nbfacem+iquad] = ibcibf[nbfacei+iquad];
    idibf[nbfacem+iquad] = idibf[nbfacei+iquad];
  }
  
  // create a pyramid-element with four exposed tria-faces and the base covering
  // each quad-face on a fixed surface that intersects the BL region or
  // and on BL generating surfaces or optionally on all surfaces

  inode = *nnode;
  ibface = *nbface;
  ielemc5 = 0;

  for (iquad = 1; iquad <= *nquad; ++iquad)
  {
    ibc = ibcibf[nbfacem+iquad];

    if (ibc == FIXED_BL_INT_UG3_GBC || CHK_BL_UG3_GBC (ibc) || mquadp == 1)
    {
      id = idibf[nbfacem+iquad];

      inode1 = iniq[iquad][0];
      inode2 = iniq[iquad][1];
      inode3 = iniq[iquad][2];
      inode4 = iniq[iquad][3];

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

      dx211 = x21 - x11;
      dx212 = x22 - x12;
      dx213 = x23 - x13;
      dx311 = x31 - x11;
      dx312 = x32 - x12;
      dx313 = x33 - x13;
      dx321 = x31 - x21;
      dx322 = x32 - x22;
      dx323 = x33 - x23;
      dx411 = x41 - x11;
      dx412 = x42 - x12;
      dx413 = x43 - x13;
      dx421 = x41 - x21;
      dx422 = x42 - x22;
      dx423 = x43 - x23;
      dx431 = x41 - x31;
      dx432 = x42 - x32;
      dx433 = x43 - x33;

      b1 = dx312 * dx423 - dx313 * dx422;
      b2 = dx313 * dx421 - dx311 * dx423;
      b3 = dx311 * dx422 - dx312 * dx421;

      if (CHK_BL_UG3_GBC (ibc) && ds != NULL)
      {
        w = ds[inode1] + ds[inode2] + ds[inode3] + ds[inode4];

        ds0 = dc1d4 * w;
      }
      else
      {
        w = cdh * (sqrt (dx211 * dx211 + dx212 * dx212 + dx213 * dx213)
                 + sqrt (dx321 * dx321 + dx322 * dx322 + dx323 * dx323)
                 + sqrt (dx431 * dx431 + dx432 * dx432 + dx433 * dx433)
                 + sqrt (dx411 * dx411 + dx412 * dx412 + dx413 * dx413));

        ds0 = dc0;
      }

      w = w / sqrt (b1 * b1 + b2 * b2 + b3 * b3);

      ++inode;

      x[inode][0] = dc1d4 * (x11 + x21 + x31 + x41 + w * b1);
      x[inode][1] = dc1d4 * (x12 + x22 + x32 + x42 + w * b2);
      x[inode][2] = dc1d4 * (x13 + x23 + x33 + x43 + w * b3);

      if (ds)
        ds[inode] = ds0;

      if (trv)
      {
        trv[inode][0] = dc0;
        trv[inode][1] = dc0;
        trv[inode][2] = dc0;
        trv[inode][3] = dc0;
      }

      ++ibface;

      ibcibf[ibface] = ibc;
      idibf[ibface] = id;
      iqibf[ibface] = -iquad;
      irfibf[ibface] = 7;

      inibf[ibface][0] = inode1;
      inibf[ibface][1] = inode2;
      inibf[ibface][2] = inode;

      ++ibface;

      ibcibf[ibface] = ibc;
      idibf[ibface] = id;
      iqibf[ibface] = -iquad;
      irfibf[ibface] = 7;

      inibf[ibface][0] = inode2;
      inibf[ibface][1] = inode3;
      inibf[ibface][2] = inode;

      ++ibface;

      ibcibf[ibface] = ibc;
      idibf[ibface] = id;
      iqibf[ibface] = -iquad;
      irfibf[ibface] = 7;

      inibf[ibface][0] = inode3;
      inibf[ibface][1] = inode4;
      inibf[ibface][2] = inode;

      ++ibface;

      ibcibf[ibface] = ibc;
      idibf[ibface] = id;
      iqibf[ibface] = -iquad;
      irfibf[ibface] = 7;

      inibf[ibface][0] = inode4;
      inibf[ibface][1] = inode1;
      inibf[ibface][2] = inode;

      ++ielemc5;

      inielc5[ielemc5][0] = inode4;
      inielc5[ielemc5][1] = inode3;
      inielc5[ielemc5][2] = inode;
      inielc5[ielemc5][3] = inode1;
      inielc5[ielemc5][4] = inode2;
      //inielc5[ielemc5][0] = inode1;
      //inielc5[ielemc5][1] = inode2;
      //inielc5[ielemc5][2] = inode3;
      //inielc5[ielemc5][3] = inode4;
      //inielc5[ielemc5][4] = inode;
    }
  }

  *nbface = ibface;
  *nnode = inode;

  if (mmsg == 2)
  {
    sprintf (Text, "Creating Pyramids  : Pyramid-Elements  =%10i", *nelemc5);
    ug_message (Text);
    sprintf (Text, "Creating Pyramids  : Quad-, Tria-BFaces=%10i%10i", *nquad, *nbface);
    ug_message (Text);
    sprintf (Text, "Creating Pyramids  : Nodes             =%10i", *nnode);
    ug_message (Text);
  }

  return (0);

}
