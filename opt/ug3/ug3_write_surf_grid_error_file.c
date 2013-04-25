#include "UG3_LIB.h"

void ug3_write_surf_grid_error_file
 (char Case_Name[],
  char File_Name[],
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
 * Write a UG3 boundary surface grid file with faces in error identified or a
 * surface grid node error file with nodes in error listed.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_write_surf_grid_error_file.c,v 1.20 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Error_File;

  CHAR_UG_MAX drive, dname, fname, ext;

  CHAR_31 Name_Text;
  CHAR_133 Text;

  INT_1D *Surf_ID_Error_Flag=NULL;
  INT_4D *Surf_Quad_Connectivity=NULL;

  INT_ Error_Flag, Index, Max_Entries, Node_Index, Write_Flag;
  INT_ Number_of_Surf_Quads = 0;

  if (Surf_Error_Flag[1] <= 0)
  {
    strcpy (File_Name, Case_Name);

    if (Surf_Error_Flag[1] >= -1)
    {
      Write_Flag = 0;

      for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
      {
        if (Surf_Error_Flag[Index] < 0)
        {
          Write_Flag = 1;

          break;
        }
      }

      if (Write_Flag == 1)
      {
        strcat (File_Name, ".FAIL");

        ug_splitpath (File_Name, drive, dname, fname, ext);

        strcat (fname, ext);
        strcpy (Name_Text, "");
        strncat (Name_Text, fname, 30);

        sprintf (Text, "*** see SURF surface grid file %s.surf ***", Name_Text);
        ug_error_message (Text);
        ug_error_message ("*** faces that may be causing an error have an ID of -1 ***");
        ug_error_message ("*** or are nearby those with an ID of -1 ***");
      }
      else
        return;
    }
    else
    {
      strcat (File_Name, ".FAIL");

      Surf_Error_Flag[1] = Surf_Error_Flag[1]+2;
    }

    Error_Flag = 0;

    Surf_ID_Error_Flag = (INT_1D *) ug_malloc (&Error_Flag, 
                                               (Number_of_Surf_Trias+1)
                                             * sizeof (INT_1D));

    if (Error_Flag > 0)
    {
      ug_error_message ("*** unable to allocate space for SURF surface grid file work array ***");
      return;
    }

    for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
    {
      if (Surf_Error_Flag[Index] < 0)
        Surf_ID_Error_Flag[Index] = -1;
      else
        Surf_ID_Error_Flag[Index] = Surf_ID_Flag[Index];
    }

    Error_Flag = ug3_write_surf_grid_file (File_Name,
                                           0,
                                           Number_of_Nodes,
                                           Number_of_Surf_Quads,
                                           Number_of_Surf_Trias,
                                           Surf_Grid_BC_Flag,
                                           Surf_ID_Error_Flag,
                                           Surf_Quad_Connectivity,
                                           Surf_Reconnection_Flag,
                                           Surf_Tria_Connectivity,
                                           Coordinates);

    ug_free (Surf_ID_Error_Flag);

    if (Error_Flag > 0)
    {
      if (Error_Flag >= 100300)
        ug_error_message ("*** unable to allocate space for SURF surface grid file work array ***");
      else
        ug_error_message ("*** error writing SURF surface grid file ***");
      return;
    }
  }

  else
  {
    strcpy (File_Name, Case_Name);
    strcat (File_Name, ".FAIL.node");

    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 30);

    sprintf (Text, "*** see file %s ***", Name_Text);
    ug_error_message (Text);
    ug_error_message ("*** this file contains a list of nodes which are not ***");
    ug_error_message ("*** included in the surface triangulation ***");
    ug_error_message ("*** each line of the file contains the node index ***");
    ug_error_message ("*** followed by the coordinates ***");

    Error_Flag = ug_backup_file (File_Name);

    Error_File = ug_fopen (File_Name, "w");

    if (Error_File == NULL)
    {
      sprintf (Text, "*** unable to open and write file %s ***", Name_Text);
      ug_error_message (Text);
      return;
    }

    Max_Entries = MIN (Number_of_Nodes, Number_of_Surf_Trias);

    for (Index = 1; Index <= Max_Entries; ++Index) 
    {
      Node_Index = Surf_Error_Flag[Index];

      if (Node_Index > 0)
        Write_Flag = fprintf (Error_File, "%i %g %g %g\n",
                              Node_Index, Coordinates[Node_Index][0],
                                          Coordinates[Node_Index][1],
                                          Coordinates[Node_Index][2]);
    }

    if (Write_Flag < 0)
    {
      ug_splitpath (File_Name, drive, dname, fname, ext);

      strcat (fname, ext);
      strcpy (Name_Text, "");
      strncat (Name_Text, fname, 30);

      sprintf (Text, "*** unable to write file %s ***", Name_Text);
      ug_error_message (Text);
      ug_fclose (Error_File);
      return;
    }

    ug_fclose (Error_File);
  }

  return;

}
