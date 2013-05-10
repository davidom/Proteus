#include "UG3_LIB.h"

INT_ ug3_qtria_pyramid
 (INT_ *nbface,
  INT_ *nelemc5,
  INT_ *nnodeb,
  INT_ nquad,
  INT_1D ** ibcibf_ptr,
  INT_1D ** idibf_ptr,
  INT_1D ** ierribf_ptr,
  INT_3D ** inibf_ptr,
  INT_5D ** inielc5_ptr,
  INT_4D * iniq,
  INT_1D ** iqibf_ptr,
  INT_1D ** irfibf_ptr,
  DOUBLE_3D ** x_ptr)

{

/*
 * Replace quad surface faces with tria surface faces on pyramid elements.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_qtria_pyramid.c,v 1.3 2012/08/27 19:13:19 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ibcibf, *idibf, *iqibf, *irfibf;
  INT_3D *inibf;
  INT_5D *inielc5;

  DOUBLE_3D *x;

  INT_ Error_Flag, ibc, ibface, id, ielemc5, inode, inode1, inode2, inode3,
       inode4, iquad, nbfacei, nbfacem, nelemc5i, nelemc5m, nnodebi, nnodebm;

  double b1, b2, b3, cdh, dc1d2, dc1d4,
         dx211, dx212, dx213, 
         dx311, dx312, dx313, 
         dx321, dx322, dx323, 
         dx411, dx412, dx413, 
         dx421, dx422, dx423, 
         dx431, dx432, dx433, 
         w, x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc1d2 = 0.5;
  dc1d4 = 0.25;

  if (nquad == 0)
    return (0);

  cdh = sqrt (dc1d2);

  nbfacei = *nbface;
  nbfacem = *nbface + 4 * nquad;

  nelemc5i = *nelemc5;
  nelemc5m = *nelemc5 + nquad;

  nnodebi = *nnodeb;
  nnodebm = *nnodeb + nquad;

  Error_Flag = 0;

  *ibcibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *ibcibf_ptr,
                                       (nbfacem+1) * sizeof (INT_1D));
  *idibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *idibf_ptr,
                                      (nbfacem+1) * sizeof (INT_1D));
  *ierribf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *ierribf_ptr,
                                        (nbfacem+1) * sizeof (INT_1D));
  *iqibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *iqibf_ptr,
                                      (nbfacem+1) * sizeof (INT_1D));
  *irfibf_ptr = (INT_1D *) ug_realloc (&Error_Flag, *irfibf_ptr,
                                       (nbfacem+1) * sizeof (INT_1D));
  *inibf_ptr = (INT_3D *) ug_realloc (&Error_Flag, *inibf_ptr,
                                      (nbfacem+1) * sizeof (INT_3D));
  *inielc5_ptr = (INT_5D *) ug_realloc (&Error_Flag, *inielc5_ptr,
                                      (nelemc5m+1) * sizeof (INT_5D));
  *x_ptr = (DOUBLE_3D *) ug_realloc (&Error_Flag, *x_ptr,
                                     (nbfacem+1) * sizeof (DOUBLE_3D));

  if (Error_Flag > 0)
    return (100317);

  ibcibf = *ibcibf_ptr;
  idibf = *idibf_ptr;
  irfibf = *irfibf_ptr;
  inibf = *inibf_ptr;
  inielc5 = *inielc5_ptr;
  x = *x_ptr;

  inode = *nnodeb;
  ibface = *nbface;

  for (iquad = 1; iquad <= nquad; ++iquad)
  {
    ++inode;

    ibc = ibcibf[*nbface+iquad];

    id = idibf[*nbface+iquad];

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

    w = cdh * (sqrt (dx211 * dx211 + dx212 * dx212 + dx213 * dx213)
             + sqrt (dx321 * dx321 + dx322 * dx322 + dx323 * dx323)
             + sqrt (dx431 * dx431 + dx432 * dx432 + dx433 * dx433)
             + sqrt (dx411 * dx411 + dx412 * dx412 + dx413 * dx413))
      / sqrt (b1 * b1 + b2 * b2 + b3 * b3);

    x[inode][0] = dc1d4 * (x11 + x21 + x31 + x41 + w * b1);
    x[inode][1] = dc1d4 * (x12 + x22 + x32 + x42 + w * b2);
    x[inode][2] = dc1d4 * (x13 + x23 + x33 + x43 + w * b3);

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

    inielc5[ielemc5][0] = inode1;
    inielc5[ielemc5][1] = inode2;
    inielc5[ielemc5][2] = inode3;
    inielc5[ielemc5][3] = inode4;
    inielc5[ielemc5][4] = inode;
  }

  return (0);

}
