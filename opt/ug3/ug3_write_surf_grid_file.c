#include "UG3_LIB.h"

INT_ ug3_write_surf_grid_file
 (char File_Name[],
  INT_ Message_Flag,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_1D * Surf_Reconnection_Flag,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Write grid data to a SURF surface grid file.
 * If Number_of_Nodes < 0 then do not remove unreference nodes and set
 * Number_of_Nodes = abs (Number_of_Nodes)
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_write_surf_grid_file.c,v 1.13 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX drive, dname, fname, ext;

  CHAR_31 Name_Text;
  CHAR_133 Text;

  FILE *Grid_File;

  INT_1D *Node_Index_Map;

  INT_ Error_Flag, Node_Index, Node_Index1, Node_Index2, Node_Index3,
       Node_Index4, Number_of_Bnd_Nodes, Node_Map_Flag, Surf_Index,
       Surf_Node_Index, Surf_Quad_Index, Surf_Tria_Index, Write_Flag;

  INT_ Surf_Grid_BC_Flag_ = 0;
  INT_ Surf_Reconnection_Flag_ = 0;

  Node_Map_Flag = (Number_of_Nodes > 0) ? 1: 0;

  Number_of_Nodes  = abs (Number_of_Nodes);

  Error_Flag = 0;

  Node_Index_Map = (INT_1D *) ug_malloc (&Error_Flag,
                                         (Number_of_Nodes+1) * sizeof (INT_1D));

  if (Error_Flag > 0)
    return (100307);

  if (Node_Map_Flag)
  {
    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      Node_Index_Map[Node_Index] = 0;
    }

    for (Surf_Tria_Index = 1;
         Surf_Tria_Index <= Number_of_Surf_Trias; ++Surf_Tria_Index)
    {
      Node_Index1 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
      Node_Index2 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
      Node_Index3 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

      Node_Index_Map[Node_Index1] = 1;
      Node_Index_Map[Node_Index2] = 1;
      Node_Index_Map[Node_Index3] = 1;
    }

    for (Surf_Quad_Index = 1;
         Surf_Quad_Index <= Number_of_Surf_Quads; ++Surf_Quad_Index)
    {
      Node_Index1 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
      Node_Index2 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
      Node_Index3 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
      Node_Index4 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

      Node_Index_Map[Node_Index1] = 1;
      Node_Index_Map[Node_Index2] = 1;
      Node_Index_Map[Node_Index3] = 1;
      Node_Index_Map[Node_Index4] = 1;
    }

    Surf_Node_Index = 0;

    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      if (Node_Index_Map[Node_Index] == 1)
      {
        ++Surf_Node_Index;

        Node_Index_Map[Node_Index] = Surf_Node_Index;
      }
    }

    Number_of_Bnd_Nodes = Surf_Node_Index;
  }
  else
  {
    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      Node_Index_Map[Node_Index] = Node_Index;
    }

    Number_of_Bnd_Nodes = Number_of_Nodes;
  }

  strcat (File_Name, ".surf");

  ug_backup_file (File_Name);

  if (ug_file_status_monitor_flag())
  {
    sprintf (Text, "ug3_write_surf_grid_file : writing SURF file with File_Name=%s",File_Name);
    ug_message (Text);
  }

  Grid_File = ug_fopen (File_Name, "w");

  if (Grid_File == NULL)
  {
    ug_free (Node_Index_Map);
    return (339);
  }

  if (Message_Flag >= 1)
  { 
    ug_message (" ");
    ug_cpu_message ("UG3      :");
    ug_message (" ");
    ug_message ("UG3      : OUTPUT GRID");
    ug_message (" ");

    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 30);

    sprintf (Text, "UG3      : Surf Grid FileName= %s", Name_Text);
    ug_message (Text);
  }

  Write_Flag = fprintf (Grid_File, "%i %i %i\n", Number_of_Surf_Trias,
                                                 Number_of_Surf_Quads,
                                                 Number_of_Bnd_Nodes);

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    ug_fclose (Grid_File);
    return (339);
  }

  for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
  {
    Surf_Node_Index = Node_Index_Map[Node_Index];

    if (Surf_Node_Index > 0)
    {
      Write_Flag = fprintf (Grid_File, "%.15g %.15g %.15g\n",
                            Coordinates[Node_Index][0],
                            Coordinates[Node_Index][1],
                            Coordinates[Node_Index][2]);
    }
  }

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    ug_fclose (Grid_File);
    return (339);
  }

  Surf_Index = 0;

  for (Surf_Tria_Index = 1;
       Surf_Tria_Index <= Number_of_Surf_Trias; ++Surf_Tria_Index)
  {
    ++Surf_Index;

    Node_Index1 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
    Node_Index2 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
    Node_Index3 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

    if (Surf_Reconnection_Flag != NULL)
      Surf_Reconnection_Flag_ = Surf_Reconnection_Flag[Surf_Index];

    if (Surf_Grid_BC_Flag != NULL)
      Surf_Grid_BC_Flag_ = Surf_Grid_BC_Flag[Surf_Index];

    Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i\n",
                          Node_Index_Map[Node_Index1],
                          Node_Index_Map[Node_Index2],
                          Node_Index_Map[Node_Index3],
                          Surf_ID_Flag[Surf_Index],
                          Surf_Reconnection_Flag_,
                          Surf_Grid_BC_Flag_);
  }

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    ug_fclose (Grid_File);
    return (339);
  }

  for (Surf_Quad_Index = 1;
       Surf_Quad_Index <= Number_of_Surf_Quads; ++Surf_Quad_Index)
  {
    ++Surf_Index;

    Node_Index1 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
    Node_Index2 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
    Node_Index3 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
    Node_Index4 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

    if (Surf_Reconnection_Flag != NULL)
      Surf_Reconnection_Flag_ = Surf_Reconnection_Flag[Surf_Index];

    if (Surf_Grid_BC_Flag != NULL)
      Surf_Grid_BC_Flag_ = Surf_Grid_BC_Flag[Surf_Index];

    Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i %i\n",
                          Node_Index_Map[Node_Index1],
                          Node_Index_Map[Node_Index2],
                          Node_Index_Map[Node_Index3],
                          Node_Index_Map[Node_Index4],
                          Surf_ID_Flag[Surf_Index],
                          Surf_Reconnection_Flag_,
                          Surf_Grid_BC_Flag_);
  }

  if (Write_Flag < 0)
  {
    ug_free (Node_Index_Map);
    ug_fclose (Grid_File);
    return (339);
  }

  ug_free (Node_Index_Map);

  ug_fclose (Grid_File);

  if (Message_Flag >= 1)
  { 
    sprintf (Text, "UG3      : Quad Surface Faces=%10i",
             Number_of_Surf_Quads);
    ug_message (Text);
    sprintf (Text, "UG3      : Tria Surface Faces=%10i",
             Number_of_Surf_Trias);
    ug_message (Text);
    sprintf (Text, "UG3      : Nodes             =%10i",
             Number_of_Nodes);
    ug_message (Text);
    ug_message (" ");
  }

  return (0);

}
