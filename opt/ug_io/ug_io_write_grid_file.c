#include "UG_IO_LIB.h"

INT_ ug_io_write_grid_file
 (char Grid_File_Name[],
  char Error_Message[],
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr,
  INT_ Message_Flag,
  INT_ Number_of_BL_Vol_Tets,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6,
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_1D * Surf_Reconnection_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D * Vol_Hex_Connectivity,
  INT_1D * Vol_ID_Flag,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Write grid data to a standard UG surface or volume grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_grid_file.c,v 1.122 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Case_Name, File_Name, drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;
  CHAR_21 File_Type_Suffix_;
  CHAR_11 File_Compression_Suffix_, File_Format_Suffix_, File_Mode_Suffix_;

  FILE *Grid_File;

  INT_1D *ID_Map = NULL;
  INT_1D *New_Surf_ID_Flag = NULL;

  INT_ Close_Flag, Error_Flag, File_Data_Type_, File_Format_, File_Mode_,
       File_Name_Length, File_Type_Format_Flag_, ID, Max_ID, Min_ID,
       Number_of_IDs, Number_of_Int_Quads, Number_of_Int_Trias,
       Number_of_Surf_Faces, Number_of_Vol_Elems, Number_of_Vol_IDs,
       Renumber_Surf_ID_Flag, Vol_ID_Max, Vol_ID_Min, Version_Flag;

  INT_ Check_File_Flag = 0;
  INT_ Number_of_Bnd_Nodes = 0;

  Version_Flag = UG_IO_Param_Struct_Ptr->Version_Flag;

  Error_Flag = ug_io_file_info (Grid_File_Name, Case_Name, Error_Message,
                                File_Compression_Suffix_, File_Format_Suffix_,
                                File_Mode_Suffix_, File_Type_Suffix_,
                                UG_IO_Param_Struct_Ptr,
                                Check_File_Flag, &File_Data_Type_,
                                &File_Format_, &File_Mode_,
                                &File_Type_Format_Flag_);
  
  if (Error_Flag != 0)
    return (Error_Flag);
  
  if (File_Data_Type_ != UG_IO_SURFACE_GRID &&
      File_Data_Type_ != UG_IO_VOLUME_GRID)
  {
    strcpy (Error_Message, "file is not a 3D grid file");
    return (-625);
  }

  File_Format_ = abs (File_Format_);

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG_IO    : OUTPUT GRID");
    ug_message (" ");
    ug_message ("UG_IO    : Writing Data");
    ug_message (" ");

    ug_cpu_message ("");
  }

  Renumber_Surf_ID_Flag = (Number_of_Surf_Trias < 0 ||
                           Number_of_Surf_Quads < 0 ||
                           strcmp (File_Type_Suffix_, ".mcell") == 0) ? 1: 0;

  Number_of_Surf_Trias = abs (Number_of_Surf_Trias);
  Number_of_Surf_Quads = abs (Number_of_Surf_Quads);

  if (strcmp (File_Type_Suffix_, ".stl") == 0 &&
      File_Format_ != UG_IO_FORMATTED && File_Format_ != UG_IO_BINARY_SINGLE)
  {
    strcpy (Error_Message, "STL surface grid files must use ASCII formatted or binary float file format");
    return (625);
  }

  if (File_Mode_ == UG_FIO_STANDARD_FILE_MODE)
    Error_Flag = ug_backup_file (Grid_File_Name);

  File_Name_Length = (INT_) strlen (Grid_File_Name)
                   - (INT_) strlen (File_Compression_Suffix_);

  strcpy (File_Name, "");
  strncat (File_Name, Grid_File_Name, File_Name_Length);

  if (File_Mode_ == UG_FIO_TMP_FILE_MODE)
    Grid_File = ug_fopen (File_Name, "tmp");
  else
    Grid_File = ug_fopen (File_Name, "w");

  if (Grid_File == NULL)
  {
    strcpy (Error_Message, "unable to open output grid file");
    return (625);
  }

  Number_of_Vol_Elems = Number_of_Vol_Hexs + Number_of_Vol_Pents_5
                      + Number_of_Vol_Pents_6 + Number_of_Vol_Tets;

  if (Vol_ID_Flag == NULL || Number_of_Vol_Elems == 0)
  {
    Vol_ID_Max = 0;
    Vol_ID_Min = 0;
  }
  else
  {
    Vol_ID_Max = ug_max_int (1, Number_of_Vol_Elems, Vol_ID_Flag);
    Vol_ID_Min = ug_min_int (1, Number_of_Vol_Elems, Vol_ID_Flag);

    if (Vol_ID_Max == Vol_ID_Min && Vol_ID_Max <= 0)
    {
      ug_set_int (1, Number_of_Vol_Tets, 0, Vol_ID_Flag);

      Error_Flag = ug3_idiel (Number_of_Surf_Trias, Number_of_Vol_Tets,
                              Number_of_Nodes, &Number_of_Vol_IDs,
                              Vol_ID_Flag, Surf_Tria_Connectivity,
                              Vol_Tet_Connectivity);

      if (Error_Flag > 0)
      {
        sprintf (Error_Message, "unable to allocate required memory %i",
                 Error_Flag);
        return (625);
      }

      Error_Flag = ug3_idielm (Number_of_Vol_Tets,
                               Number_of_Vol_Pents_5,
                               Number_of_Vol_Pents_6,
                               Number_of_Vol_Hexs,
                               Number_of_Nodes,
                               Vol_ID_Flag,
                               Vol_Tet_Connectivity,
                               Vol_Pent_5_Connectivity,
                               Vol_Pent_6_Connectivity,
                               Vol_Hex_Connectivity);

      if (Error_Flag > 0)
      {
        sprintf (Error_Message, "unable to allocate required memory %i",
                 Error_Flag);
        return (625);
      }

      Vol_ID_Max = Number_of_Vol_IDs;
      Vol_ID_Min = 1;
    }
  }

  if (Renumber_Surf_ID_Flag)
  {
    Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;

    Error_Flag = 0;

    New_Surf_ID_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                             (Number_of_Surf_Faces+1)
                                           * (INT_) sizeof (INT_1D));

    if (Error_Flag > 0)
    {
      strcpy (Error_Message, "unable to malloc I/O work array");
      return (1);
    }

    Number_of_IDs = 0;

    // reorder based on "first encountered" for NSU3D (.mcell)
    Number_of_IDs = (strcmp (File_Type_Suffix_, ".mcell") == 0) ? -1: 0;

    Error_Flag = ug_renumber_int_values (Number_of_Surf_Faces,
			                 &Number_of_IDs, &Max_ID, &Min_ID,
			                 Surf_ID_Flag, New_Surf_ID_Flag,
                                         &ID_Map);

    if (Error_Flag > 0)
    {
      strcpy (Error_Message, "unable to malloc I/O work array");
      ug_free (New_Surf_ID_Flag);
      ug_free (ID_Map);
      return (1);
    }
  }
  else
    New_Surf_ID_Flag = Surf_ID_Flag;

  Close_Flag = 1;

  if (strcmp (File_Type_Suffix_, ".cfd++") == 0)
  {
    Close_Flag = ug_fclose (Grid_File);

    Error_Flag = ug_io_write_cfdpp (Error_Message,
                                    Number_of_Nodes,
                                    Number_of_Surf_Quads,
                                    Number_of_Surf_Trias,
                                    Number_of_Vol_Hexs,
                                    Number_of_Vol_Pents_5,
                                    Number_of_Vol_Pents_6,
                                    Number_of_Vol_Tets,
                                    New_Surf_ID_Flag, 
                                    Surf_Quad_Connectivity,
                                    Surf_Tria_Connectivity,
                                    Vol_Hex_Connectivity,
                                    Vol_Pent_5_Connectivity,
                                    Vol_Pent_6_Connectivity,
                                    Vol_Tet_Connectivity,
                                    Coordinates);

    Close_Flag = 0;
  }

  else if (strcmp (File_Type_Suffix_, ".cobalt.grd") == 0)
    Error_Flag = ug_io_write_cobalt (Grid_File, Error_Message,
                                     Number_of_Nodes,
                                     Number_of_Surf_Quads,
                                     Number_of_Surf_Trias,
                                     Number_of_Vol_Hexs,
                                     Number_of_Vol_Pents_5,
                                     Number_of_Vol_Pents_6,
                                     Number_of_Vol_Tets,
                                     New_Surf_ID_Flag, 
                                     Surf_Quad_Connectivity,
                                     Surf_Tria_Connectivity,
                                     Vol_Hex_Connectivity,
                                     Vol_Pent_5_Connectivity,
                                     Vol_Pent_6_Connectivity,
                                     Vol_Tet_Connectivity,
                                     Coordinates);

  else if (strcmp (File_Type_Suffix_, ".egrid") == 0)
    Error_Flag = ug_io_write_egrid (Grid_File, Case_Name, Error_Message,
                                    Number_of_Nodes,
                                    Number_of_Surf_Quads,
                                    Number_of_Surf_Trias,
                                    Number_of_Vol_Hexs,
                                    Number_of_Vol_Pents_5,
                                    Number_of_Vol_Pents_6,
                                    Number_of_Vol_Tets,
                                    New_Surf_ID_Flag, 
                                    Surf_Quad_Connectivity,
                                    Surf_Tria_Connectivity,
                                    Vol_Hex_Connectivity,
                                    Vol_Pent_5_Connectivity,
                                    Vol_Pent_6_Connectivity,
                                    Vol_Tet_Connectivity,
                                    Coordinates);

  else if (strcmp (File_Type_Suffix_, ".fluent.cas") == 0)
    Error_Flag = ug_io_write_fluent (Grid_File, Case_Name, Error_Message,
                                     Number_of_Nodes,
                                     Number_of_Surf_Quads,
                                     Number_of_Surf_Trias,
                                     Number_of_Vol_Hexs,
                                     Number_of_Vol_Pents_5,
                                     Number_of_Vol_Pents_6,
                                     Number_of_Vol_Tets,
                                     Surf_Grid_BC_Flag, New_Surf_ID_Flag, 
                                     Surf_Quad_Connectivity,
                                     Surf_Tria_Connectivity,
                                     Vol_Hex_Connectivity,
                                     Vol_Pent_5_Connectivity,
                                     Vol_Pent_6_Connectivity,
                                     Vol_Tet_Connectivity,
                                     Coordinates);

  else if (strcmp (File_Type_Suffix_, ".fgrid") == 0)
  {
    Number_of_Surf_Quads = 0;
    Number_of_Vol_Hexs = 0;
    Number_of_Vol_Pents_5 = 0;
    Number_of_Vol_Pents_6 = 0;

    Error_Flag = ug_io_write_fgrid (Grid_File, Error_Message, File_Format_,
                                    Number_of_Nodes, Number_of_Surf_Trias,
                                    Number_of_Vol_Tets,
                                    New_Surf_ID_Flag, Surf_Tria_Connectivity,
                                    Vol_Tet_Connectivity, Coordinates);

    if (Error_Flag == 0)
      Error_Flag = ug_io_write_gdata (Grid_File, Error_Message, File_Format_,
                                      Number_of_BL_Vol_Tets,
                                      Number_of_Nodes,
                                      Number_of_Surf_Quads,
                                      Number_of_Surf_Trias,
                                      Number_of_Vol_Hexs, Number_of_Vol_Pents_5,
                                      Number_of_Vol_Pents_6, Number_of_Vol_Tets,
                                      Surf_Grid_BC_Flag, Surf_Reconnection_Flag,
                                      Vol_ID_Flag,
                                      Initial_Normal_Spacing, BL_Thickness);
  }

  else if (strcmp (File_Type_Suffix_, ".fvuns") == 0)
    Error_Flag = ug_io_write_fvuns (Grid_File, Error_Message, File_Format_,
                                    Number_of_Nodes,
                                    Number_of_Surf_Quads, Number_of_Surf_Trias,
                                    Number_of_Vol_Hexs,
                                    Number_of_Vol_Pents_5,
                                    Number_of_Vol_Pents_6,
                                    Number_of_Vol_Tets,
                                    New_Surf_ID_Flag,
                                    Surf_Quad_Connectivity, 
                                    Surf_Tria_Connectivity,
                                    Vol_Hex_Connectivity,
                                    Vol_Pent_5_Connectivity,
                                    Vol_Pent_6_Connectivity,
                                    Vol_Tet_Connectivity,
                                    Coordinates);

  else if (strcmp (File_Type_Suffix_, ".gambit") == 0)
    Error_Flag = ug_io_write_gambit (Grid_File, Error_Message,
                                     Number_of_Nodes,
                                     Number_of_Surf_Quads, Number_of_Surf_Trias,
                                     Number_of_Vol_Hexs,
                                     Number_of_Vol_Pents_5,
                                     Number_of_Vol_Pents_6,
                                     Number_of_Vol_Tets,
                                     New_Surf_ID_Flag, 
                                     Surf_Quad_Connectivity,
                                     Surf_Tria_Connectivity,
                                     Vol_Hex_Connectivity,
                                     Vol_Pent_5_Connectivity,
                                     Vol_Pent_6_Connectivity,
                                     Vol_Tet_Connectivity,
                                     Coordinates);

  else if (strcmp (File_Type_Suffix_, ".mesh") == 0 ||
           strcmp (File_Type_Suffix_, ".meshb") == 0)
  {
    Close_Flag = ug_fclose (Grid_File);

    Error_Flag = ug_io_write_mesh_grid (File_Name, Error_Message,
                                        Number_of_BL_Vol_Tets,
                                        Number_of_Nodes,
                                        Number_of_Surf_Quads,
                                        Number_of_Surf_Trias,
                                        Number_of_Vol_Hexs,
                                        Number_of_Vol_Pents_5,
                                        Number_of_Vol_Pents_6,
                                        Number_of_Vol_Tets,
                                        New_Surf_ID_Flag, 
                                        Surf_Quad_Connectivity,
                                        Surf_Tria_Connectivity,
                                        Vol_Hex_Connectivity,
                                        Vol_ID_Flag, 
                                        Vol_Pent_5_Connectivity,
                                        Vol_Pent_6_Connectivity,
                                        Vol_Tet_Connectivity,
                                        Coordinates);

    Close_Flag = 0;
  }

  else if (strcmp (File_Type_Suffix_, ".bdf") == 0 || strcmp (File_Type_Suffix_, ".nas") == 0)
    Error_Flag = ug_io_write_nas (Grid_File, Error_Message,
                                  Number_of_BL_Vol_Tets,
                                  Number_of_Nodes,
                                  Number_of_Surf_Quads, Number_of_Surf_Trias,
                                  Number_of_Vol_Hexs,
                                  Number_of_Vol_Pents_5,
                                  Number_of_Vol_Pents_6,
                                  Number_of_Vol_Tets,
                                  Surf_Grid_BC_Flag, New_Surf_ID_Flag, 
                                  Surf_Quad_Connectivity,
                                  Surf_Reconnection_Flag,
                                  Surf_Tria_Connectivity,
                                  Vol_Hex_Connectivity,
                                  Vol_ID_Flag, 
                                  Vol_Pent_5_Connectivity,
                                  Vol_Pent_6_Connectivity,
                                  Vol_Tet_Connectivity,
                                  Coordinates,
                                  Initial_Normal_Spacing,
                                  BL_Thickness);

  else if (strcmp (File_Type_Suffix_, ".mcell") == 0)
    Error_Flag = ug_io_write_nsu3d (Grid_File, Error_Message, File_Format_,
                                    Number_of_Nodes,
                                    Number_of_Surf_Quads, Number_of_Surf_Trias,
                                    Number_of_Vol_Hexs,
                                    Number_of_Vol_Pents_5,
                                    Number_of_Vol_Pents_6,
                                    Number_of_Vol_Tets,
                                    New_Surf_ID_Flag, 
                                    Surf_Quad_Connectivity,
                                    Surf_Tria_Connectivity,
                                    Vol_Hex_Connectivity,
                                    Vol_Pent_5_Connectivity,
                                    Vol_Pent_6_Connectivity,
                                    Vol_Tet_Connectivity,
                                    Coordinates);

  else if(strcmp (File_Type_Suffix_, ".foam") == 0)
  {
    Close_Flag = ug_fclose (Grid_File);

    Error_Flag = ug_io_write_OpenFOAM
      (Error_Message,
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

    Close_Flag = 0;
  }


  else if (strcmp (File_Type_Suffix_, ".poly") == 0)
  {
    Number_of_BL_Vol_Tets = 0;
    Number_of_Vol_Hexs = 0;
    Number_of_Vol_Pents_5 = 0;
    Number_of_Vol_Pents_6 = 0;
    Number_of_Vol_Tets = 0;

    Error_Flag = ug_io_write_poly (Grid_File, Error_Message,
                                   &Number_of_Bnd_Nodes, Number_of_Nodes,
                                   Number_of_Surf_Quads, Number_of_Surf_Trias,
                                   New_Surf_ID_Flag, 
                                   Surf_Quad_Connectivity,
                                   Surf_Tria_Connectivity,
                                   Coordinates);
  }

  else if (strcmp (File_Type_Suffix_, ".stl") == 0)
  {
    Number_of_BL_Vol_Tets = 0;
    Number_of_Vol_Hexs = 0;
    Number_of_Vol_Pents_5 = 0;
    Number_of_Vol_Pents_6 = 0;
    Number_of_Vol_Tets = 0;

    Error_Flag = ug_io_write_stl (Grid_File, Error_Message, File_Format_,
                                  &Number_of_Bnd_Nodes, Number_of_Nodes,
                                  Number_of_Surf_Quads, Number_of_Surf_Trias,
                                  New_Surf_ID_Flag,
                                  Surf_Quad_Connectivity, 
                                  Surf_Tria_Connectivity,
                                  Coordinates);
  } 

  else if (strcmp (File_Type_Suffix_, ".surf") == 0)
  {
    Number_of_BL_Vol_Tets = 0;
    Number_of_Vol_Hexs = 0;
    Number_of_Vol_Pents_5 = 0;
    Number_of_Vol_Pents_6 = 0;
    Number_of_Vol_Tets = 0;

    Error_Flag = ug_io_write_surf (Grid_File, Error_Message,
                                   &Number_of_Bnd_Nodes, Number_of_Nodes,
                                   Number_of_Surf_Quads, Number_of_Surf_Trias,
                                   Version_Flag,
                                   Surf_Grid_BC_Flag, New_Surf_ID_Flag,
                                   Surf_Quad_Connectivity, 
                                   Surf_Reconnection_Flag,
                                   Surf_Tria_Connectivity,
                                   Coordinates,
                                   Initial_Normal_Spacing, BL_Thickness);
  } 

  else if (strcmp (File_Type_Suffix_, ".tri") == 0)
  {
    Number_of_BL_Vol_Tets = 0;
    Number_of_Surf_Quads = 0;
    Number_of_Vol_Hexs = 0;
    Number_of_Vol_Pents_5 = 0;
    Number_of_Vol_Pents_6 = 0;
    Number_of_Vol_Tets = 0;

    Error_Flag = ug_io_write_cart3d (Grid_File, Error_Message, File_Format_,
                                     &Number_of_Bnd_Nodes, Number_of_Nodes,
                                     Number_of_Surf_Trias,
                                     New_Surf_ID_Flag,
                                     Surf_Tria_Connectivity,
                                     Coordinates);
  } 

  else if (strcmp (File_Type_Suffix_, ".ugrid") == 0)
  {
    Error_Flag = ug_io_write_ugrid (Grid_File, Error_Message, File_Format_,
                                    Number_of_Nodes,
                                    Number_of_Surf_Quads, Number_of_Surf_Trias,
                                    Number_of_Vol_Hexs,
                                    Number_of_Vol_Pents_5,
                                    Number_of_Vol_Pents_6,
                                    Number_of_Vol_Tets,
                                    New_Surf_ID_Flag, Surf_Quad_Connectivity, 
                                    Surf_Tria_Connectivity,
                                    Vol_Hex_Connectivity,
                                    Vol_Pent_5_Connectivity,
                                    Vol_Pent_6_Connectivity,
                                    Vol_Tet_Connectivity,
                                    Coordinates);

    if (Error_Flag == 0)
      Error_Flag = ug_io_write_gdata (Grid_File, Error_Message, File_Format_,
                                      Number_of_BL_Vol_Tets,
                                      Number_of_Nodes,
                                      Number_of_Surf_Quads,
                                      Number_of_Surf_Trias,
                                      Number_of_Vol_Hexs, Number_of_Vol_Pents_5,
                                      Number_of_Vol_Pents_6, Number_of_Vol_Tets,
                                      Surf_Grid_BC_Flag, Surf_Reconnection_Flag,
                                      Vol_ID_Flag,
                                      Initial_Normal_Spacing, BL_Thickness);
  }

  else if (strcmp (File_Type_Suffix_, ".cogsg") == 0)
  {
    Number_of_Surf_Quads = 0;
    Number_of_Vol_Hexs = 0;
    Number_of_Vol_Pents_5 = 0;
    Number_of_Vol_Pents_6 = 0;

    Error_Flag = ug_io_write_vgrid (Grid_File, Case_Name, Error_Message,
                                    Number_of_Nodes,
                                    Number_of_Surf_Trias,
                                    Number_of_Vol_Tets,
                                    Number_of_BL_Vol_Tets,
                                    New_Surf_ID_Flag,
                                    Surf_Tria_Connectivity,
                                    Vol_Tet_Connectivity,
                                    Coordinates);
  }

  else if (strcmp (File_Type_Suffix_, ".cgns") == 0)
  {
    Close_Flag = ug_fclose (Grid_File);

    Error_Flag = ug_io_write_cgns_grid (Case_Name, File_Name, Error_Message,
                                        Number_of_BL_Vol_Tets,
                                        Number_of_Nodes,
                                        Number_of_Surf_Quads,
                                        Number_of_Surf_Trias,
                                        Number_of_Vol_Hexs,
                                        Number_of_Vol_Pents_5,
                                        Number_of_Vol_Pents_6,
                                        Number_of_Vol_Tets,
                                        Surf_Grid_BC_Flag, New_Surf_ID_Flag, 
                                        Surf_Quad_Connectivity,
                                        Surf_Reconnection_Flag,
                                        Surf_Tria_Connectivity,
                                        Vol_Hex_Connectivity,
                                        Vol_ID_Flag, 
                                        Vol_Pent_5_Connectivity,
                                        Vol_Pent_6_Connectivity,
                                        Vol_Tet_Connectivity,
                                        Coordinates,
                                        Initial_Normal_Spacing, BL_Thickness);

    Close_Flag = 0;
  } 

  else
  { 
    Error_Flag = 1;
    strcpy (Error_Message, "unknown output grid file type");
  }

  if (Close_Flag == 1)
    Close_Flag = ug_fclose (Grid_File);

  if (Error_Flag > 0)
  {
    if (Renumber_Surf_ID_Flag)
    {
      ug_free (New_Surf_ID_Flag);
      ug_free (ID_Map);
    }
    return (625);
  }

  if (Message_Flag >= 1)
  {
    Number_of_Vol_Elems = Number_of_Vol_Hexs + Number_of_Vol_Pents_5
                        + Number_of_Vol_Pents_6 + Number_of_Vol_Tets;

    Number_of_Int_Trias = (Number_of_Vol_Tets*4 + Number_of_Vol_Pents_5*4
                         + Number_of_Vol_Pents_6*2 - Number_of_Surf_Trias) / 2;
    Number_of_Int_Quads = (Number_of_Vol_Pents_5 + Number_of_Vol_Pents_6*3
                         + Number_of_Vol_Hexs*6 - Number_of_Surf_Quads) / 2;

    Number_of_Int_Trias = MAX (Number_of_Int_Trias, 0);
    Number_of_Int_Quads = MAX (Number_of_Int_Quads, 0);

    ug_splitpath (Grid_File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "UG_IO    : Grid File Name    = %s", Name_Text);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Quad Surface Faces=%10i", Number_of_Surf_Quads);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Tria Surface Faces=%10i", Number_of_Surf_Trias);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Quad Int. Faces   =%10i", Number_of_Int_Quads);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Tria Int. Faces   =%10i", Number_of_Int_Trias);
    ug_message (Text);
    if (Number_of_Bnd_Nodes > 0)
      sprintf (Text, "UG_IO    : Nodes             =%10i", Number_of_Bnd_Nodes);
    else
      sprintf (Text, "UG_IO    : Nodes             =%10i", Number_of_Nodes);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Total Elements    =%10i", Number_of_Vol_Elems);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Hex Elements      =%10i", Number_of_Vol_Hexs);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Pent_5 Elements   =%10i", Number_of_Vol_Pents_5);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Pent_6 Elements   =%10i", Number_of_Vol_Pents_6);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Tet Elements      =%10i", Number_of_Vol_Tets);
    ug_message (Text);

    if (Number_of_Vol_Elems > 0)
    {
      sprintf (Text, "UG_IO    : BL Tet Elements   =%10i", Number_of_BL_Vol_Tets);
      ug_message (Text);
      sprintf (Text, "UG_IO    : Volume ID Min/Max =%10i%10i", Vol_ID_Min,
                                                             Vol_ID_Max);
      ug_message (Text);
    }

    if (Renumber_Surf_ID_Flag)
    {
      for (ID = Min_ID; ID <= Max_ID; ++ID)
      {
        if (ID_Map[ID-Min_ID])
        {
          sprintf (Text, "UG_IO    : Inl,Final Surf ID =%10i%10i",
                   ID, ID_Map[ID-Min_ID]);
          ug_message (Text);
        }
      }
    }

    ug_message (" ");
    ug_cpu_message ("UG_IO    :");
  }

  if (Renumber_Surf_ID_Flag)
  {
    ug_free (New_Surf_ID_Flag);
    ug_free (ID_Map);
  }

  Error_Flag = ug_compress_file (1, File_Compression_Suffix_, File_Name);

  return (0);

}
