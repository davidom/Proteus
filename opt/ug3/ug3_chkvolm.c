#include "UG3_LIB.h"

INT_ ug3_chkvolm
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
  double tol,
  DOUBLE_3D * x)

{

/*
 * Check mixed element volume. Element volume (vol) is checked for zero or
 * negative volumes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkvolm.c,v 1.13 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, ierr, inode1, inode2, inode3, inode4, inode5, inode6, inode7,
       inode8, nsmall;

  double dc0, dx, dx1, dx2, dx211, dx212, dx213, dx3, dx311, dx312, dx313,
         dx411, dx412, dx413, dx511, dx512, dx513, dx611, dx612, dx613, dx711,
         dx712, dx713, dx811, dx812, dx813, vol, voltol, x11, x12, x13, x1max,
         x1min, x21, x22, x23, x2max, x2min, x31, x32, x33, x3max, x3min, x41,
         x42, x43, x51, x52, x53, x61, x62, x63, x71, x72, x73, x81, x82, x83;

  dc0 = 0.0;

  if (mmsg == 2)
  {
    sprintf (Text, "Checking Tets      : Nodes, Elements   =%10i%10i",
             nnode, nelem);
    ug_message (Text);
  }

  ierr = 0;

  nsmall = 0;

  if (nelem > 0)
  {
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

      voltol = tol * dx * dx * dx;

      if (vol <= dc0)
      {
        ierr = 1;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: TetVol<=0   @%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);
        }
      }

      else if (vol < voltol)
      {
        ++nsmall;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: TetVol<Tol  @%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);
        }
      }
    }
  }

  if (nelemc5 > 0)
  {
    if (mmsg == 2)
    {
      sprintf (Text, "Checking Pyramids  : Nodes, Elements   =%10i%10i",
               nnode, nelemc5);
      ug_message (Text);
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

      x1max = MAX (x11, x21);
      x1max = MAX (x31, x1max);
      x1max = MAX (x41, x1max);
      x1max = MAX (x51, x1max);
      x2max = MAX (x12, x22);
      x2max = MAX (x32, x2max);
      x2max = MAX (x42, x2max);
      x2max = MAX (x52, x2max);
      x3max = MAX (x13, x23);
      x3max = MAX (x33, x3max);
      x3max = MAX (x43, x3max);
      x3max = MAX (x53, x3max);
      x1min = MIN (x11, x21);
      x1min = MIN (x31, x1min);
      x1min = MIN (x41, x1min);
      x1min = MIN (x51, x1min);
      x2min = MIN (x12, x22);
      x2min = MIN (x32, x2min);
      x2min = MIN (x42, x2min);
      x2min = MIN (x52, x2min);
      x3min = MIN (x13, x23);
      x3min = MIN (x33, x3min);
      x3min = MIN (x43, x3min);
      x3min = MIN (x53, x3min);

      dx1 = x1max - x1min;
      dx2 = x2max - x2min;
      dx3 = x3max - x3min;

      dx = MAX (dx1, dx2);
      dx = MAX (dx3, dx);

      voltol = tol * dx * dx * dx;

      if (vol <= dc0)
      {
        ierr = 1;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: Pent5Vol<=0 @%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x51, x52, x53);
          ug_message (Text);
        }
      }

      else if (vol < voltol)
      {
        ++nsmall;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: Pent5Vol<Tol@%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x51, x52, x53);
          ug_message (Text);
        }
      }
    }
  }

  if (nelemc6 > 0)
  {
    if (mmsg == 2)
    {
      sprintf (Text, "Checking Prisms    : Nodes, Elements   =%10i%10i",
               nnode, nelemc6);
      ug_message (Text);
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

      x1max = MAX (x11, x21);
      x1max = MAX (x31, x1max);
      x1max = MAX (x41, x1max);
      x1max = MAX (x51, x1max);
      x1max = MAX (x61, x1max);
      x2max = MAX (x12, x22);
      x2max = MAX (x32, x2max);
      x2max = MAX (x42, x2max);
      x2max = MAX (x52, x2max);
      x2max = MAX (x62, x2max);
      x3max = MAX (x13, x23);
      x3max = MAX (x33, x3max);
      x3max = MAX (x43, x3max);
      x3max = MAX (x53, x3max);
      x3max = MAX (x63, x3max);
      x1min = MIN (x11, x21);
      x1min = MIN (x31, x1min);
      x1min = MIN (x41, x1min);
      x1min = MIN (x51, x1min);
      x1min = MIN (x61, x1min);
      x2min = MIN (x12, x22);
      x2min = MIN (x32, x2min);
      x2min = MIN (x42, x2min);
      x2min = MIN (x52, x2min);
      x2min = MIN (x62, x2min);
      x3min = MIN (x13, x23);
      x3min = MIN (x33, x3min);
      x3min = MIN (x43, x3min);
      x3min = MIN (x53, x3min);
      x3min = MIN (x63, x3min);

      dx1 = x1max - x1min;
      dx2 = x2max - x2min;
      dx3 = x3max - x3min;

      dx = MAX (dx1, dx2);
      dx = MAX (dx3, dx);

      voltol = tol * dx * dx * dx;

      if (vol <= dc0)
      {
        ierr = 1;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: Pent6Vol<=0 @%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x51, x52, x53);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x61, x62, x63);
          ug_message (Text);
        }
      }

      else if (vol < voltol)
      {
        ++nsmall;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: Pent6Vol<Tol@%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x51, x52, x53);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x61, x62, x63);
          ug_message (Text);
        }
      }
    }
  }

  if (nelemc8 > 0)
  {
    if (mmsg == 2)
    {
      sprintf (Text, "Checking Hexs      : Nodes, Elements   =%10i%10i",
               nnode, nelemc8);
      ug_message (Text);
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

      x1max = MAX (x11, x21);
      x1max = MAX (x31, x1max);
      x1max = MAX (x41, x1max);
      x1max = MAX (x51, x1max);
      x1max = MAX (x61, x1max);
      x1max = MAX (x71, x1max);
      x1max = MAX (x81, x1max);
      x2max = MAX (x12, x22);
      x2max = MAX (x32, x2max);
      x2max = MAX (x42, x2max);
      x2max = MAX (x52, x2max);
      x2max = MAX (x62, x2max);
      x2max = MAX (x72, x2max);
      x2max = MAX (x82, x2max);
      x3max = MAX (x13, x23);
      x3max = MAX (x33, x3max);
      x3max = MAX (x43, x3max);
      x3max = MAX (x53, x3max);
      x3max = MAX (x63, x3max);
      x3max = MAX (x73, x3max);
      x3max = MAX (x83, x3max);
      x1min = MIN (x11, x21);
      x1min = MIN (x31, x1min);
      x1min = MIN (x41, x1min);
      x1min = MIN (x51, x1min);
      x1min = MIN (x61, x1min);
      x1min = MIN (x71, x1min);
      x1min = MIN (x81, x1min);
      x2min = MIN (x12, x22);
      x2min = MIN (x32, x2min);
      x2min = MIN (x42, x2min);
      x2min = MIN (x52, x2min);
      x2min = MIN (x62, x2min);
      x2min = MIN (x72, x2min);
      x2min = MIN (x82, x2min);
      x3min = MIN (x13, x23);
      x3min = MIN (x33, x3min);
      x3min = MIN (x43, x3min);
      x3min = MIN (x53, x3min);
      x3min = MIN (x63, x3min);
      x3min = MIN (x73, x3min);
      x3min = MIN (x83, x3min);

      dx1 = x1max - x1min;
      dx2 = x2max - x2min;
      dx3 = x3max - x3min;

      dx = MAX (dx1, dx2);
      dx = MAX (dx3, dx);

      voltol = tol * dx * dx * dx;

      if (vol <= dc0)
      {
        ierr = 1;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: HexVol<=0   @%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x51, x52, x53);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x61, x62, x63);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x71, x72, x73);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x81, x82, x83);
          ug_message (Text);
        }
      }

      else if (vol < voltol)
      {
        ++nsmall;

        if (mmsg >= 2)
        {
          sprintf (Text, "VOL CHECK: HexVol<Tol  @%-10i XYZ=%10g,%10g,%10g",
                   ielem, x11, x12, x13);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x21, x22, x23);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x31, x32, x33);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x41, x42, x43);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x51, x52, x53);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x61, x62, x63);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x71, x72, x73);
          ug_message (Text);

          sprintf (Text, "                                       %10g,%10g,%10g",
                     x81, x82, x83);
          ug_message (Text);
        }
      }
    }
  }

  if (ierr > 0)
    return (325);

  if (mmsg == 2)
  {
    if (nsmall == 0)
      ug_message ("Checking           : Volume OK");

    else
    {
      sprintf (Text, "Checking           : Small Volumes     =%10i", nsmall);
      ug_message (Text);
    }
  }

  ierr = (nsmall == 0) ? 0: -nsmall;

  return (ierr);

}
