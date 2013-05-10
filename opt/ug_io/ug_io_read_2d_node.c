#include "UG_IO_LIB.h"

INT_ ug_io_read_2d_node
 (FILE * Node_Data_File,
  char Error_Message[],
  INT_ File_Type_Flag,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  DOUBLE_2D * Coordinates,
  DOUBLE_1D * Spacing,
  DOUBLE_3D * Vector)

{

/*
 * Read data from a 2D VNODE2D directional source node, SNODE2D source node, or
 * NODE2D node data file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_2d_node.c,v 1.9 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */


  CHAR_UG_MAX Text;

  char *Text_;

  INT_ Index, Length, Node_Index, Node_Index_1, Node_Index_2,
       Number_of_Read_Items, Read_Flag;

  double dc0, dc1, dc10, dc1d2, M_Vector,
         Spacing_, Spacing_Max, Spacing_Min, Spacing_Tol,
         Tol, v1, v2, Vector_DP, Vector_DP_Max, Vector_DP_Tol, w,
         X_Coordinate, X_Coordinate_Previous, X_Spacing, X_Vector,
         Y_Coordinate, Y_Coordinate_Previous, Y_Spacing, Y_Vector;

  dc0 = 0.0;
  dc1 = 1.0;
  dc10 = 10.0;
  dc1d2 = 0.5;

  ug_round_off_error (&Tol);

  Tol = dc10 * Tol;

  if (Read_Task_Flag == 1)
  {
    *Number_of_Nodes = 0;

    do
    {
      Index = 0;

      do
      {
        Length = 0;

        Text_ = &Text[0];

        Text_ = fgets (Text_, (int) UG_MAX_CHAR_STRING_LENGTH, Node_Data_File);

        if (Text_ != NULL)
        {
          Length = (INT_) strlen (Text_);

          if (Length != 1)
          {
            if (strncmp (Text_, "#", 1) != 0)
              ++Index;
          }
        }
      }
      while (Length > 1 && Text_ != NULL);

      if (Index > 0)
        *Number_of_Nodes = *Number_of_Nodes + Index;
    }
    while (Text_ != NULL);

    if (*Number_of_Nodes == 0)
    {
      if (File_Type_Flag == 1)
        strcpy (Error_Message,
                "no nodes in 2D NODE2D node data file");
      else if (File_Type_Flag == 2)
        strcpy (Error_Message,
                "no nodes in 2D SNODE2D source node data file");
      else
        strcpy (Error_Message,
                "no nodes in 2D VNODE2D directional source node data file");
      return (1);
    }
  }

  else if (Read_Task_Flag == 2)
  {
    Spacing_ = dc0;

    X_Vector = dc1;
    Y_Vector = dc0;
    M_Vector = dc1;

    Number_of_Read_Items = (File_Type_Flag == 1) ? 2:
                           (File_Type_Flag == 2) ? 3: 6;

    Node_Index = 0;

    Node_Index_2 = 0;

    Read_Flag = 1;

    do
    {
      Index = 0;

      do
      {
        Length = 0;

        Text_ = &Text[0];

        Text_ = fgets (Text_, (int) UG_MAX_CHAR_STRING_LENGTH, Node_Data_File);

        if (Text_ != NULL)
        {
          Length = (INT_) strlen (Text_);

          if (Length != 1)
          {
            if (strncmp (Text_, "#", 1) != 0)
            {
              ++Index;

              ++Node_Index;

              if (File_Type_Flag == 1)
                Read_Flag = sscanf (Text_, "%lg %lg",
                                    &X_Coordinate, &Y_Coordinate);

              else if (File_Type_Flag == 2)
                Read_Flag = sscanf (Text_, "%lg %lg %lg",
                                    &X_Coordinate, &Y_Coordinate,
                                    &Spacing_);

              else
                Read_Flag = sscanf (Text_, "%lg %lg %lg %lg %lg %lg",
                                    &X_Coordinate, &Y_Coordinate,
                                    &Spacing_,
                                    &X_Vector, &Y_Vector, &M_Vector);

              if (Read_Flag != Number_of_Read_Items)
                Read_Flag = 0;

              if (Read_Flag != 0 && Node_Index <= *Number_of_Nodes)
              {
                Coordinates[Node_Index][0] = X_Coordinate;
                Coordinates[Node_Index][1] = Y_Coordinate;

                if (Spacing != NULL)
                  Spacing[Node_Index] = Spacing_;

                if (Vector != NULL)
                {
                  Vector[Node_Index][0] = X_Vector;
                  Vector[Node_Index][1] = Y_Vector;
                  Vector[Node_Index][2] = M_Vector;
                }
              }
            }
          }
        }
      }
      while (Read_Flag != 0 && Length > 1 && Text_ != NULL);

      if (File_Type_Flag == 1 &&
          Read_Flag != 0 && Index > 0 && (Spacing != NULL || Vector != NULL))
      {
        Node_Index_1 = Node_Index_2 + 1;
        Node_Index_2 = Node_Index_2 + Index;

        Index = Node_Index_1;

        X_Coordinate = Coordinates[Index][0];
        Y_Coordinate = Coordinates[Index][1];

        X_Coordinate_Previous = X_Coordinate;
        Y_Coordinate_Previous = Y_Coordinate;

        for (Index = Node_Index_1+1; Index <= Node_Index_2; ++Index)
        {
          X_Coordinate = Coordinates[Index][0];
          Y_Coordinate = Coordinates[Index][1];

          X_Spacing = X_Coordinate - X_Coordinate_Previous;
          Y_Spacing = Y_Coordinate - Y_Coordinate_Previous;

          if (Spacing != NULL)
            Spacing[Index] = sqrt (X_Spacing * X_Spacing
                                 + Y_Spacing * Y_Spacing);

          if (Vector != NULL)
          {
            Vector[Index][0] = -Y_Spacing;
            Vector[Index][1] = X_Spacing;
          }

          X_Coordinate_Previous = X_Coordinate;
          Y_Coordinate_Previous = Y_Coordinate;
        }

        if (Spacing != NULL)
        {
          Index = Node_Index_1;

          Spacing[Index] = Spacing[Index+1];

          for (Index = Node_Index_1+1; Index <= Node_Index_2-1; ++Index)
          {
            Spacing[Index] = dc1d2 * (Spacing[Index] + Spacing[Index+1]);
          }
        }

        if (Vector != NULL)
        {
          Index = Node_Index_1;

          Vector[Index][0] = Vector[Index+1][0];
          Vector[Index][1] = Vector[Index+1][1];
          Vector[Index][2] = dc1;

          for (Index = Node_Index_1+1; Index <= Node_Index_2-1; ++Index)
          {
            Vector[Index][0] = dc1d2 * (Vector[Index][0] + Vector[Index+1][0]);
            Vector[Index][1] = dc1d2 * (Vector[Index][1] + Vector[Index+1][1]);
            Vector[Index][2] = dc1;
          }
        }
      }
    }
    while (Read_Flag != 0 && Text_ != NULL);

    if (Node_Index != *Number_of_Nodes)
    {
      if (File_Type_Flag == 1)
        strcpy (Error_Message,
                "error reading 2D NODE2D node data file");
      else if (File_Type_Flag == 2)
        strcpy (Error_Message,
                "error reading 2D SNODE2D source node data file");
      else
        strcpy (Error_Message,
                "error reading 2D VNODE2D directional source node data file");
      return (1);
    }

    if (Spacing != NULL)
    {
      Node_Index = 1;

      Spacing_ = Spacing[Node_Index];

      Spacing_Max = Spacing_;
      Spacing_Min = Spacing_;

      for (Node_Index = 2; Node_Index <= *Number_of_Nodes; ++Node_Index)
      {
        Spacing_ = Spacing[Node_Index];

        Spacing_Max = MAX (Spacing_Max, Spacing_);
        Spacing_Min = MIN (Spacing_Min, Spacing_);
      }

      Spacing_Tol = Tol * Spacing_Max;

      if (Spacing_Min < Spacing_Tol || Spacing_Tol <= dc0)
      {
        Node_Index = 1;

        do
        {
          Spacing_ = Spacing[Node_Index];

          ++Node_Index;
        }
        while (Node_Index <= *Number_of_Nodes &&
               Spacing_ >= Spacing_Tol && Spacing_ > dc0);

        --Node_Index;

        if (File_Type_Flag == 1)
          sprintf (Error_Message,
                  "zero or negative spacing function calculated at node %i",
                  Node_Index);
        else
          sprintf (Error_Message,
                  "zero or negative spacing function read at node %i",
                  Node_Index);
        return (1);
      }
    }

    if (Vector != NULL)
    {
      Vector_DP_Max = dc0;

      for (Node_Index = 1; Node_Index <= *Number_of_Nodes; ++Node_Index)
      {
        v1 = Vector[Node_Index][0];
        v2 = Vector[Node_Index][1];

        Vector_DP = v1 * v1 + v2 * v2;

        Vector_DP_Max = MAX (Vector_DP_Max, Vector_DP);
      }

      Vector_DP_Tol = Tol * Vector_DP_Max;

      if (Vector_DP_Tol > dc0)
      {
        Node_Index = 1;

        do
        {
          v1 = Vector[Node_Index][0];
          v2 = Vector[Node_Index][1];

          Vector_DP = v1 * v1 + v2 * v2;

          if (Vector_DP >= Vector_DP_Tol)
          {
            w = dc1 / sqrt (Vector_DP);

            Vector[Node_Index][0] = w * v1;
            Vector[Node_Index][1] = w * v2;
            Vector[Node_Index][2] = MAX (Vector[Node_Index][2], dc1);
          }

          ++Node_Index;
        }
        while (Node_Index <= *Number_of_Nodes && Vector_DP >= Vector_DP_Tol);
      }

      if (Vector_DP < Vector_DP_Tol || Vector_DP_Tol <= dc0)
      {
        Node_Index = 1;

        do
        {
          v1 = Vector[Node_Index][0];
          v2 = Vector[Node_Index][1];

          Vector_DP = v1 * v1 + v2 * v2;

          ++Node_Index;
        }
        while (Node_Index <= *Number_of_Nodes &&
               Vector_DP >= Vector_DP_Tol && Vector_DP > dc0);

        --Node_Index;

        if (File_Type_Flag <= 2)
          sprintf (Error_Message,
                  "zero magnitude vector calculated at node %i", Node_Index);
        else
          sprintf (Error_Message,
                  "zero magnitude vector read at node %i", Node_Index);
        return (1);
      }
    }
  }

  return (0);

}
