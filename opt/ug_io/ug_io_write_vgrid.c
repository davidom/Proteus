#include "UG_IO_LIB.h"

INT_ ug_io_write_vgrid
 (FILE *Grid_File,
  char Case_Name[],
  char Error_Message[],
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Tets,
  INT_ Number_of_BL_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Write grid data to a VGRID volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_vgrid.c,v 1.7 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *BC_File = NULL;
  FILE *MapBC_File = NULL;

  CHAR_81 Compile_Date;
  CHAR_81 Compile_OS;
  CHAR_81 Version_Date;
  CHAR_81 Version_Number;
  CHAR_UG_MAX BC_File_Name;
  CHAR_UG_MAX MapBC_File_Name;

  INT_1D *ID_Map = NULL;
  INT_1D *New_Node_List = NULL;
  INT_1D *New_Surf_ID_Flag = NULL;
  INT_1D *Node_List = NULL;
  INT_1D *Node_Map = NULL;

  INT_ inew, mtype, n, Error_Flag, Geom_Index, Index, Max_Node, Min_Node,
       Max_Surf_ID, Min_Surf_ID, Number_of_Bnd_Nodes, Number_of_Bytes,
       Number_of_Surf_IDs, Number_of_Write_Items, Number_of_Viscous_Nodes,
       Number_of_Viscous_Tets, Write_Flag;

  double TMP_double;

  inew = -1;
  mtype = 1;
  Max_Surf_ID = 0;
  Number_of_Viscous_Nodes = 0;
  Number_of_Viscous_Tets = 0;

  // determine the number of boundary nodes
  // this is only needed if the boundary nodes are not first in the ordering
  // or if there are quad surface faces 

  n = 3*Number_of_Surf_Trias;

  Node_List = (INT_1D *) Surf_Tria_Connectivity;

  Number_of_Bnd_Nodes = 0;

  Error_Flag = ug_renumber_int_values (n, &Number_of_Bnd_Nodes, &Max_Node,
                                       &Min_Node,
                                       Node_List, New_Node_List,
                                       &Node_Map);

  if (Error_Flag > 0)
  {
    sprintf (Error_Message, "unable to malloc VGRID I/O work array %i",
             Error_Flag);
    return (1);
  }

  ug_free (Node_Map);

  // determine the grid type (viscous or inviscid)

  if (Number_of_BL_Vol_Tets > 0)
  {
    mtype = 2;
    Number_of_Viscous_Tets = Number_of_BL_Vol_Tets;

    for (Index = 1; Index <= Number_of_BL_Vol_Tets; Index++)
    {
      for (Geom_Index = 0; Geom_Index <= 3; Geom_Index++)
      {
        if (Vol_Tet_Connectivity[Index][Geom_Index] > Number_of_Viscous_Nodes)
          Number_of_Viscous_Nodes = Vol_Tet_Connectivity[Index][Geom_Index];
      }
    }
  }

  Number_of_Bytes = 6 * ((INT_) (sizeof (INT_)))
                  + 4 * Number_of_Vol_Tets * ((INT_) (sizeof (INT_)))
                  + ((INT_) (sizeof (double)));

  Write_Flag = 0;

  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                       sizeof (INT_), 1, Grid_File);

  Write_Flag = Write_Flag + ug_fwrite (&inew,
                                       sizeof (INT_), 1, Grid_File);
  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Tets,
                                       sizeof (INT_), 1, Grid_File);
  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                       sizeof (INT_), 1, Grid_File);
  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bnd_Nodes,
                                       sizeof (INT_), 1, Grid_File);
  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Viscous_Nodes,
                                       sizeof (INT_), 1, Grid_File);
  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Viscous_Tets,
                                       sizeof (INT_), 1, Grid_File);

  TMP_double = -1.0;
  Write_Flag = Write_Flag + ug_fwrite (&TMP_double,
                                       sizeof (double), 1, Grid_File);

  for (Geom_Index = 0; Geom_Index <= 3; Geom_Index++)
  {
    for (Index = 1; Index <= Number_of_Vol_Tets; Index++)
    {
      Write_Flag = Write_Flag + ug_fwrite (&Vol_Tet_Connectivity[Index][Geom_Index],
                                           sizeof (INT_), 1, Grid_File);
    }
  }

  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                       sizeof (INT_), 1, Grid_File);

  Number_of_Write_Items = 6 + 1 + 4 * Number_of_Vol_Tets + 2;

  if (Write_Flag != Number_of_Write_Items)
  {
    strcpy (Error_Message, "error writing VGRID grid file");
    return (1);
  }

  Number_of_Bytes = 3 * Number_of_Nodes * (sizeof (double));

  Write_Flag = 0;

  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                       sizeof (INT_), 1, Grid_File);

  for (Geom_Index = 0; Geom_Index <= 2; ++Geom_Index)
  {
    for (Index = 1; Index <= Number_of_Nodes; ++Index)
    {
      TMP_double = Coordinates[Index][Geom_Index];
 
      Write_Flag = Write_Flag + ug_fwrite (&TMP_double,
                                           sizeof (double), 1, Grid_File);
    }
  }

  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                       sizeof (INT_), 1, Grid_File);

  Number_of_Write_Items = 3 * Number_of_Nodes + 2;

  if (Write_Flag != Number_of_Write_Items)
  {
    strcpy (Error_Message, "error writing VGRID grid file");
    return (1);
  }

  // determine the number of surface ids and consecutive numbered map
  // this is only needed if the surface ids are node consecutive or if there are
  // quad surface faces

  Number_of_Surf_IDs = 0;

  Error_Flag = ug_renumber_int_values (Number_of_Surf_Trias,
                                       &Number_of_Surf_IDs, &Max_Surf_ID,
                                       &Min_Surf_ID,
                                       Surf_ID_Flag, New_Surf_ID_Flag,
                                       &ID_Map);

  if (Error_Flag > 0)
  {
    sprintf (Error_Message, "unable to malloc VGRID I/O work array %i",
             Error_Flag);
    return (1);
  }

  // write the .bc file (tria connectivity)

  sprintf (BC_File_Name,"%s.bc",Case_Name);

  Error_Flag = ug_backup_file (BC_File_Name);

  BC_File = ug_fopen (BC_File_Name, "w+");

  if (BC_File == NULL)
  {
    strcpy (Error_Message, "unable to open VGRID bc file");
    ug_free (ID_Map);
    return (1);
  }

  Write_Flag = fprintf (BC_File, "%i %i %i %i\n",
                        Number_of_Surf_Trias,
                        Number_of_Bnd_Nodes,
                        Number_of_Surf_IDs,
                        mtype);

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing VGRID grid bc file");
    ug_free (ID_Map);
    ug_fclose (BC_File);
    return (1);
  }

  Write_Flag = fprintf (BC_File, "Triangle    Surface Patch        Nodes\n");

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing VGRID grid bc file");
    ug_free (ID_Map);
    ug_fclose (BC_File);
    return (1);
  }

  for (Index = 1; Index <= Number_of_Surf_Trias; Index++)
  {
    Write_Flag = fprintf (BC_File,"%i %i %i %i %i\n",
                          Index,
                          Surf_ID_Flag[Index],
                          Surf_Tria_Connectivity[Index][0],
                          Surf_Tria_Connectivity[Index][1],
                          Surf_Tria_Connectivity[Index][2]);
  }

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing VGRID grid bc file");
    ug_free (ID_Map);
    ug_fclose (BC_File);
    return (1);
  }

  ug_fclose (BC_File);

  // write the .mapbc file

  sprintf (MapBC_File_Name,"%s.mapbc",Case_Name);

  Error_Flag = ug_backup_file (MapBC_File_Name);

  MapBC_File = ug_fopen (MapBC_File_Name, "w+");

  if (MapBC_File == NULL)
  {
    strcpy (Error_Message, "unable to open VGRID mapbc file");
    ug_free (ID_Map);
    return (1);
  }

  ug_io_version (Compile_Date, Compile_OS, Version_Date, Version_Number);

  Write_Flag = fprintf (MapBC_File, "# Generated by UG_IO: %s %s\n", Version_Number, Version_Date);
  Write_Flag = fprintf (MapBC_File, "# %s\n", Case_Name);
  Write_Flag = fprintf (MapBC_File, "Patch #        BC             Family   #surf   surfIDs         Family\n");
  Write_Flag = fprintf (MapBC_File, "#---------------------------------------------------------------------\n");

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing VGRID grid mapbc file");
    ug_free (ID_Map);
    ug_fclose (MapBC_File);
    return (1);
  }

  for (Index = 1; Index <= Max_Surf_ID; Index++)
  {
    if (ID_Map[Index] == 1)
    {
      Write_Flag = fprintf (MapBC_File, "%i %i\n",Index,0);
    }
  }

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing VGRID grid mapbc file");
    ug_free (ID_Map);
    ug_fclose (MapBC_File);
    return (1);
  }

  ug_free (ID_Map);

  ug_fclose (MapBC_File);

  return (0);

}
