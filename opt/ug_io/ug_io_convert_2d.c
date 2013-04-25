#include "UG_IO_LIB.h"

INT_ ug_io_convert_2d
 (char Input_File_Name[],
  char Output_File_Name[],
  INT_ Task_Flag)

{
  
/* 
 * Convert UG_IO 2D grid or node files.
 *
 * If Task_Flag = 1 then output summary of parameter information.
 * If Task_Flag = 2 then output full parameter information.
 * If Task_Flag = 3 then convert a 2D grid or node file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_convert_2d.c,v 1.8 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

/*
 * -----------------------------------------------------------------------------
 * Declare routine variables.
 * -----------------------------------------------------------------------------
 */

  CHAR_133 Error_Message;

  INT_ Data_Type_Flag, Input_File_Data_Type, Output_File_Data_Type;

  INT_ Grid_Data_Type_Flag = 1;
  INT_ Node_Data_Type_Flag = 2;
  INT_ Error_Flag = 0;
  INT_ Known_Error = 0;
  INT_ Message_Flag = 1;

/*
 * -----------------------------------------------------------------------------
 * Declare grid data I/O variables.
 * -----------------------------------------------------------------------------
 */

  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr;

  INT_1D *Bnd_Edge_Grid_BC_Flag, *Bnd_Edge_ID_Flag;

  INT_2D  *Bnd_Edge_Connectivity;

  INT_3D *Tria_Connectivity;

  INT_4D *Quad_Connectivity;

  DOUBLE_1D *Initial_Normal_Spacing, *BL_Thickness;

  DOUBLE_2D *Coordinates;

  INT_ Number_of_Bnd_Edges, Number_of_Nodes, Number_of_Quads, Number_of_Trias;

  INT_ M_BL_Thickness = 1;
  INT_ M_Initial_Normal_Spacing = 1;
  INT_ M_Bnd_Edge_Grid_BC_Flag = 1;
  INT_ M_Bnd_Edge_ID_Flag = 1;

/*
 * -----------------------------------------------------------------------------
 * Declare node data I/O variables.
 * -----------------------------------------------------------------------------
 */

  DOUBLE_1D *Spacing;
  DOUBLE_3D *Vector;

  INT_ M_Spacing = 1;
  INT_ M_Vector = 1;

/*
 * -----------------------------------------------------------------------------
 * Malloc I/O parameter structure arrays.
 * -----------------------------------------------------------------------------
 */

  Error_Flag = ug_io_malloc_param (Error_Message, &UG_IO_Param_Struct_Ptr);

  if (Error_Flag > 0)
  { 
    ug_io_free_param (UG_IO_Param_Struct_Ptr);
    ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
    return (Error_Flag);
  }

/*
 * -----------------------------------------------------------------------------
 * Set default values for I/O grid data file parameters.
 * -----------------------------------------------------------------------------
 */

  Error_Flag = ug_io_initialize_param (Error_Message, 1, 0, 0, 0, 1, 1,
                                       UG_IO_Param_Struct_Ptr);

  if (Error_Flag > 0)
  { 
    ug_io_free_param (UG_IO_Param_Struct_Ptr);
    ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
    return (Error_Flag);
  }

/*
 * -----------------------------------------------------------------------------
 * Write I/O parameter information.
 * -----------------------------------------------------------------------------
 */

  if (Task_Flag == 1 || Task_Flag == 2)
  {
    ug_io_write_param_info (Task_Flag, UG_IO_Param_Struct_Ptr);

    ug_io_free_param (UG_IO_Param_Struct_Ptr);

    return (0);
  }

/*
 * -----------------------------------------------------------------------------
 * Check to see if the strings Input_File_Name and Output_File_Name contains
 * grid or node file names.
 * -----------------------------------------------------------------------------
 */

  else
  {
    Error_Flag = ug_io_file_type (Input_File_Name, Error_Message,
                                  UG_IO_Param_Struct_Ptr,
                                  &Input_File_Data_Type);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

    Error_Flag = ug_io_file_type (Output_File_Name, Error_Message,
                                  UG_IO_Param_Struct_Ptr,
                                  &Output_File_Data_Type);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

    if (Input_File_Data_Type == UG_IO_2D_EDGE_GRID ||
        Input_File_Data_Type == UG_IO_2D_GRID)
    {
      Data_Type_Flag = Grid_Data_Type_Flag;

      if (Input_File_Data_Type != UG_IO_2D_EDGE_GRID &&
          Input_File_Data_Type != UG_IO_2D_GRID)
      {
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        Error_Flag = 699;
        strcpy (Error_Message,
                "if input is a grid file then output must be also");
        ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
        return (Error_Flag);
      }
    }

    else if (Output_File_Data_Type == UG_IO_2D_NODE_DATA)
    {
      Data_Type_Flag = Node_Data_Type_Flag;

      if (Output_File_Data_Type != UG_IO_2D_NODE_DATA)
      {
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        Error_Flag = 699;
        strcpy (Error_Message,
                "if input is a node file then output must be also");
        ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
        return (Error_Flag);
      }
    }

    else
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      Error_Flag = 699;
      strcpy (Error_Message, "file is not a 2d grid or node data type");
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }
  }

/*
 * -----------------------------------------------------------------------------
 * Start of section to read and write a grid data file.
 * -----------------------------------------------------------------------------
 */

  if (Data_Type_Flag == Grid_Data_Type_Flag)
  {

/*
 * -----------------------------------------------------------------------------
 * Initialize grid I/O arrays.
 * -----------------------------------------------------------------------------
 */

    Bnd_Edge_Grid_BC_Flag = NULL;
    Bnd_Edge_ID_Flag = NULL;
    Bnd_Edge_Connectivity = NULL;
    Quad_Connectivity = NULL;
    Tria_Connectivity = NULL;
    Coordinates = NULL;
    Initial_Normal_Spacing = NULL;
    BL_Thickness = NULL;

/*
 * -----------------------------------------------------------------------------
 * Determine grid I/O variables and read grid dimensions.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_2d_grid_file (Input_File_Name,
                                          Error_Message,
                                          UG_IO_Param_Struct_Ptr,
                                          Message_Flag,
                                          &Number_of_Bnd_Edges,
                                          &Number_of_Nodes,
                                          &Number_of_Quads,
                                          &Number_of_Trias,
                                          Bnd_Edge_Connectivity,
                                          Bnd_Edge_Grid_BC_Flag,
                                          Bnd_Edge_ID_Flag,
                                          Quad_Connectivity,
                                          Tria_Connectivity,
                                          Coordinates,
                                          Initial_Normal_Spacing,
                                          BL_Thickness);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Malloc grid data arrays.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_malloc_2d_grid (Error_Message,
                                       M_BL_Thickness,
                                       M_Initial_Normal_Spacing,
                                       M_Bnd_Edge_Grid_BC_Flag,
                                       M_Bnd_Edge_ID_Flag,
                                       Number_of_Bnd_Edges,
                                       Number_of_Nodes,
                                       Number_of_Quads, Number_of_Trias,
                                       &Bnd_Edge_Connectivity,
                                       &Bnd_Edge_Grid_BC_Flag,
                                       &Bnd_Edge_ID_Flag,
                                       &Quad_Connectivity, 
                                       &Tria_Connectivity,
                                       &Coordinates,
                                       &Initial_Normal_Spacing,
                                       &BL_Thickness);

    if (Error_Flag > 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_2d_grid (Bnd_Edge_Connectivity, Bnd_Edge_Grid_BC_Flag,
                          Bnd_Edge_ID_Flag, Quad_Connectivity,
                          Tria_Connectivity,
                          Coordinates, Initial_Normal_Spacing, BL_Thickness);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Read all grid data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_2d_grid_file (Input_File_Name,
                                          Error_Message,
                                          UG_IO_Param_Struct_Ptr,
                                          Message_Flag,
                                          &Number_of_Bnd_Edges,
                                          &Number_of_Nodes,
                                          &Number_of_Quads,
                                          &Number_of_Trias,
                                          Bnd_Edge_Connectivity,
                                          Bnd_Edge_Grid_BC_Flag,
                                          Bnd_Edge_ID_Flag,
                                          Quad_Connectivity,
                                          Tria_Connectivity,
                                          Coordinates,
                                          Initial_Normal_Spacing,
                                          BL_Thickness);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_2d_grid (Bnd_Edge_Connectivity, Bnd_Edge_Grid_BC_Flag,
                          Bnd_Edge_ID_Flag, Quad_Connectivity,
                          Tria_Connectivity,
                          Coordinates, Initial_Normal_Spacing, BL_Thickness);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }
 
/*
 * -----------------------------------------------------------------------------
 * Output grid data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_write_2d_grid_file (Output_File_Name,
                                           Error_Message,
                                           UG_IO_Param_Struct_Ptr,
                                           Message_Flag,
                                           Number_of_Bnd_Edges,
                                           Number_of_Nodes,
                                           Number_of_Quads,
                                           Number_of_Trias,
                                           Bnd_Edge_Connectivity,
                                           Bnd_Edge_Grid_BC_Flag,
                                           Bnd_Edge_ID_Flag,
                                           Quad_Connectivity,
                                           Tria_Connectivity,
                                           Coordinates,
                                           Initial_Normal_Spacing,
                                           BL_Thickness);
 
    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_2d_grid (Bnd_Edge_Connectivity, Bnd_Edge_Grid_BC_Flag,
                          Bnd_Edge_ID_Flag, Quad_Connectivity,
                          Tria_Connectivity,
                          Coordinates, Initial_Normal_Spacing, BL_Thickness);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Free I/O parameter structure and grid data arrays.
 * -----------------------------------------------------------------------------
 */

    ug_io_free_param (UG_IO_Param_Struct_Ptr);

    ug_io_free_2d_grid (Bnd_Edge_Connectivity, Bnd_Edge_Grid_BC_Flag,
                        Bnd_Edge_ID_Flag, Quad_Connectivity, Tria_Connectivity,
                        Coordinates, Initial_Normal_Spacing, BL_Thickness);
  }

/*
 * -----------------------------------------------------------------------------
 * Start of section to read and write a node data file.
 * -----------------------------------------------------------------------------
 */

  else if (Data_Type_Flag == Node_Data_Type_Flag)
  {

/*
 * -----------------------------------------------------------------------------
 * Initialize node I/O arrays.
 * -----------------------------------------------------------------------------
 */

    Coordinates = NULL;
    Spacing = NULL;
    Vector = NULL;

/*
 * -----------------------------------------------------------------------------
 * Determine node data I/O variables and read node data dimensions.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_2d_node_file (Input_File_Name, Error_Message,
                                          UG_IO_Param_Struct_Ptr,
                                          Message_Flag, &Number_of_Nodes,
                                          Coordinates, Spacing, Vector);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Malloc node data arrays.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_malloc_2d_node (Error_Message,
                                       M_Spacing, M_Vector, Number_of_Nodes,
                                       &Coordinates, &Spacing, &Vector);

    if (Error_Flag > 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_2d_node (Coordinates, Spacing, Vector);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Read all node data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_2d_node_file (Input_File_Name, Error_Message,
                                          UG_IO_Param_Struct_Ptr,
                                          Message_Flag, &Number_of_Nodes,
                                          Coordinates, Spacing, Vector);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_2d_node (Coordinates, Spacing, Vector);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Output node data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_write_2d_node_file (Output_File_Name, Error_Message,
                                           UG_IO_Param_Struct_Ptr,
                                           Message_Flag, Number_of_Nodes,
                                           Coordinates, Spacing, Vector);
 
    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_2d_node (Coordinates, Spacing, Vector);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Free I/O parameter structure and node data arrays.
 * -----------------------------------------------------------------------------
 */

    ug_io_free_param (UG_IO_Param_Struct_Ptr);

    ug_io_free_2d_node (Coordinates, Spacing, Vector);
  }

  return (0);

}
