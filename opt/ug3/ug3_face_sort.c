#include "UG3_LIB.h"

/*
 * Face sorting routines for face based data structures.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_face_sort.c,v 1.2 2012/08/25 19:01:45 marcum Exp $
 */

int ug3_tria_sort (const void *a, const void *b)
{
  INT_5D *a1;
  INT_5D *a2;

  int i = 0;

  a1 = (INT_5D *) a;
  a2 = (INT_5D *) b;

       if (a1[0][0] < a2[0][0]) i = -1;
  else if (a1[0][0] > a2[0][0]) i =  1;
  else if (a1[0][1] < a2[0][1]) i = -1;
  else if (a1[0][1] > a2[0][1]) i =  1;
  else if (a1[0][2] < a2[0][2]) i = -1;
  else if (a1[0][2] > a2[0][2]) i =  1;
  else if (a1[0][3] < a2[0][3]) i = -1;
  else if (a1[0][3] > a2[0][3]) i =  1;

  return (i);
}

int ug3_quad_sort (const void *a, const void *b)
{
  INT_6D *a1;
  INT_6D *a2;

  int i = 0;

  a1 = (INT_6D *) a;
  a2 = (INT_6D *) b;

       if (a1[0][0] < a2[0][0]) i = -1;
  else if (a1[0][0] > a2[0][0]) i =  1;
  else if (a1[0][1] < a2[0][1]) i = -1;
  else if (a1[0][1] > a2[0][1]) i =  1;
  else if (a1[0][2] < a2[0][2]) i = -1;
  else if (a1[0][2] > a2[0][2]) i =  1;
  else if (a1[0][3] < a2[0][3]) i = -1;
  else if (a1[0][3] > a2[0][3]) i =  1;
  else if (a1[0][4] < a2[0][4]) i = -1;
  else if (a1[0][4] > a2[0][4]) i =  1;

  return (i);
}

int ug3_bnd_tria_sort (const void *a, const void *b)
{
  INT_6D *a1;
  INT_6D *a2;

  int i = 0;

  a1 = (INT_6D *) a;
  a2 = (INT_6D *) b;

       if (a1[0][5] < a2[0][5]) i = -1;
  else if (a1[0][5] > a2[0][5]) i =  1;

  return (i);
}

int ug3_bnd_quad_sort (const void *a, const void *b)
{
  INT_7D *a1;
  INT_7D *a2;

  int i = 0;

  a1 = (INT_7D *) a;
  a2 = (INT_7D *) b;

       if (a1[0][6] < a2[0][6]) i = -1;
  else if (a1[0][6] > a2[0][6]) i =  1;

  return (i);
}

void ug3_swap_int (INT_ *a, INT_ *b)
{
  INT_ itmp;

  itmp = *a;
  *a = *b;
  *b = itmp;

  return;
} 
