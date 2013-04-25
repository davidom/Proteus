#include "UG_IO_LIB.h"

INT_ ug_io_write_2d_node
 (FILE * Node_Data_File,
  char Error_Message[],
  INT_ File_Type_Flag,
  INT_ Number_of_Nodes,
  DOUBLE_2D * Coordinates,
  DOUBLE_1D * Spacing,
  DOUBLE_3D * Vector)

{

/*
 * Write node data to a 2D VNODE2D directional source node, SNODE2D source node,
 * or NODE2D node data file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_2d_node.c,v 1.3 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Node_Index, Write_Flag;

  double dc0, dc1, Spacing_, Vector1, Vector2, Vectorm;

  dc0 = 0.0;
  dc1 = 1.0;

  if (File_Type_Flag == 1)
  {
    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      Write_Flag = fprintf (Node_Data_File, "%.15g %.15g\n",
                            Coordinates[Node_Index][0],
                            Coordinates[Node_Index][1]);
    }
  }

  else if (File_Type_Flag == 2)
  {
    Spacing_ = dc0;

    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      if (Spacing != NULL)
        Spacing_ = Spacing[Node_Index];

      Write_Flag = fprintf (Node_Data_File, "%.15g %.15g %.15g\n",
                            Coordinates[Node_Index][0],
                            Coordinates[Node_Index][1],
                            Spacing_);
    }
  }

  else
  {
    Spacing_ = dc0;
    Vector1 = dc1;
    Vector2 = dc0;
    Vectorm = dc1;

    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      if (Spacing != NULL)
        Spacing_ = Spacing[Node_Index];

      if (Vector != NULL)
      {
        Vector1 = Vector[Node_Index][0];
        Vector2 = Vector[Node_Index][1];
        Vectorm = Vector[Node_Index][2];
      }

      Write_Flag = fprintf (Node_Data_File,
                            "%.15g %.15g %.15g %.15g %.15g %.15g\n",
                            Coordinates[Node_Index][0],
                            Coordinates[Node_Index][1],
                            Spacing_,
                            Vector1, Vector2, Vectorm);
    }
  }

  if (Write_Flag < 0)
  {
    if (File_Type_Flag == 1)
      strcpy (Error_Message,
              "error writing 2D NODE2D node data file");
    else if (File_Type_Flag == 2)
      strcpy (Error_Message,
              "error writing 2D SNODE2D source node data file");
    else
      strcpy (Error_Message,
              "error writing 2D VNODE2D directional source node data file");
    return (1);
  }

  return (0);

}
