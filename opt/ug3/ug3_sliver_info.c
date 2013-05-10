#include "UG3_LIB.h"

void ug3_sliver_info
 (INT_ mmsg,
  INT_ nelem,
  INT_4D * ieliel,
  INT_4D * iniel,
  double angsliver1,
  double angsliver2,
  DOUBLE_3D * x)

{

/*
 * Get information about sliver elements.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_sliver_info.c,v 1.12 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ i, ibad, ielem, ieln1, ieln2, ieln3, ieln4, inode1, inode2, inode3,
       inode4, isliver, jelem, jelem3, jelem4, jeln1, jeln2, m1, m2, ni1, ni2;

  INT_ ielnmap[4][4][4] =
  {
    {
      {0, 1, 2, 3},
      {1, 0, 3, 2},
      {2, 0, 1, 3},
      {3, 0, 2, 1}},
    {
      {0, 1, 2, 3},
      {1, 2, 0, 3},
      {2, 1, 3, 0},
      {3, 1, 0, 2}},
    {
      {0, 2, 3, 1},
      {1, 2, 0, 3},
      {2, 3, 0, 1},
      {3, 2, 1, 0}},
    {
      {0, 3, 1, 2},
      {1, 3, 2, 0},
      {2, 3, 0, 1},
      {3, 0, 2, 1}}};

  double a1241, a1242, a1243, a124sr, a1321, a1322, a1323, a132sr, a1431, a1432,
         a1433, a143sr, a2341, a2342, a2343, a234sr, angmax, cosmax1, cosmax1s,
         cosmax2, cosmax2s, dc0, dc1, dc45, dx211, dx212, dx213, dx311, dx312,
         dx313, dx411, dx412, dx413, w, w21, w31, w41, w32, w42, w43, wmax, x11,
         x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  if (mmsg != 2) return;

  cosmax1 = -cos (angsliver1 * atan (dc1) / dc45);
  cosmax2 = -cos (angsliver2 * atan (dc1) / dc45);

  cosmax1s = cosmax1 * fabs (cosmax1);
  cosmax2s = cosmax2 * fabs (cosmax2);

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

    a1241 = dx212 * dx413 - dx213 * dx412;
    a1242 = dx213 * dx411 - dx211 * dx413;
    a1243 = dx211 * dx412 - dx212 * dx411;
    a1321 = dx312 * dx213 - dx313 * dx212;
    a1322 = dx313 * dx211 - dx311 * dx213;
    a1323 = dx311 * dx212 - dx312 * dx211;
    a1431 = dx412 * dx313 - dx413 * dx312;
    a1432 = dx413 * dx311 - dx411 * dx313;
    a1433 = dx411 * dx312 - dx412 * dx311;
    a2341 = -a1241 - a1321 - a1431;
    a2342 = -a1242 - a1322 - a1432;
    a2343 = -a1243 - a1323 - a1433;

    a124sr = dc1 / (a1241 * a1241 + a1242 * a1242 + a1243 * a1243);
    a132sr = dc1 / (a1321 * a1321 + a1322 * a1322 + a1323 * a1323);
    a143sr = dc1 / (a1431 * a1431 + a1432 * a1432 + a1433 * a1433);
    a234sr = dc1 / (a2341 * a2341 + a2342 * a2342 + a2343 * a2343);

    w21 = a1241 * a1321 + a1242 * a1322 + a1243 * a1323;
    w31 = a1431 * a1321 + a1432 * a1322 + a1433 * a1323;
    w41 = a1431 * a1241 + a1432 * a1242 + a1433 * a1243;
    w32 = a2341 * a1321 + a2342 * a1322 + a2343 * a1323;
    w42 = a2341 * a1241 + a2342 * a1242 + a2343 * a1243;
    w43 = a2341 * a1431 + a2342 * a1432 + a2343 * a1433;

    w21 = w21 * fabs (w21) * a124sr * a132sr;
    w31 = w31 * fabs (w31) * a143sr * a132sr;
    w41 = w41 * fabs (w41) * a143sr * a124sr;
    w32 = w32 * fabs (w32) * a234sr * a132sr;
    w42 = w42 * fabs (w42) * a234sr * a124sr;
    w43 = w43 * fabs (w43) * a234sr * a143sr;

    wmax = MAX (w21, wmax);
    wmax = MAX (w31, wmax);
    wmax = MAX (w41, wmax);
    wmax = MAX (w32, wmax);
    wmax = MAX (w42, wmax);
    wmax = MAX (w43, wmax);

    ibad = (wmax > cosmax2s) ? 1 : 0;

    if (ibad == 1)
    {
      w = sqrt (fabs (wmax));
      w = MIN (w, dc1);

      angmax = (wmax >= dc0) ? acos (-w): acos (w);
      angmax = angmax * dc45 / atan (dc1);

      isliver = (w21 > cosmax1s && w43 > cosmax1s) ? 1 :
                (w31 > cosmax1s && w42 > cosmax1s) ? 2 :
                (w41 > cosmax1s && w32 > cosmax1s) ? 3 : 0;

      if (isliver > 0)
      {
        ieln1 = 0;
        ieln2 = isliver;
        ieln3 = ielnmap[ieln2][ieln1][2];
        ieln4 = ielnmap[ieln2][ieln1][3];

        jelem3 = ieliel[ielem][ieln3];
        jelem4 = ieliel[ielem][ieln4];

        ni1 = 0;

        if (jelem3 > 0 && jelem4 > 0)
        {
          inode1 = iniel[ielem][ieln1];
          inode2 = iniel[ielem][ieln2];

          jelem3 = ielem;

          i = 1;

          do
          {
            jelem = jelem3;

            ieln1 = (iniel[jelem][0] == inode1) ? 0 :
                    (iniel[jelem][1] == inode1) ? 1 :
                    (iniel[jelem][2] == inode1) ? 2 : 3;
            ieln2 = (iniel[jelem][0] == inode2) ? 0 :
                    (iniel[jelem][1] == inode2) ? 1 :
                    (iniel[jelem][2] == inode2) ? 2 : 3;
            ieln3 = ielnmap[ieln2][ieln1][2];

            jelem3 = ieliel[jelem][ieln3];

            ++i;
          }
          while (i <= nelem && jelem3 != ielem && jelem3 > 0);

          if (jelem3 == ielem)
          {
            m1 = 1;

            ni1 = i - 1;
          }
          else
            m1 = 0;
        }

        else if (jelem3 <= 0 && jelem4 <= 0)
          m1 = 2;

        else
          m1 = 3;

        jeln1 = 0;
        jeln2 = isliver;

        ieln1 = ielnmap[jeln2][jeln1][2];
        ieln2 = ielnmap[jeln2][jeln1][3];
        ieln3 = ielnmap[ieln2][ieln1][2];
        ieln4 = ielnmap[ieln2][ieln1][3];

        jelem3 = ieliel[ielem][ieln3];
        jelem4 = ieliel[ielem][ieln4];

        ni2 = 0;

        if (jelem3 > 0 && jelem4 > 0)
        {
          inode1 = iniel[ielem][ieln1];
          inode2 = iniel[ielem][ieln2];

          jelem3 = ielem;

          i = 1;

          do
          {
            jelem = jelem3;

            ieln1 = (iniel[jelem][0] == inode1) ? 0 :
                    (iniel[jelem][1] == inode1) ? 1 :
                    (iniel[jelem][2] == inode1) ? 2 : 3;
            ieln2 = (iniel[jelem][0] == inode2) ? 0 :
                    (iniel[jelem][1] == inode2) ? 1 :
                    (iniel[jelem][2] == inode2) ? 2 : 3;
            ieln3 = ielnmap[ieln2][ieln1][2];

            jelem3 = ieliel[jelem][ieln3];

            ++i;
          }
          while (i <= nelem && jelem3 != ielem && jelem3 > 0);

          if (jelem3 == ielem)
          {
            m2 = 1;

            ni2 = i - 1;
          }
          else
            m2 = 0;
        }

        else if (jelem3 <= 0 && jelem4 <= 0)
          m2 = 2;

        else
          m2 = 3;

        sprintf (Text, "Sliver Info        : Sliver Elem Index =%10i", ielem);
        ug_message (Text);

        sprintf (Text, "Sliver Info        : Sliver Max Angle  =%10g", angmax);
        ug_message (Text);

        if (m1 == 1)
        {
          sprintf (Text, "Sliver Info        : Elements at Edge 1=%10i", ni1);
          ug_message (Text);
        }

        else if (m1 == 2)
          ug_message ("Sliver Info        : Elements at Edge 1= B-Faces");

        else
          ug_message ("Sliver Info        : Elements at Edge 1= Field Elem/B-Face");

        if (m2 == 1)
        {
          sprintf (Text, "Sliver Info        : Elements at Edge 2=%10i", ni2);
          ug_message (Text);
        }

        else if (m2 == 2)
          ug_message ("Sliver Info        : Elements at Edge 2= B-Faces");

        else
          ug_message ("Sliver Info        : Elements at Edge 2= Field Elem/B-Face");
      }

      else
      {
        sprintf (Text, "Sliver Info        : Bad Elem Index    =%10i", ielem);
        ug_message (Text);

        sprintf (Text, "Sliver Info        : Bad Max Angle     =%10g", angmax);
        ug_message (Text);
      }
    }
  }

  return;

}
