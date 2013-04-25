#include "UG_IO_LIB.h"

INT_ ug_io_read_2d_bedge
 (FILE * Grid_File,
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_Bnd_Edges,
  INT_ *Number_of_Nodes,
  INT_2D * Bnd_Edge_Connectivity,
  INT_1D * Bnd_Edge_Grid_BC_Flag,
  INT_1D * Bnd_Edge_ID_Flag,
  DOUBLE_2D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Read grid data from a 2D BEDGE boundary edge grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_2d_bedge.c,v 1.8 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *Number_of_Sub_Curves, *Number_of_Sub_Curve_Nodes,
         *Sub_Curve_Grid_BC_Flag;

  INT_ Curve_Index, Error_Flag, Grid_BC_Flag, Index, Index_0, Index_1, Index_2,
       Number_of_Curves, Number_of_Sub_Curve_Nodes_, Number_of_Sub_Curves_,
       Number_of_Sub_Curves_Total, Read_Flag, Sub_Curve_Index,
       Sub_Curve_Index_1, Sub_Curve_Index_2;

  int true_int = 0;

  double dc0;

  dc0 = 0.0;

  if (Read_Task_Flag == 1)
  {
    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    Number_of_Curves = (INT_) true_int;

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      return (1);
    }

    Number_of_Sub_Curves_Total = 0;

    for (Curve_Index = 0; Curve_Index < Number_of_Curves; ++Curve_Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Number_of_Sub_Curves_ = (INT_) true_int;

      Number_of_Sub_Curves_Total = Number_of_Sub_Curves_Total
                                 + Number_of_Sub_Curves_;
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      return (1);
    }

    *Number_of_Nodes = 0;

    for (Sub_Curve_Index = 0;
         Sub_Curve_Index < Number_of_Sub_Curves_Total; ++Sub_Curve_Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Number_of_Sub_Curve_Nodes_ = (INT_) true_int;

      *Number_of_Nodes = *Number_of_Nodes + Number_of_Sub_Curve_Nodes_;
    }

    *Number_of_Bnd_Edges = *Number_of_Nodes;

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      return (1);
    }
  }
  else
  {
    Read_Flag = fscanf (Grid_File, "%i", &true_int);

    Number_of_Curves = (INT_) true_int;

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      return (1);
    }

    Error_Flag = 0;
 
    Number_of_Sub_Curves = (INT_1D *) ug_malloc (&Error_Flag,
                                                 Number_of_Curves
                                               * sizeof (INT_1D));
 
    if (Error_Flag > 0)
    {
      strcpy (Error_Message,
              "unable to malloc temporary space for 2D BEDGE grid file");
      ug_free (Number_of_Sub_Curves);
      return (1);
    }

    Number_of_Sub_Curves_Total = 0;
 
    for (Curve_Index = 0; Curve_Index < Number_of_Curves; ++Curve_Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Number_of_Sub_Curves_ = (INT_) true_int;

      Number_of_Sub_Curves_Total = Number_of_Sub_Curves_Total
                                 + Number_of_Sub_Curves_;

      Number_of_Sub_Curves[Curve_Index] = Number_of_Sub_Curves_;
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      ug_free (Number_of_Sub_Curves);
      return (1);
    }
 
    Error_Flag = 0;
 
    Number_of_Sub_Curve_Nodes = (INT_1D *) ug_malloc (&Error_Flag,
                                                      Number_of_Sub_Curves_Total
                                                    * sizeof (INT_1D));
    Sub_Curve_Grid_BC_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                                   Number_of_Sub_Curves_Total
                                                 * sizeof (INT_1D));
 
    if (Error_Flag > 0)
    {
      strcpy (Error_Message,
              "unable to malloc temporary space for 2D BEDGE grid file");
      ug_free (Number_of_Sub_Curves);
      ug_free (Number_of_Sub_Curve_Nodes);
      ug_free (Sub_Curve_Grid_BC_Flag);
      return (1);
    }

    for (Sub_Curve_Index = 0;
         Sub_Curve_Index < Number_of_Sub_Curves_Total; ++Sub_Curve_Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Number_of_Sub_Curve_Nodes[Sub_Curve_Index] = (INT_) true_int;
    }
 
    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      ug_free (Number_of_Sub_Curves);
      ug_free (Number_of_Sub_Curve_Nodes);
      ug_free (Sub_Curve_Grid_BC_Flag);
      return (1);
    }

    for (Sub_Curve_Index = 0;
         Sub_Curve_Index < Number_of_Sub_Curves_Total; ++Sub_Curve_Index)
    {
      Read_Flag = fscanf (Grid_File, "%i", &true_int);

      Sub_Curve_Grid_BC_Flag[Sub_Curve_Index] = (INT_) true_int;
    }
 
    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      ug_free (Number_of_Sub_Curves);
      ug_free (Number_of_Sub_Curve_Nodes);
      ug_free (Sub_Curve_Grid_BC_Flag);
      return (1);
    }

    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
    {
      Read_Flag = fscanf (Grid_File, "%lf %lf",
                          &Coordinates[Index][0],
                          &Coordinates[Index][1]);
    }

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading 2D BEDGE grid file");
      ug_free (Number_of_Sub_Curves);
      ug_free (Number_of_Sub_Curve_Nodes);
      ug_free (Sub_Curve_Grid_BC_Flag);
      return (1);
    }

    if (Initial_Normal_Spacing != NULL)
    {
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%lf",
                            &Initial_Normal_Spacing[Index]);
      }

      if (Read_Flag == EOF)
      {
        for (Index = 1; Index <= *Number_of_Nodes; ++Index)
        {
          Initial_Normal_Spacing[Index] = dc0;
        }
      }
    }

    if (BL_Thickness != NULL)
    {
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%lf", &BL_Thickness[Index]);
      }

      if (Read_Flag == EOF)
      {
        for (Index = 1; Index <= *Number_of_Nodes; ++Index)
        {
          BL_Thickness[Index] = dc0;
        }
      }
    }

    Index_2 = 0;

    Sub_Curve_Index_2 = -1;

    for (Curve_Index = 0; Curve_Index < Number_of_Curves; ++Curve_Index)
    {
      Index_0 = Index_2 + 1;

      Sub_Curve_Index_1 = Sub_Curve_Index_2 + 1;
      Sub_Curve_Index_2 = Sub_Curve_Index_2 + Number_of_Sub_Curves[Curve_Index];

      for (Sub_Curve_Index = Sub_Curve_Index_1;
           Sub_Curve_Index <= Sub_Curve_Index_2; ++Sub_Curve_Index)
      {
        Grid_BC_Flag = Sub_Curve_Grid_BC_Flag[Sub_Curve_Index];

        Index_1 = Index_2 + 1;
        Index_2 = Index_2 + Number_of_Sub_Curve_Nodes[Sub_Curve_Index];

        for (Index = Index_1; Index < Index_2; ++Index)
        {
          Bnd_Edge_Connectivity[Index][0] = Index;
          Bnd_Edge_Connectivity[Index][1] = Index + 1;

          if (Bnd_Edge_Grid_BC_Flag != NULL)
            Bnd_Edge_Grid_BC_Flag[Index] = Grid_BC_Flag;

          if (Bnd_Edge_ID_Flag != NULL)
            Bnd_Edge_ID_Flag[Index] = Sub_Curve_Index;
        }

        Index = Index_2;

        Index_1 = (Sub_Curve_Index < Sub_Curve_Index_2) ? Index+1 : Index_0;
         
        Bnd_Edge_Connectivity[Index][0] = Index;
        Bnd_Edge_Connectivity[Index][1] = Index_1;

        if (Bnd_Edge_Grid_BC_Flag != NULL)
            Bnd_Edge_Grid_BC_Flag[Index] = Grid_BC_Flag;

        if (Bnd_Edge_ID_Flag != NULL)
            Bnd_Edge_ID_Flag[Index] = Sub_Curve_Index;
      }
    }

    ug_free (Number_of_Sub_Curves);
    ug_free (Number_of_Sub_Curve_Nodes);
    ug_free (Sub_Curve_Grid_BC_Flag);

    if (Bnd_Edge_ID_Flag != NULL)
    {
      for (Index = 1; Index <= *Number_of_Bnd_Edges; ++Index)
      {
        Bnd_Edge_ID_Flag[Index] = Bnd_Edge_ID_Flag[Index] + 1;
      }
    }
  }

  return (0);

}
