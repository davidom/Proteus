#include "UG3_LIB.h"

void ug3_dv
 (INT_ ielem,
  INT_4D * iniel,
  double *dvi,
  double *xe1,
  double *xe2,
  double *xe3,
  DOUBLE_3D * x)

{

/*
 * Determine circum-radius and circum-center coordinates for a given element.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_dv.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ inode1, inode2, inode3, inode4;

  double a1342s1, a1343s1, a1341s2, a1343s2, a1341s3, a1342s3, a1431, a1432,
         a1433,  dc1d2, detr, dx1, dx2, dx211, dx212, dx213, dx21s, dx3, dx311,
         dx312, dx313, dx31s, dx411, dx412, dx413, dx41s, x11, x12, x13, x21,
         x22, x23, x31, x32, x33, x41, x42, x43;

  dc1d2 = 0.5;

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

  a1431 = dx412 * dx313 - dx413 * dx312;
  a1432 = dx413 * dx311 - dx411 * dx313;
  a1433 = dx411 * dx312 - dx412 * dx311;

  dx21s = dx211 * dx211
                + dx212 * dx212
                + dx213 * dx213;
  dx31s = dx311 * dx311
                + dx312 * dx312
                + dx313 * dx313;
  dx41s = dx411 * dx411
                + dx412 * dx412
                + dx413 * dx413;

  detr = -dc1d2 / (dx211 * a1431
                 + dx212 * a1432
                 + dx213 * a1433);

  a1342s1 = dx313 * dx41s - dx31s * dx413;
  a1343s1 = dx31s * dx412 - dx312 * dx41s;
  a1341s2 = dx31s * dx413 - dx313 * dx41s;
  a1343s2 = dx311 * dx41s - dx31s * dx411;
  a1341s3 = dx312 * dx41s - dx31s * dx412;
  a1342s3 = dx31s * dx411 - dx311 * dx41s;

  dx1 = detr * (- dx21s * a1431  
                + dx212 * a1342s1
                + dx213 * a1343s1);
  dx2 = detr * (  dx211 * a1341s2
                - dx21s * a1432
                + dx213 * a1343s2);
  dx3 = detr * (  dx211 * a1341s3
                + dx212 * a1342s3
                - dx21s * a1433);

  *dvi = dx1 * dx1 + dx2 * dx2 + dx3 * dx3;

  *xe1 = x11 + dx1;
  *xe2 = x12 + dx2;
  *xe3 = x13 + dx3;

  return;

}
