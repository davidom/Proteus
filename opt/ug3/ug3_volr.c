#include "UG3_LIB.h"

void ug3_volr
 (INT_ mmsg,
  INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nelemc8,
  INT_ nnode,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inielc8,
  double * volrmax,
  double * volrmin,
  DOUBLE_1D * volmax,
  DOUBLE_1D * volmin,
  DOUBLE_1D * volr,
  DOUBLE_3D * x)

{

/*
 * Determine ratio of minimum to maximum element volume at each node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_volr.c,v 1.6 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, inode, inode1, inode2, inode3, inode4, inode5, inode6, inode7,
       inode8;

  double dc0, dc1, dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412,
         dx413, dx511, dx512, dx513, dx611, dx612, dx613, dx711, dx712, dx713,
         dx811, dx812, dx813, vol, volmax1, volmax2, volmax3, volmax4, volmax5,
         volmax6, volmax7, volmax8,  volmin1, volmin2, volmin3, volmin4,
         volmin5, volmin6, volmin7, volmin8,  x11, x12, x13, x21, x22, x23, x31,
         x32, x33, x41, x42, x43, x51, x52, x53, x61, x62, x63, x71, x72, x73,
         x81, x82, x83;

  dc0 = 0.0;
  dc1 = 1.0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    volmax[inode] = dc0;
    volmin[inode] = dc0;
  }

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

    vol = dx211 * (dx312 * dx413 - dx313 * dx412)
        + dx212 * (dx313 * dx411 - dx311 * dx413)
        + dx213 * (dx311 * dx412 - dx312 * dx411);

    volmax1 = volmax[inode1];
    volmax2 = volmax[inode2];
    volmax3 = volmax[inode3];
    volmax4 = volmax[inode4];

    volmin1 = volmin[inode1];
    volmin2 = volmin[inode2];
    volmin3 = volmin[inode3];
    volmin4 = volmin[inode4];

    if (volmin1 == dc0) volmin1 = vol;
    if (volmin2 == dc0) volmin2 = vol;
    if (volmin3 == dc0) volmin3 = vol;
    if (volmin4 == dc0) volmin4 = vol;

    volmax1 = MAX (vol, volmax1);
    volmax2 = MAX (vol, volmax2);
    volmax3 = MAX (vol, volmax3);
    volmax4 = MAX (vol, volmax4);

    volmin1 = MIN (vol, volmin1);
    volmin2 = MIN (vol, volmin2);
    volmin3 = MIN (vol, volmin3);
    volmin4 = MIN (vol, volmin4);

    volmax[inode1] = volmax1;
    volmax[inode2] = volmax2;
    volmax[inode3] = volmax3;
    volmax[inode4] = volmax4;

    volmin[inode1] = volmin1;
    volmin[inode2] = volmin2;
    volmin[inode3] = volmin3;
    volmin[inode4] = volmin4;
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

    vol = dx511 * (dx212 * dx313 - dx213 * dx312)
        + dx512 * (dx213 * dx311 - dx211 * dx313)
        + dx513 * (dx211 * dx312 - dx212 * dx311)
        + dx411 * (dx512 * dx313 - dx513 * dx312)
        + dx412 * (dx513 * dx311 - dx511 * dx313)
        + dx413 * (dx511 * dx312 - dx512 * dx311);

    volmax1 = volmax[inode1];
    volmax2 = volmax[inode2];
    volmax3 = volmax[inode3];
    volmax4 = volmax[inode4];
    volmax5 = volmax[inode5];

    volmin1 = volmin[inode1];
    volmin2 = volmin[inode2];
    volmin3 = volmin[inode3];
    volmin4 = volmin[inode4];
    volmin5 = volmin[inode5];

    if (volmin1 == dc0) volmin1 = vol;
    if (volmin2 == dc0) volmin2 = vol;
    if (volmin3 == dc0) volmin3 = vol;
    if (volmin4 == dc0) volmin4 = vol;
    if (volmin5 == dc0) volmin5 = vol;

    volmax1 = MAX (vol, volmax1);
    volmax2 = MAX (vol, volmax2);
    volmax3 = MAX (vol, volmax3);
    volmax4 = MAX (vol, volmax4);
    volmax5 = MAX (vol, volmax5);

    volmin1 = MIN (vol, volmin1);
    volmin2 = MIN (vol, volmin2);
    volmin3 = MIN (vol, volmin3);
    volmin4 = MIN (vol, volmin4);
    volmin5 = MIN (vol, volmin5);

    volmax[inode1] = volmax1;
    volmax[inode2] = volmax2;
    volmax[inode3] = volmax3;
    volmax[inode4] = volmax4;
    volmax[inode5] = volmax5;

    volmin[inode1] = volmin1;
    volmin[inode2] = volmin2;
    volmin[inode3] = volmin3;
    volmin[inode4] = volmin4;
    volmin[inode5] = volmin5;
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

    vol = dx611 * (dx212 * dx313 - dx213 * dx312)
        + dx612 * (dx213 * dx311 - dx211 * dx313)
        + dx613 * (dx211 * dx312 - dx212 * dx311)
        + dx511 * (dx212 * dx613 - dx213 * dx612)
        + dx512 * (dx213 * dx611 - dx211 * dx613)
        + dx513 * (dx211 * dx612 - dx212 * dx611)
        + dx411 * (dx512 * dx613 - dx513 * dx612)
        + dx412 * (dx513 * dx611 - dx511 * dx613)
        + dx413 * (dx511 * dx612 - dx512 * dx611);

    volmax1 = volmax[inode1];
    volmax2 = volmax[inode2];
    volmax3 = volmax[inode3];
    volmax4 = volmax[inode4];
    volmax5 = volmax[inode5];
    volmax6 = volmax[inode6];

    volmin1 = volmin[inode1];
    volmin2 = volmin[inode2];
    volmin3 = volmin[inode3];
    volmin4 = volmin[inode4];
    volmin5 = volmin[inode5];
    volmin6 = volmin[inode6];

    if (volmin1 == dc0) volmin1 = vol;
    if (volmin2 == dc0) volmin2 = vol;
    if (volmin3 == dc0) volmin3 = vol;
    if (volmin4 == dc0) volmin4 = vol;
    if (volmin5 == dc0) volmin5 = vol;
    if (volmin6 == dc0) volmin6 = vol;

    volmax1 = MAX (vol, volmax1);
    volmax2 = MAX (vol, volmax2);
    volmax3 = MAX (vol, volmax3);
    volmax4 = MAX (vol, volmax4);
    volmax5 = MAX (vol, volmax5);
    volmax6 = MAX (vol, volmax6);

    volmin1 = MIN (vol, volmin1);
    volmin2 = MIN (vol, volmin2);
    volmin3 = MIN (vol, volmin3);
    volmin4 = MIN (vol, volmin4);
    volmin5 = MIN (vol, volmin5);
    volmin6 = MIN (vol, volmin6);

    volmax[inode1] = volmax1;
    volmax[inode2] = volmax2;
    volmax[inode3] = volmax3;
    volmax[inode4] = volmax4;
    volmax[inode5] = volmax5;
    volmax[inode6] = volmax6;

    volmin[inode1] = volmin1;
    volmin[inode2] = volmin2;
    volmin[inode3] = volmin3;
    volmin[inode4] = volmin4;
    volmin[inode5] = volmin5;
    volmin[inode6] = volmin6;
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

    vol = dx711 * (dx212 * dx313 - dx213 * dx312)
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

    volmax1 = volmax[inode1];
    volmax2 = volmax[inode2];
    volmax3 = volmax[inode3];
    volmax4 = volmax[inode4];
    volmax5 = volmax[inode5];
    volmax6 = volmax[inode6];
    volmax7 = volmax[inode7];
    volmax8 = volmax[inode8];

    volmin1 = volmin[inode1];
    volmin2 = volmin[inode2];
    volmin3 = volmin[inode3];
    volmin4 = volmin[inode4];
    volmin5 = volmin[inode5];
    volmin6 = volmin[inode6];
    volmin7 = volmin[inode7];
    volmin8 = volmin[inode8];

    if (volmin1 == dc0) volmin1 = vol;
    if (volmin2 == dc0) volmin2 = vol;
    if (volmin3 == dc0) volmin3 = vol;
    if (volmin4 == dc0) volmin4 = vol;
    if (volmin5 == dc0) volmin5 = vol;
    if (volmin6 == dc0) volmin6 = vol;
    if (volmin7 == dc0) volmin7 = vol;
    if (volmin8 == dc0) volmin8 = vol;

    volmax1 = MAX (vol, volmax1);
    volmax2 = MAX (vol, volmax2);
    volmax3 = MAX (vol, volmax3);
    volmax4 = MAX (vol, volmax4);
    volmax5 = MAX (vol, volmax5);
    volmax6 = MAX (vol, volmax6);
    volmax7 = MAX (vol, volmax7);
    volmax8 = MAX (vol, volmax8);

    volmin1 = MIN (vol, volmin1);
    volmin2 = MIN (vol, volmin2);
    volmin3 = MIN (vol, volmin3);
    volmin4 = MIN (vol, volmin4);
    volmin5 = MIN (vol, volmin5);
    volmin6 = MIN (vol, volmin6);
    volmin7 = MIN (vol, volmin7);
    volmin8 = MIN (vol, volmin8);

    volmax[inode1] = volmax1;
    volmax[inode2] = volmax2;
    volmax[inode3] = volmax3;
    volmax[inode4] = volmax4;
    volmax[inode5] = volmax5;
    volmax[inode6] = volmax6;
    volmax[inode7] = volmax7;
    volmax[inode8] = volmax8;

    volmin[inode1] = volmin1;
    volmin[inode2] = volmin2;
    volmin[inode3] = volmin3;
    volmin[inode4] = volmin4;
    volmin[inode5] = volmin5;
    volmin[inode6] = volmin6;
    volmin[inode7] = volmin7;
    volmin[inode8] = volmin8;
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    volr[inode] = volmin[inode] / volmax[inode];
  }

  *volrmax = dc0;
  *volrmin = dc1;

  for (inode = 1; inode <= nnode; ++inode)
  {
    *volrmax = MAX (volr[inode], *volrmax);
    *volrmin = MIN (volr[inode], *volrmin);
  }

  if (mmsg == 2)
  {
    sprintf (Text, "Volume Ratio       : Maximum           =%10g", *volrmax);
    sprintf (Text, "Volume Ratio       : Minimum           =%10g", *volrmin);

    ug_message (Text);
  }

  return;

}
