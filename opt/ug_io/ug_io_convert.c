#include "UG_IO_LIB.h"

INT_ ug_io_convert
 (char Input_File_Name[],
  char Output_File_Name[],
  INT_ Quad_Flag,
  INT_ Reorder_Surf_ID_Flag,
  INT_ Task_Flag,
  INT_ Version_Flag)

{

/*
 * Convert UG_IO files.
 *
 * If Quad_Flag = 0 then convert surface grid quads to trias.
 * If Quad_Flag = 1 then do not convert surface grid quads.
 *
 * If Task_Flag = 1 then output summary of parameter information.
 * If Task_Flag = 2 then output full parameter information.
 * If Task_Flag = -1 then output summary of geometry parameter information.
 * If Task_Flag = -2 then output full geometry parameter information.
 * If Task_Flag = 3 or 4 then convert a grid, node or function file.
 * If Task_Flag = -3, -4 or -5 then convert a grid, node or function file
 * and enable interactive allocation/de-allocation of output file function
 * variables.
 * If Task_Flag = 4, -4, 5, or -5 then read in only the boundary surface grid
 * even if the input grid file contains a volume grid.
 * If Task_Flag = 5 or -5 then re-order the surface grid such that it is sorted
 * into groups by surface ID and duplicate all nodes that are shared by faces
 * with different surface IDs.
 * If Task_Flag = 13 then convert a grid, node or function file.
 * Also, allocate, read, set, and write volume ID flag.
 * If Task_Flag = -13 then convert a grid, node or function file
 * and enable interactive allocation/de-allocation of output file function
 * variables. Also, allocate, read, set, and write volume ID flag.
 *
 * If Version_Flag =  0 then use current version output grid type definition
 * If Version_Flag = -1 then use previous version output grid type definition
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_convert.c,v 1.23 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

/*
 * -----------------------------------------------------------------------------
 * Declare routine variables.
 * -----------------------------------------------------------------------------
 */

  CHAR_133 Error_Message;
  CHAR_UG_MAX Input_Function_File_Name;

  INT_1D *Node_Flag = NULL;
  INT_1D *Node_ID_Flag = NULL;
  INT_1D *Surf_Error_Flag = NULL;
  INT_1D *Surf_Flag = NULL;
  INT_1D *Quad_Tria_Flag = NULL;

  INT_ Data_Type_Flag, Flag, Index, Input_File_Data_Type, Output_File_Data_Type;

  INT_ Grid_Data_Type_Flag = 1;
  INT_ Node_Data_Type_Flag = 2;
  INT_ Func_Data_Type_Flag = 3;
  INT_ Error_Flag = 0;
  INT_ Known_Error = 0;
  INT_ Message_Flag = 1;
  INT_ M_Quad_Tria_Flag = 0;
  INT_ Number_of_Nodes_Dim = 0;

  int true_int = 0;

/*
 * -----------------------------------------------------------------------------
 * Declare grid data I/O variables.
 * -----------------------------------------------------------------------------
 */

  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr;

  INT_1D *Surf_Grid_BC_Flag, *Surf_ID_Flag, *Surf_Reconnection_Flag,
         *Vol_ID_Flag;

  INT_3D *Surf_Tria_Connectivity;

  INT_4D *Surf_Quad_Connectivity, *Vol_Tet_Connectivity;

  INT_5D *Vol_Pent_5_Connectivity;

  INT_6D *Vol_Pent_6_Connectivity;

  INT_8D *Vol_Hex_Connectivity;

  DOUBLE_1D *Initial_Normal_Spacing, *BL_Thickness;

  DOUBLE_3D *Coordinates;

  INT_ Number_of_BL_Vol_Tets, Number_of_Nodes, Number_of_Surf_Quads,
       Number_of_Surf_Trias, Number_of_Vol_Hexs, Number_of_Vol_Pents_5,
       Number_of_Vol_Pents_6, Number_of_Vol_Tets;

  INT_ M_BL_Thickness = 1;
  INT_ M_Initial_Normal_Spacing = 1;
  INT_ M_Surf_Grid_BC_Flag = 1;
  INT_ M_Surf_ID_Flag = 1;
  INT_ M_Surf_Reconnection_Flag = 1;
  INT_ M_Vol_ID_Flag = 0;

/*
 * -----------------------------------------------------------------------------
 * Declare node data I/O variables.
 * -----------------------------------------------------------------------------
 */

  DOUBLE_1D *Spacing;
  DOUBLE_4D *Vector1;
  DOUBLE_4D *Vector2;

  INT_ M_Spacing = 1;
  INT_ M_Vector1 = 1;
  INT_ M_Vector2 = 1;

/*
 * -----------------------------------------------------------------------------
 * Declare function data I/O variables.
 * -----------------------------------------------------------------------------
 */

  CHAR_21 *U_Scalar_Labels, *U_Vector_Labels;

  INT_1D *U_Scalar_Flags, *U_Vector_Flags;

  DOUBLE_1D *U_Scalars;
  DOUBLE_3D *U_Vectors;

  INT_ Number_of_U_Scalars, Number_of_U_Vectors;

/*
 * -----------------------------------------------------------------------------
 * Set volume ID flag.
 * -----------------------------------------------------------------------------
 */

  if (Task_Flag == -13 || Task_Flag == 13)
  {
    M_Vol_ID_Flag = 1;

    if (Task_Flag == -13) Task_Flag = -3;
    if (Task_Flag == 13) Task_Flag = 3;
  }

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
 * Set default values for I/O parameters.
 * -----------------------------------------------------------------------------
 */

  if (Task_Flag == -1 || Task_Flag == -2)
    Error_Flag = ug_io_initialize_param (Error_Message, 0, 1, 0, 1, 0, 0,
                                         UG_IO_Param_Struct_Ptr);
  else
    Error_Flag = ug_io_initialize_param (Error_Message, 0, 1, 1, 0, 1, 1,
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

  if (abs (Task_Flag) == 1 || abs (Task_Flag) == 2)
  {
    ug_io_write_param_info (Task_Flag, UG_IO_Param_Struct_Ptr);

    ug_io_free_param (UG_IO_Param_Struct_Ptr);

    return (0);
  }

/*
 * -----------------------------------------------------------------------------
 * Check to see if the strings Input_File_Name and Output_File_Name contains
 * grid, node or function file names.
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

    if (Input_File_Data_Type == UG_IO_SURFACE_GRID ||
        Input_File_Data_Type == UG_IO_VOLUME_GRID)
    {
      Data_Type_Flag = Grid_Data_Type_Flag;

      if (Output_File_Data_Type != UG_IO_SURFACE_GRID &&
          Output_File_Data_Type != UG_IO_VOLUME_GRID)
      {
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        Error_Flag = 699;
        strcpy (Error_Message,
                "if input is a grid file then output must be also");
        ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
        return (Error_Flag);
      }
    }

    else if (Input_File_Data_Type == UG_IO_NODE_DATA)
    {
      Data_Type_Flag = Node_Data_Type_Flag;

      if (Output_File_Data_Type != UG_IO_NODE_DATA)
      {
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        Error_Flag = 699;
        strcpy (Error_Message,
                "if input is a node file then output must be also");
        ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
        return (Error_Flag);
      }
    }

    else if (Input_File_Data_Type == UG_IO_FUNCTION_DATA)
    {
      Data_Type_Flag = Func_Data_Type_Flag;

      if (Output_File_Data_Type != UG_IO_FUNCTION_DATA)
      {
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        Error_Flag = 699;
        strcpy (Error_Message,
                "if input is a function file then output must be also");
        ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
        return (Error_Flag);
      }
    }

    else
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      Error_Flag = 699;
      strcpy (Error_Message,
              "file is not a grid, node or function data type");
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

    Surf_Grid_BC_Flag = NULL;
    Surf_ID_Flag = NULL;
    Surf_Reconnection_Flag = NULL;
    Surf_Quad_Connectivity = NULL;
    Surf_Tria_Connectivity = NULL;
    Vol_Hex_Connectivity = NULL;
    Vol_ID_Flag = NULL;
    Vol_Pent_5_Connectivity = NULL;
    Vol_Pent_6_Connectivity = NULL;
    Vol_Tet_Connectivity = NULL;
    Coordinates = NULL;
    Initial_Normal_Spacing = NULL;
    BL_Thickness = NULL;

/*
 * -----------------------------------------------------------------------------
 * Set option flag to reorder the surface face ID flag.
 * -----------------------------------------------------------------------------
 */

    if (Reorder_Surf_ID_Flag == 1)
      Number_of_Surf_Trias = -1;
    else
      Number_of_Surf_Trias = 0;

    Number_of_Surf_Quads = 0;

/*
 * -----------------------------------------------------------------------------
 * Set option flag to only read in the boundary surface grid.
 * -----------------------------------------------------------------------------
 */

    if (Task_Flag >= 4 || Task_Flag <= -4)
      Number_of_Vol_Tets = -1;
    else
      Number_of_Vol_Tets = 0;

/*
 * -----------------------------------------------------------------------------
 * Determine grid I/O variables and read grid dimensions.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_grid_file (Input_File_Name, Error_Message,
                                       UG_IO_Param_Struct_Ptr,
                                       Message_Flag,
                                       &Number_of_BL_Vol_Tets,
                                       &Number_of_Nodes,
                                       &Number_of_Surf_Quads,
                                       &Number_of_Surf_Trias,
                                       &Number_of_Vol_Hexs,
                                       &Number_of_Vol_Pents_5,
                                       &Number_of_Vol_Pents_6,
                                       &Number_of_Vol_Tets,
                                       Surf_Grid_BC_Flag, Surf_ID_Flag,
                                       Surf_Reconnection_Flag,
                                       Surf_Quad_Connectivity,
                                       Surf_Tria_Connectivity,
                                       Vol_Hex_Connectivity, Vol_ID_Flag,
                                       Vol_Pent_5_Connectivity,
                                       Vol_Pent_6_Connectivity,
                                       Vol_Tet_Connectivity,
                                       Coordinates,
                                       Initial_Normal_Spacing, BL_Thickness);

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

    Error_Flag = ug_io_malloc_grid (Error_Message,
                                    M_BL_Thickness,
                                    M_Initial_Normal_Spacing,
                                    M_Surf_Grid_BC_Flag, M_Surf_ID_Flag,
                                    M_Surf_Reconnection_Flag, M_Vol_ID_Flag,
                                    Number_of_Nodes,
                                    Number_of_Surf_Quads, Number_of_Surf_Trias,
                                    Number_of_Vol_Hexs,
                                    Number_of_Vol_Pents_5,
                                    Number_of_Vol_Pents_6,
                                    Number_of_Vol_Tets,
                                    &Surf_Grid_BC_Flag, &Surf_ID_Flag,
                                    &Surf_Reconnection_Flag,
                                    &Surf_Quad_Connectivity, 
                                    &Surf_Tria_Connectivity,
                                    &Vol_Hex_Connectivity, &Vol_ID_Flag,
                                    &Vol_Pent_5_Connectivity,
                                    &Vol_Pent_6_Connectivity,
                                    &Vol_Tet_Connectivity,
                                    &Coordinates,
                                    &Initial_Normal_Spacing, &BL_Thickness);

    if (Error_Flag > 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_grid (Surf_Grid_BC_Flag, Surf_ID_Flag, Surf_Reconnection_Flag,
                       Surf_Quad_Connectivity, Surf_Tria_Connectivity,
                       Vol_Hex_Connectivity, Vol_ID_Flag,
                       Vol_Pent_5_Connectivity, Vol_Pent_6_Connectivity,
                       Vol_Tet_Connectivity,
                       Coordinates, Initial_Normal_Spacing, BL_Thickness);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Read all grid data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_grid_file (Input_File_Name, Error_Message,
                                       UG_IO_Param_Struct_Ptr,
                                       Message_Flag,
                                       &Number_of_BL_Vol_Tets,
                                       &Number_of_Nodes,
                                       &Number_of_Surf_Quads,
                                       &Number_of_Surf_Trias,
                                       &Number_of_Vol_Hexs,
                                       &Number_of_Vol_Pents_5,
                                       &Number_of_Vol_Pents_6,
                                       &Number_of_Vol_Tets,
                                       Surf_Grid_BC_Flag, Surf_ID_Flag,
                                       Surf_Reconnection_Flag,
                                       Surf_Quad_Connectivity,
                                       Surf_Tria_Connectivity,
                                       Vol_Hex_Connectivity, Vol_ID_Flag,
                                       Vol_Pent_5_Connectivity,
                                       Vol_Pent_6_Connectivity,
                                       Vol_Tet_Connectivity,
                                       Coordinates,
                                       Initial_Normal_Spacing, BL_Thickness);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_grid (Surf_Grid_BC_Flag, Surf_ID_Flag, Surf_Reconnection_Flag,
                       Surf_Quad_Connectivity, Surf_Tria_Connectivity,
                       Vol_Hex_Connectivity, Vol_ID_Flag,
                       Vol_Pent_5_Connectivity, Vol_Pent_6_Connectivity,
                       Vol_Tet_Connectivity,
                       Coordinates, Initial_Normal_Spacing, BL_Thickness);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Convert surface grid quads to trias.
 * -----------------------------------------------------------------------------
 */

    if ((Quad_Flag == 0 || Task_Flag == 5 || Task_Flag == -5) &&
        Number_of_Surf_Quads > 0 &&
        Number_of_Vol_Hexs == 0 && 
        Number_of_Vol_Pents_5 == 0 && 
        Number_of_Vol_Pents_6 == 0 && 
        Number_of_Vol_Tets == 0)
    {
      Error_Flag = ug3_qtria (M_Quad_Tria_Flag,
                              &Number_of_Surf_Trias, &Number_of_Surf_Quads,
                              &Surf_Grid_BC_Flag, &Surf_ID_Flag,
                              &Surf_Error_Flag,
                              &Surf_Tria_Connectivity, Surf_Quad_Connectivity,
                              &Quad_Tria_Flag, &Surf_Reconnection_Flag,
                              Coordinates);

      if (Error_Flag != 0)
      {
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        ug_io_free_grid (Surf_Grid_BC_Flag, Surf_ID_Flag,
                         Surf_Reconnection_Flag,
                         Surf_Quad_Connectivity, Surf_Tria_Connectivity,
                         Vol_Hex_Connectivity, Vol_ID_Flag,
                         Vol_Pent_5_Connectivity, Vol_Pent_6_Connectivity,
                         Vol_Tet_Connectivity,
                         Coordinates, Initial_Normal_Spacing, BL_Thickness);
        ug3_error_message (Error_Flag, &Known_Error);
        return (Error_Flag);
      }
    }

/*
 * -----------------------------------------------------------------------------
 * Re-order a surface grid in separate groups by ID
 * -----------------------------------------------------------------------------
 */

    if (Task_Flag == 5 || Task_Flag == -5)
    {
      Number_of_Nodes_Dim = Number_of_Nodes;

      Error_Flag = 0;

      Surf_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                        (Number_of_Surf_Trias+1)
                                      * sizeof (INT_1D));
      Node_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                        (Number_of_Nodes+1)
                                      * sizeof (INT_1D));
      Node_ID_Flag = (INT_1D *) ug_malloc (&Error_Flag,
                                           (Number_of_Nodes+1)
                                         * sizeof (INT_1D));

      if (Error_Flag > 0)
      {
        Error_Flag = 699;
        strcpy (Error_Message, "unable to malloc work arrays for re-ordering surface grid by ID");
        ug_free (Surf_Flag);
        ug_free (Node_Flag);
        ug_free (Node_ID_Flag);
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        ug_io_free_grid (Surf_Grid_BC_Flag, Surf_ID_Flag,
                         Surf_Reconnection_Flag,
                         Surf_Quad_Connectivity, Surf_Tria_Connectivity,
                         Vol_Hex_Connectivity, Vol_ID_Flag,
                         Vol_Pent_5_Connectivity, Vol_Pent_6_Connectivity,
                         Vol_Tet_Connectivity,
                         Coordinates, Initial_Normal_Spacing, BL_Thickness);
        ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
        return (Error_Flag);
      }

      Error_Flag = ug3_reorderid (Number_of_Surf_Trias,
                                  &Number_of_Nodes, &Number_of_Nodes_Dim,
                                  Surf_Grid_BC_Flag, Surf_ID_Flag,
                                  &Node_ID_Flag, Surf_Tria_Connectivity,
                                  Surf_Reconnection_Flag, Surf_Flag, &Node_Flag,
                                  &BL_Thickness, &Initial_Normal_Spacing,
                                  &Coordinates);

      ug_free (Surf_Flag);
      ug_free (Node_Flag);
      ug_free (Node_ID_Flag);
 
      if (Error_Flag != 0)
      {
        ug_io_free_param (UG_IO_Param_Struct_Ptr);
        ug_io_free_grid (Surf_Grid_BC_Flag, Surf_ID_Flag,
                         Surf_Reconnection_Flag,
                         Surf_Quad_Connectivity, Surf_Tria_Connectivity,
                         Vol_Hex_Connectivity, Vol_ID_Flag,
                         Vol_Pent_5_Connectivity, Vol_Pent_6_Connectivity,
                         Vol_Tet_Connectivity,
                         Coordinates, Initial_Normal_Spacing, BL_Thickness);
        ug3_error_message (Error_Flag, &Known_Error);
        return (Error_Flag);
      }
    }

/*
 * -----------------------------------------------------------------------------
 * Set option flag to reorder the output grid file surface face ID flag.
 * -----------------------------------------------------------------------------
 */

    if (Reorder_Surf_ID_Flag == 2)
      Number_of_Surf_Trias = -Number_of_Surf_Trias;

/*
 * -----------------------------------------------------------------------------
 * Output grid data.
 * -----------------------------------------------------------------------------
 */

    UG_IO_Param_Struct_Ptr->Version_Flag = Version_Flag;

    Error_Flag = ug_io_write_grid_file (Output_File_Name, Error_Message,
                                        UG_IO_Param_Struct_Ptr,
                                        Message_Flag,
                                        Number_of_BL_Vol_Tets,
                                        Number_of_Nodes,
                                        Number_of_Surf_Quads,
                                        Number_of_Surf_Trias,
                                        Number_of_Vol_Hexs,
                                        Number_of_Vol_Pents_5,
                                        Number_of_Vol_Pents_6,
                                        Number_of_Vol_Tets,
                                        Surf_Grid_BC_Flag, Surf_ID_Flag,
                                        Surf_Reconnection_Flag,
                                        Surf_Quad_Connectivity,
                                        Surf_Tria_Connectivity,
                                        Vol_Hex_Connectivity, Vol_ID_Flag,
                                        Vol_Pent_5_Connectivity,
                                        Vol_Pent_6_Connectivity,
                                        Vol_Tet_Connectivity,
                                        Coordinates,
                                        Initial_Normal_Spacing, BL_Thickness);

    Number_of_Surf_Trias = abs (Number_of_Surf_Trias);
 
    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_grid (Surf_Grid_BC_Flag, Surf_ID_Flag, Surf_Reconnection_Flag,
                       Surf_Quad_Connectivity, Surf_Tria_Connectivity,
                       Vol_Hex_Connectivity, Vol_ID_Flag,
                       Vol_Pent_5_Connectivity, Vol_Pent_6_Connectivity,
                       Vol_Tet_Connectivity,
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

    ug_io_free_grid (Surf_Grid_BC_Flag, Surf_ID_Flag, Surf_Reconnection_Flag,
                     Surf_Quad_Connectivity, Surf_Tria_Connectivity,
                     Vol_Hex_Connectivity, Vol_ID_Flag,
                     Vol_Pent_5_Connectivity, Vol_Pent_6_Connectivity,
                     Vol_Tet_Connectivity,
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
    Vector1 = NULL;
    Vector2 = NULL;

/*
 * -----------------------------------------------------------------------------
 * Determine node data I/O variables and read node data dimensions.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_node_file (Input_File_Name, Error_Message,
                                       UG_IO_Param_Struct_Ptr,
                                       Message_Flag, &Number_of_Nodes,
                                       Coordinates, Spacing, Vector1, Vector2);

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

    Error_Flag = ug_io_malloc_node (Error_Message,
                                    M_Spacing, M_Vector1, M_Vector2,
                                    Number_of_Nodes,
                                    &Coordinates, &Spacing, &Vector1, &Vector2);

    if (Error_Flag > 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_node (Coordinates, Spacing, Vector1, Vector2);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Read all node data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_node_file (Input_File_Name, Error_Message,
                                       UG_IO_Param_Struct_Ptr,
                                       Message_Flag, &Number_of_Nodes,
                                       Coordinates, Spacing, Vector1, Vector2);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_node (Coordinates, Spacing, Vector1, Vector2);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Output node data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_write_node_file (Output_File_Name, Error_Message,
                                        UG_IO_Param_Struct_Ptr,
                                        Message_Flag, Number_of_Nodes,
                                        Coordinates, Spacing, Vector1, Vector2);
 
    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_node (Coordinates, Spacing, Vector1, Vector2);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Free I/O parameter structure and node data arrays.
 * -----------------------------------------------------------------------------
 */

    ug_io_free_param (UG_IO_Param_Struct_Ptr);

    ug_io_free_node (Coordinates, Spacing, Vector1, Vector2);
  }

/*
 * -----------------------------------------------------------------------------
 * Start of section to read and write a function data file.
 * -----------------------------------------------------------------------------
 */

  else if (Data_Type_Flag == Func_Data_Type_Flag)
  {

/*
 * -----------------------------------------------------------------------------
 * Check to see if the string Input_File_Name contains a function file name.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_file_type (Input_File_Name, Error_Message,
                                  UG_IO_Param_Struct_Ptr,
                                  &Input_File_Data_Type);

    strcpy (Input_Function_File_Name, Input_File_Name);

/*
 * -----------------------------------------------------------------------------
 * Initialize function data I/O arrays.
 * -----------------------------------------------------------------------------
 */

    U_Scalar_Labels = NULL;
    U_Vector_Labels = NULL;
    U_Scalar_Flags = NULL;
    U_Vector_Flags = NULL;
    U_Scalars = NULL;
    U_Vectors = NULL;

    Number_of_Nodes = 0;

/*
 * -----------------------------------------------------------------------------
 * Determine function data I/O variables and read data dimensions.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_func_file (Input_Function_File_Name, Error_Message,
                                       UG_IO_Param_Struct_Ptr,
                                       Message_Flag,
                                       &Number_of_Nodes, &Number_of_U_Scalars,  
                                       &Number_of_U_Vectors,
                                       U_Scalar_Labels, U_Vector_Labels,
                                       U_Scalar_Flags, U_Vector_Flags,
                                       U_Scalars, U_Vectors);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Malloc function label and flag arrays.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_malloc_func_flag (Error_Message,
                                         Number_of_U_Scalars, 
                                         Number_of_U_Vectors,
                                         &U_Scalar_Labels, &U_Vector_Labels,
                                         &U_Scalar_Flags, &U_Vector_Flags);

    if (Error_Flag > 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_func_flag (U_Scalar_Labels, U_Vector_Labels,
                            U_Scalar_Flags, U_Vector_Flags);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Read and set function labels and flags.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_func_file (Input_Function_File_Name, Error_Message,
                                       UG_IO_Param_Struct_Ptr,
                                       Message_Flag,
                                       &Number_of_Nodes, &Number_of_U_Scalars,  
                                       &Number_of_U_Vectors,
                                       U_Scalar_Labels, U_Vector_Labels,
                                       U_Scalar_Flags, U_Vector_Flags,
                                       U_Scalars, U_Vectors);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_func_flag (U_Scalar_Labels, U_Vector_Labels,
                            U_Scalar_Flags, U_Vector_Flags);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Allocate or De-allocate variables.
 * -----------------------------------------------------------------------------
 */

    if (Task_Flag <= -3)
    {
      printf ("\n");
      printf ("Enter 1 to change or 0 to not change scalar/vector variable allocation : ");

      fflush (stdout);

      scanf ("%i", &true_int);

      printf ("\n");

      Flag = (INT_) true_int;

      if (Flag == 1)
      {
        for (Index = 0; Index < Number_of_U_Scalars; ++Index)
        {
          printf ("Scalar Label %-4i = %s\n", Index, U_Scalar_Labels[Index]);
          printf ("Enter 1 to allocate, read and write this scalar variable\n");
          printf ("      0 to de-allocate this scalar variable : ");

          fflush (stdout);

          scanf ("%i", &true_int);

          true_int = MIN (MAX (true_int, 0), 1);

          U_Scalar_Flags[Index] = (INT_) true_int;

          printf ("\n");
        }

        printf ("\n");

        for (Index = 0; Index < Number_of_U_Vectors; ++Index)
        {
          printf ("Vector Label %-4i = %s\n", Index, U_Vector_Labels[Index]);
          printf ("Enter 1 to allocate, read and write this vector variable\n");
          printf ("      0 to de-allocate this vector variable : ");

          fflush (stdout);

          scanf ("%i", &true_int);

          true_int = MIN (MAX (true_int, 0), 1);

          U_Vector_Flags[Index] = (INT_) true_int;

          printf ("\n");
        }
      }
    }

/*
 * -----------------------------------------------------------------------------
 * Malloc function data arrays.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_malloc_func (Error_Message, Number_of_Nodes,
                                    Number_of_U_Scalars, 
                                    Number_of_U_Vectors,
                                    U_Scalar_Flags, U_Vector_Flags,
                                    &U_Scalars, &U_Vectors);

    if (Error_Flag > 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_func_flag (U_Scalar_Labels, U_Vector_Labels,
                            U_Scalar_Flags, U_Vector_Flags);
      ug_io_free_func (U_Scalars, U_Vectors);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Read function data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_read_func_file (Input_Function_File_Name, Error_Message,
                                       UG_IO_Param_Struct_Ptr,
                                       Message_Flag,
                                       &Number_of_Nodes, &Number_of_U_Scalars,  
                                       &Number_of_U_Vectors,
                                       U_Scalar_Labels, U_Vector_Labels,
                                       U_Scalar_Flags, U_Vector_Flags,
                                       U_Scalars, U_Vectors);

    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_func_flag (U_Scalar_Labels, U_Vector_Labels,
                            U_Scalar_Flags, U_Vector_Flags);
      ug_io_free_func (U_Scalars, U_Vectors);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Output function data.
 * -----------------------------------------------------------------------------
 */

    Error_Flag = ug_io_write_func_file (Output_File_Name, Error_Message,
                                        UG_IO_Param_Struct_Ptr,
                                        Message_Flag, Number_of_Nodes,
                                        Number_of_U_Scalars,
                                        Number_of_U_Vectors,
                                        U_Scalar_Labels, U_Vector_Labels,
                                        U_Scalar_Flags, U_Vector_Flags,
                                        U_Scalars, U_Vectors);
 
    if (Error_Flag != 0)
    {
      ug_io_free_param (UG_IO_Param_Struct_Ptr);
      ug_io_free_func_flag (U_Scalar_Labels, U_Vector_Labels,
                            U_Scalar_Flags, U_Vector_Flags);
      ug_io_free_func (U_Scalars, U_Vectors);
      ug_io_error_message (Error_Message, Error_Flag, &Known_Error);
      return (Error_Flag);
    }

/*
 * -----------------------------------------------------------------------------
 * Free I/O parameter structure and function data arrays.
 * -----------------------------------------------------------------------------
 */

    ug_io_free_param (UG_IO_Param_Struct_Ptr);

    ug_io_free_func_flag (U_Scalar_Labels, U_Vector_Labels,
                          U_Scalar_Flags, U_Vector_Flags);

    ug_io_free_func (U_Scalars, U_Vectors);
  }

  return (0);

}
