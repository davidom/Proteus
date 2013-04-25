#include "UG_IO_LIB.h"

void ug_io_read_gdata
 (FILE * Grid_File,
  CHAR_21 File_Type_Suffix_,
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ Number_of_Bnd_Nodes,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6, 
  INT_ Number_of_Vol_Tets,
  INT_ *Number_of_BL_Vol_Tets,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_Reconnection_Flag,
  INT_1D * Vol_ID_Flag,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Read volume ID and number of BL tets (volume grids only) and write boundary
 * surface grid BC, boundary surface reconection flag, BL initial spacing, and
 * BL thicknes.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_gdata.c,v 1.10 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index, Number_of_Bytes, Number_of_Read_Items, Number_of_Surf_Faces,
       Number_of_Vol_Elems, Read_Flag, Special_Read_Items;

  int TMP_int = 0;

  double TMP_double;

  float fc0, TMP_float;

  fc0 = 0.0;

  if (Read_Task_Flag != 2 || strcmp (File_Type_Suffix_, ".ufast") == 0)
    return;

  Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;

  Number_of_Vol_Elems = Number_of_Vol_Hexs + Number_of_Vol_Pents_5
                      + Number_of_Vol_Pents_6 + Number_of_Vol_Tets;

  Read_Flag = 1;

  *Number_of_BL_Vol_Tets = 0;

  if (Number_of_Vol_Elems > 0)
  {
    if (File_Format == UG_IO_FORMATTED)
    {
      Read_Flag = fscanf (Grid_File, "%i", &TMP_int);

      *Number_of_BL_Vol_Tets = (INT_) TMP_int;

      if (Read_Flag != EOF)
        Read_Flag = 1;
      else
      {
        Read_Flag = 0;

        *Number_of_BL_Vol_Tets = 0;
      }

      if (Read_Flag == 1)
      {
        for (Index = 1; Index <= Number_of_Vol_Elems; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%i", &TMP_int);

          if (Read_Flag != EOF && Vol_ID_Flag != NULL &&
              Number_of_Bnd_Nodes == 0)
            Vol_ID_Flag[Index] = (INT_) TMP_int;
        }

        if (Read_Flag != EOF)
          Read_Flag = 1;
        else
          Read_Flag = 0;
      }
    }
    else
    {
      Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                             File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag
                  + ug_fread (&Number_of_Bytes, sizeof (INT_), 1, Grid_File);

      Read_Flag = Read_Flag + ug_fread (Number_of_BL_Vol_Tets,
                                        sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag
                  + ug_fread (&Number_of_Bytes, sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = 1 + Special_Read_Items + Special_Read_Items;

      if (Read_Flag == Number_of_Read_Items)
        Read_Flag = 1;
      else
      {
        Read_Flag = 0;

        *Number_of_BL_Vol_Tets = 0;
      }

      if (Read_Flag == 1)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Grid_File);

        for (Index = 1; Index <= Number_of_Vol_Elems; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_1D), 1, Grid_File);

          Number_of_Read_Items = Index + Special_Read_Items;

          if (Read_Flag == Number_of_Read_Items && Vol_ID_Flag != NULL &&
              Number_of_Bnd_Nodes == 0)
            Vol_ID_Flag[Index] = (INT_) TMP_int;
        }

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Grid_File);

        Number_of_Read_Items = Number_of_Vol_Elems
                             + Special_Read_Items + Special_Read_Items;

        if (Read_Flag == Number_of_Read_Items)
          Read_Flag = 1;
        else
          Read_Flag = 0;
      }
    }
  }

  if (File_Format == UG_IO_FORMATTED)
  {
    if (Read_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int);

        TMP_int = MAX (TMP_int, 0);
        TMP_int = MIN (TMP_int, 7);

        if (Surf_Reconnection_Flag != NULL)
          Surf_Reconnection_Flag[Index] = (INT_) TMP_int;
      }

      if (Read_Flag != EOF)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }

    if (Read_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int);

        if (Surf_Grid_BC_Flag != NULL)
          Surf_Grid_BC_Flag[Index] = (INT_) TMP_int;
      }

      if (Read_Flag != EOF)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }

    if (Read_Flag == 1 && Number_of_Vol_Elems == 0)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%lf", &TMP_double);

        if (Initial_Normal_Spacing != NULL)
          Initial_Normal_Spacing[Index] = TMP_double;
      }

      if (Read_Flag != EOF)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }

    if (Read_Flag == 1 && Number_of_Vol_Elems == 0)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%lf", &TMP_double);

        if (BL_Thickness != NULL)
          BL_Thickness[Index] = TMP_double;
      }

      if (Read_Flag != EOF)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }
  }
  else
  {
    Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                           File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    if (Read_Flag == 1)
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
      {
        Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                          sizeof (INT_1D), 1, Grid_File);

        TMP_int = MAX (TMP_int, 0);
        TMP_int = MIN (TMP_int, 7);

        if (Surf_Reconnection_Flag != NULL)
          Surf_Reconnection_Flag[Index] = (INT_) TMP_int;
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = Number_of_Surf_Faces
                           + Special_Read_Items + Special_Read_Items;

      if (Read_Flag == Number_of_Read_Items)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }

    if (Read_Flag == 1)
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
      {
        Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                          sizeof (INT_1D), 1, Grid_File);

        if (Surf_Grid_BC_Flag != NULL)
          Surf_Grid_BC_Flag[Index] = (INT_) TMP_int;
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = Number_of_Surf_Faces
                           + Special_Read_Items + Special_Read_Items;

      if (Read_Flag == Number_of_Read_Items)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }

    if (Read_Flag == 1 && Number_of_Vol_Elems == 0)
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
          File_Format == UG_IO_BINARY_SINGLE)
      {
        TMP_float = fc0;

        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          Read_Flag = Read_Flag
                    + ug_fread (&TMP_float, sizeof (float), 1, Grid_File);
 
          if (Initial_Normal_Spacing != NULL)
            Initial_Normal_Spacing[Index] = (double) TMP_float;
        }
      }
      else
      {
        TMP_double = fc0;

        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          Read_Flag = Read_Flag
                    + ug_fread (&TMP_double, sizeof (double), 1, Grid_File);
 
          if (Initial_Normal_Spacing != NULL)
            Initial_Normal_Spacing[Index] = (double) TMP_double;
        }
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = Number_of_Surf_Faces
                           + Special_Read_Items + Special_Read_Items;

      if (Read_Flag == Number_of_Read_Items)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }

    if (Read_Flag == 1 && Number_of_Vol_Elems == 0)
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
          File_Format == UG_IO_BINARY_SINGLE)
      {
        TMP_float = fc0;

        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          Read_Flag = Read_Flag
                    + ug_fread (&TMP_float, sizeof (float), 1, Grid_File);
 
          if (BL_Thickness != NULL)
            BL_Thickness[Index] = (double) TMP_float;
        }
      }
      else
      {
        TMP_double = fc0;

        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          Read_Flag = Read_Flag
                    + ug_fread (&TMP_double, sizeof (double), 1, Grid_File);
 
          if (BL_Thickness!= NULL)
            BL_Thickness[Index] = (double) TMP_double;
        }
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = Number_of_Surf_Faces
                           + Special_Read_Items + Special_Read_Items;

      if (Read_Flag == Number_of_Read_Items)
        Read_Flag = 1;
      else
        Read_Flag = 0;
    }
  }

  return;

}
