#include "UG_IO_LIB.h"

INT_ ug_io_write_node
 (FILE * Node_Data_File,
  char Error_Message[],
  INT_ File_Type_Flag,
  INT_ Number_of_Nodes,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Spacing,
  DOUBLE_4D * Vector1,
  DOUBLE_4D * Vector2)

{

/*
 * Write node data to a 3D VNODE directional source node, SNODE source node, or
 * NODE node data file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_node.c,v 1.4 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Node_Index, Write_Flag;

  double dc0, dc1, Spacing_, Vector11, Vector12, Vector13, Vector1m, Vector21,
         Vector22, Vector23, Vector2m;

  dc0 = 0.0;
  dc1 = 1.0;

  if (File_Type_Flag == 1)
  {
    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      Write_Flag = fprintf (Node_Data_File, "%.15g %.15g %.15g\n",
                            Coordinates[Node_Index][0],
                            Coordinates[Node_Index][1],
                            Coordinates[Node_Index][2]);
    }
  }

  else if (File_Type_Flag == 2)
  {
    Spacing_ = dc0;

    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      if (Spacing != NULL)
        Spacing_ = Spacing[Node_Index];

      Write_Flag = fprintf (Node_Data_File, "%.15g %.15g %.15g %.15g\n",
                            Coordinates[Node_Index][0],
                            Coordinates[Node_Index][1],
                            Coordinates[Node_Index][2],
                            Spacing_);
    }
  }

  else
  {
    Spacing_ = dc0;
    Vector11 = dc1;
    Vector12 = dc0;
    Vector13 = dc0;
    Vector1m = dc1;
    Vector21 = dc1;
    Vector22 = dc0;
    Vector23 = dc0;
    Vector2m = dc1;

    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      if (Spacing != NULL)
        Spacing_ = Spacing[Node_Index];

      if (Vector1 != NULL)
      {
        Vector11 = Vector1[Node_Index][0];
        Vector12 = Vector1[Node_Index][1];
        Vector13 = Vector1[Node_Index][2];
        Vector1m = Vector1[Node_Index][3];
      }

      if (Vector2 != NULL)
      {
        Vector21 = Vector2[Node_Index][0];
        Vector22 = Vector2[Node_Index][1];
        Vector23 = Vector2[Node_Index][2];
        Vector2m = Vector2[Node_Index][3];
      }

      Write_Flag = fprintf (Node_Data_File, "%.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g\n",
                            Coordinates[Node_Index][0],
                            Coordinates[Node_Index][1],
                            Coordinates[Node_Index][2],
                            Spacing_,
                            Vector11, Vector12, Vector13, Vector1m, 
                            Vector21, Vector22, Vector23, Vector2m);
    }
  }

  if (Write_Flag < 0)
  {
    if (File_Type_Flag == 1)
      strcpy (Error_Message,
              "error writing NODE node data file");
    else if (File_Type_Flag == 2)
      strcpy (Error_Message,
              "error writing SNODE source node data file");
    else
      strcpy (Error_Message,
              "error writing VNODE directional source node data file");
    return (1);
  }

  return (0);

}
