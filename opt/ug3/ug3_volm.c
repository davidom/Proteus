#include "UG3_LIB.h"

void ug3_volm
 (INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nelemc8,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inielc8,
  DOUBLE_1D * vol,
  DOUBLE_3D * x)

{

/*
 * Compute tet, prism, pyramid, and hex element volumes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_volm.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, inode1, inode2, inode3, inode4, inode5, inode6, inode7, inode8,
       jelem;

  double dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412, dx413, dx511,
         dx512, dx513, dx611, dx612, dx613, dx711, dx712, dx713, dx811, dx812,
         dx813, x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43, x51,
         x52, x53, x61, x62, x63, x71, x72, x73, x81, x82, x83;

  jelem = 0;

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

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
    dx411 = x41 - x11;
    dx412 = x42 - x12;
    dx413 = x43 - x13;

    ++jelem;

    vol[jelem] = dx211 * (dx312 * dx413 - dx313 * dx412)
               + dx212 * (dx313 * dx411 - dx311 * dx413)
               + dx213 * (dx311 * dx412 - dx312 * dx411);
  }

  for (ielem = 1; ielem <= nelemc5; ++ielem)
  {
    inode1 = inielc5[ielem][0];
    inode2 = inielc5[ielem][1];
    inode3 = inielc5[ielem][2];
    inode4 = inielc5[ielem][3];
    inode5 = inielc5[ielem][4];

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
    x51 = x[inode5][0];
    x52 = x[inode5][1];
    x53 = x[inode5][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;
    dx411 = x41 - x11;
    dx412 = x42 - x12;
    dx413 = x43 - x13;
    dx511 = x51 - x11;
    dx512 = x52 - x12;
    dx513 = x53 - x13;

    ++jelem;

    vol[jelem] = dx511 * (dx212 * dx313 - dx213 * dx312)
               + dx512 * (dx213 * dx311 - dx211 * dx313)
               + dx513 * (dx211 * dx312 - dx212 * dx311)
               + dx411 * (dx512 * dx313 - dx513 * dx312)
               + dx412 * (dx513 * dx311 - dx511 * dx313)
               + dx413 * (dx511 * dx312 - dx512 * dx311);
  }

  for (ielem = 1; ielem <= nelemc6; ++ielem)
  {
    inode1 = inielc6[ielem][0];
    inode2 = inielc6[ielem][1];
    inode3 = inielc6[ielem][2];
    inode4 = inielc6[ielem][3];
    inode5 = inielc6[ielem][4];
    inode6 = inielc6[ielem][5];

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
    x51 = x[inode5][0];
    x52 = x[inode5][1];
    x53 = x[inode5][2];
    x61 = x[inode6][0];
    x62 = x[inode6][1];
    x63 = x[inode6][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;
    dx411 = x41 - x11;
    dx412 = x42 - x12;
    dx413 = x43 - x13;
    dx511 = x51 - x11;
    dx512 = x52 - x12;
    dx513 = x53 - x13;
    dx611 = x61 - x11;
    dx612 = x62 - x12;
    dx613 = x63 - x13;

    ++jelem;

    vol[jelem] = dx611 * (dx212 * dx313 - dx213 * dx312)
               + dx612 * (dx213 * dx311 - dx211 * dx313)
               + dx613 * (dx211 * dx312 - dx212 * dx311)
               + dx511 * (dx212 * dx613 - dx213 * dx612)
               + dx512 * (dx213 * dx611 - dx211 * dx613)
               + dx513 * (dx211 * dx612 - dx212 * dx611)
               + dx411 * (dx512 * dx613 - dx513 * dx612)
               + dx412 * (dx513 * dx611 - dx511 * dx613)
               + dx413 * (dx511 * dx612 - dx512 * dx611);
  }

  for (ielem = 1; ielem <= nelemc8; ++ielem)
  {
    inode1 = inielc8[ielem][0];
    inode2 = inielc8[ielem][1];
    inode3 = inielc8[ielem][2];
    inode4 = inielc8[ielem][3];
    inode5 = inielc8[ielem][4];
    inode6 = inielc8[ielem][5];
    inode7 = inielc8[ielem][6];
    inode8 = inielc8[ielem][7];

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
    x51 = x[inode5][0];
    x52 = x[inode5][1];
    x53 = x[inode5][2];
    x61 = x[inode6][0];
    x62 = x[inode6][1];
    x63 = x[inode6][2];
    x71 = x[inode7][0];
    x72 = x[inode7][1];
    x73 = x[inode7][2];
    x81 = x[inode8][0];
    x82 = x[inode8][1];
    x83 = x[inode8][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;
    dx411 = x41 - x11;
    dx412 = x42 - x12;
    dx413 = x43 - x13;
    dx511 = x51 - x11;
    dx512 = x52 - x12;
    dx513 = x53 - x13;
    dx611 = x61 - x11;
    dx612 = x62 - x12;
    dx613 = x63 - x13;
    dx711 = x71 - x11;
    dx712 = x72 - x12;
    dx713 = x73 - x13;
    dx811 = x81 - x11;
    dx812 = x82 - x12;
    dx813 = x83 - x13;

    ++jelem;

    vol[jelem] = dx711 * (dx212 * dx313 - dx213 * dx312)
               + dx712 * (dx213 * dx311 - dx211 * dx313)
               + dx713 * (dx211 * dx312 - dx212 * dx311)
               + dx611 * (dx212 * dx713 - dx213 * dx712)
               + dx612 * (dx213 * dx711 - dx211 * dx713)
               + dx613 * (dx211 * dx712 - dx212 * dx711)
               + dx511 * (dx612 * dx713 - dx613 * dx712)
               + dx512 * (dx613 * dx711 - dx611 * dx713)
               + dx513 * (dx611 * dx712 - dx612 * dx711)
               + dx711 * (dx312 * dx413 - dx313 * dx412)
               + dx712 * (dx313 * dx411 - dx311 * dx413)
               + dx713 * (dx311 * dx412 - dx312 * dx411)
               + dx811 * (dx712 * dx413 - dx713 * dx412)
               + dx812 * (dx713 * dx411 - dx711 * dx413)
               + dx813 * (dx711 * dx412 - dx712 * dx411)
               + dx511 * (dx712 * dx813 - dx713 * dx812)
               + dx512 * (dx713 * dx811 - dx711 * dx813)
               + dx513 * (dx711 * dx812 - dx712 * dx811);
  }

  return;

}
