#include "UG3_LIB.h"

void ug3_surf_error_message
 (char Case_Name[],
  INT_ Error_Flag,
  INT_ *Known_Error,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Trias,
  INT_1D * Surf_Error_Flag,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_1D * Surf_Reconnection_Flag,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Write a UG3 boundary surface grid error message and grid file.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_surf_error_message.c,v 1.34 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX File_Name;

  CHAR_133 Text;

  if (*Known_Error == 1)
    return;
  
  if (Error_Flag >= 200300 && Error_Flag <= 200399)
    *Known_Error = -1;

  if (Error_Flag == 200300)
  {
    ug_error_message ("*** dihedral angle between adjacent boundary faces too small ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200301)
  {
    ug_error_message ("*** boundary face connectivity is wrong ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200302)
  {
    ug_error_message ("*** face angle is too large for at least one of the boundary surface faces ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200303)
  {
    ug_error_message ("*** boundary faces not ordered correctly ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200304)
  {
    ug_error_message ("*** surface that intersects BL found with a large included angle ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200305)
  {
    ug_error_message ("*** fixed surface that intersects BL found adjacent to an invalid grid BC ***");
    ug_error_message ("*** neighbors of fixed surfaces may only be another fixed surface that ***");
    ug_error_message ("*** intersects the BL region, a standard solid BL generating surface, or a ***");
    ug_error_message ("*** standard solid surface ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200306)
  {
    ug_error_message ("*** boundary recovery failed finding boundary elements ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200307)
  {
    ug_error_message ("*** surface grid BC flags are not all set to valid values ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200308 || Error_Flag == 200317)
  {
    if (Error_Flag == 200317)
      ug_error_message ("*** boundary surface for external object is not fully connected ***");
    ug_error_message ("*** boundary faces found without neighbor(s) ***");
    ug_error_message ("*** there may be extra boundary faces unconnected ***");
    ug_error_message ("*** or partially connected to surface ***");
    ug_error_message ("*** there may be a hole in the boundary surface ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200309)
  {
    ug_error_message ("*** more than two boundary faces share an edge of simply connected surface ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200310)
  {
    ug_error_message ("*** multiply-connected boundary faces must be ordered consistently ***");
    ug_error_message ("*** there may be extra boundary faces connected to surface ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200311)
  {
    ug_error_message ("*** unable to reorder boundary faces ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200312)
  {
    ug_error_message ("*** found nodes that are not included in boundary surface connectivity ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200313)
  {
    ug_error_message ("*** duplicate boundary faces found ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200314)
  {
    ug_error_message ("*** boundary face found with a node that is not specified ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200315)
  {
    ug_error_message ("*** unable to reorder boundary faces ***");
    ug_error_message ("*** multiply-connected boundary faces must be ordered consistently ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200316)
  {
    ug_error_message ("*** fixed surface that intersects BL found without a valid adjacent grid BC ***");
    ug_error_message ("*** fixed surfaces that intersect the BL region must be adjacent to at ***");
    ug_error_message ("*** least one face of a surface that is a BL generating surface ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200318)
  {
    ug_error_message ("*** external boundary object has transparent/embedded faces ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200319)
  {
    ug_error_message ("*** boundary surface face found that contains duplicate nodes ***");
    ug_error_message ("*** faces attached to the duplicate nodes are identified ***");

    *Known_Error = 1;
  }

  else if (Error_Flag == 200320)
  {
    ug_error_message ("*** closed surface found that is a tansparent/embedded surface and a BL ***");
    ug_error_message ("*** generating surface ***");

    *Known_Error = 1;
  }

  if (*Known_Error == -1)
  {
    sprintf (Text, "*** UNKNOWN FATAL SURFACE GRID ERROR (%i) ***", Error_Flag);
    ug_error_message (Text);
  }

  else if (*Known_Error == 1)
    ug3_write_surf_grid_error_file (Case_Name, File_Name,
                                    Number_of_Nodes, Number_of_Surf_Trias,
                                    Surf_Error_Flag,
                                    Surf_Grid_BC_Flag, Surf_ID_Flag,
                                    Surf_Reconnection_Flag,
                                    Surf_Tria_Connectivity,
                                    Coordinates);

  if (*Known_Error != 0)
    *Known_Error = 1;

  return;

}
