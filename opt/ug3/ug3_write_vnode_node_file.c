#include "UG3_LIB.h"

INT_ ug3_write_vnode_node_file
 (char File_Name[],
  INT_ Message_Flag,
  INT_ Number_of_Nodes,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Spacing,
  DOUBLE_4D * Vector1,
  DOUBLE_4D * Vector2)

{

/*
 * Write node data to a 3D VNODE directional source node data file.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_write_vnode_node_file.c,v 1.9 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Data_File;

  CHAR_UG_MAX drive, dname, fname, ext;

  CHAR_31 Name_Text;
  CHAR_133 Text;

  INT_ Node_Index, Write_Flag;

  double dc0, dc1, Spacing_, Vector11, Vector12, Vector13, Vector1m, Vector21,
         Vector22, Vector23, Vector2m;

  dc0 = 0.0;
  dc1 = 1.0;

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG3      : OUTPUT SOURCE NODE DATA");
    ug_message (" ");
    ug_message ("UG3      : Writing Data");
    ug_message (" ");
  }

  if (ug_file_status_monitor_flag())
  {
    sprintf (Text, "ug3_write_vnode_node_file: writing VNODE file with File_Name=%s",File_Name);
    ug_message (Text);
  }

  ug_backup_file (File_Name);

  Data_File = ug_fopen (File_Name, "w");

  if (Data_File == NULL)
    return (346);

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

    Write_Flag = fprintf (Data_File, "%.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g\n",
                          Coordinates[Node_Index][0],
                          Coordinates[Node_Index][1],
                          Coordinates[Node_Index][2],
                          Spacing_,
                          Vector11, Vector12, Vector13, Vector1m, 
                          Vector21, Vector22, Vector23, Vector2m);
  }

  ug_fclose (Data_File);

  if (Write_Flag < 0)
    return (346);

  if (Message_Flag >= 1)
  { 
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 30);

    sprintf (Text, "UG3      : NodeData File Name= %s", Name_Text);
    ug_message (Text);
    sprintf (Text, "UG3      : Source Nodes      =%10i", Number_of_Nodes);
    ug_message (Text);
    ug_message (" ");
  }

  return (0);

}
