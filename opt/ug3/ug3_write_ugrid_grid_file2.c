#include "UG3_LIB.h"

INT_ ug3_write_ugrid_grid_file2
 (char File_Name[],
  INT_ Message_Flag,
  INT_ File_Format_Flag,
  INT_ Write_Task_Flag,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Incrementally write grid data (tet only) to a UGRID volume grid file.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_write_ugrid_grid_file2.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_4D *Surf_Quad_Connectivity = NULL;
  INT_8D *Vol_Hex_Connectivity = NULL;
  INT_5D *Vol_Pent_5_Connectivity = NULL;
  INT_6D *Vol_Pent_6_Connectivity = NULL;

  INT_ Number_of_BL_Vol_Tets = 0;
  INT_ Number_of_Surf_Quads = 0;
  INT_ Number_of_Vol_Hexs = 0;
  INT_ Number_of_Vol_Pents_5 = 0;
  INT_ Number_of_Vol_Pents_6 = 0;
  INT_ Error_Flag;

  if (Write_Task_Flag == -5) Write_Task_Flag = -9; 

  if (ug_file_status_monitor_flag())
    ug_message ("ug3_write_ugrid_grid_file2");

  Error_Flag = ug3_write_ugrid_grid_file (File_Name, Message_Flag,
                                          File_Format_Flag, Write_Task_Flag,
                                          Number_of_BL_Vol_Tets,
                                          Number_of_Nodes,
                                          Number_of_Surf_Quads,
                                          Number_of_Surf_Trias,
                                          Number_of_Vol_Hexs,
                                          Number_of_Vol_Pents_5,
                                          Number_of_Vol_Pents_6,
                                          Number_of_Vol_Tets,
                                          Surf_ID_Flag,
                                          Surf_Quad_Connectivity,
                                          Surf_Tria_Connectivity,
                                          Vol_Hex_Connectivity,
                                          Vol_Pent_5_Connectivity,
                                          Vol_Pent_6_Connectivity,
                                          Vol_Tet_Connectivity,
                                          Coordinates);

  return (Error_Flag);

}
