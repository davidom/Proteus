#include "UG_IO_LIB.h"

static INT_ Grid_Data_Flag_ = 0;
static INT_ Number_of_Nodes_ = 0;
static INT_ Number_of_Surf_Quads_ = 0;
static INT_ Number_of_Surf_Trias_ = 0;
static INT_ Number_of_Vol_Hexs_ = 0;
static INT_ Number_of_Vol_Pents_5_ = 0;
static INT_ Number_of_Vol_Pents_6_ = 0;
static INT_ Number_of_Vol_Tets_ = 0;

static INT_1D *Surf_Grid_BC_Flag_ = NULL;
static INT_1D *Surf_ID_Flag_ = NULL;
static INT_1D *Surf_Reconnection_Flag_ = NULL;
static INT_4D *Surf_Quad_Connectivity_ = NULL;
static INT_3D *Surf_Tria_Connectivity_ = NULL;
static INT_8D *Vol_Hex_Connectivity_ = NULL;
static INT_1D *Vol_ID_Flag_ = NULL;
static INT_5D *Vol_Pent_5_Connectivity_ = NULL;
static INT_6D *Vol_Pent_6_Connectivity_ = NULL;
static INT_4D *Vol_Tet_Connectivity_ = NULL;
static DOUBLE_3D *Coordinates_ = NULL;
static DOUBLE_1D *Initial_Normal_Spacing_ = NULL;
static DOUBLE_1D *BL_Thickness_ = NULL;

static INT_ Func_Data_Flag_ = 0;
static INT_ Number_of_Func_Nodes_ = 0;
static INT_ Number_of_U_Scalars_ = 0;
static INT_ Number_of_U_Vectors_ = 0;

static CHAR_21 *U_Scalar_Labels_ = NULL;
static CHAR_21 *U_Vector_Labels_ = NULL;
static INT_1D *U_Scalar_Flags_ = NULL;
static INT_1D *U_Vector_Flags_ = NULL;
static DOUBLE_1D *U_Scalars_ = NULL;
static DOUBLE_3D *U_Vectors_ = NULL;

/*
 * Routines to save and get pointers to grid and function data.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_data.c,v 1.2 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

INT_ ug_io_get_func_data
 (INT_ *Number_of_Func_Nodes,
  INT_ *Number_of_U_Scalars,
  INT_ *Number_of_U_Vectors,
  CHAR_21 ** U_Scalar_Labels,
  CHAR_21 ** U_Vector_Labels,
  INT_1D ** U_Scalar_Flags,
  INT_1D ** U_Vector_Flags,
  DOUBLE_1D ** U_Scalars,
  DOUBLE_3D ** U_Vectors)

{

  if (Func_Data_Flag_ == 0)
    return (1);

  *Number_of_Func_Nodes = Number_of_Func_Nodes_;
  *Number_of_U_Scalars = Number_of_U_Scalars_;
  *Number_of_U_Vectors = Number_of_U_Vectors_;

  *U_Scalar_Labels = U_Scalar_Labels_;
  *U_Vector_Labels = U_Vector_Labels_;
  *U_Scalar_Flags = U_Scalar_Flags_;
  *U_Vector_Flags = U_Vector_Flags_;
  *U_Scalars = U_Scalars_;
  *U_Vectors = U_Vectors_;

  return (0);

}

INT_ ug_io_get_grid_data
 (INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Hexs,
  INT_ *Number_of_Vol_Pents_5,
  INT_ *Number_of_Vol_Pents_6,
  INT_ *Number_of_Vol_Tets,
  INT_1D ** Surf_Grid_BC_Flag,
  INT_1D ** Surf_ID_Flag,
  INT_1D ** Surf_Reconnection_Flag,
  INT_4D ** Surf_Quad_Connectivity,
  INT_3D ** Surf_Tria_Connectivity,
  INT_8D ** Vol_Hex_Connectivity,
  INT_1D ** Vol_ID_Flag,
  INT_5D ** Vol_Pent_5_Connectivity,
  INT_6D ** Vol_Pent_6_Connectivity,
  INT_4D ** Vol_Tet_Connectivity,
  DOUBLE_3D ** Coordinates,
  DOUBLE_1D ** Initial_Normal_Spacing,
  DOUBLE_1D ** BL_Thickness)

{

  if (Grid_Data_Flag_ == 0)
    return (1);

  *Number_of_Nodes = Number_of_Nodes_;
  *Number_of_Surf_Quads = Number_of_Surf_Quads_;
  *Number_of_Surf_Trias = Number_of_Surf_Trias_;
  *Number_of_Vol_Hexs = Number_of_Vol_Hexs_;
  *Number_of_Vol_Pents_5 = Number_of_Vol_Pents_5_;
  *Number_of_Vol_Pents_6 = Number_of_Vol_Pents_6_;
  *Number_of_Vol_Tets = Number_of_Vol_Tets_;

  *Surf_Grid_BC_Flag = Surf_Grid_BC_Flag_;
  *Surf_ID_Flag = Surf_ID_Flag_;
  *Surf_Reconnection_Flag = Surf_Reconnection_Flag_;
  *Surf_Quad_Connectivity = Surf_Quad_Connectivity_;
  *Surf_Tria_Connectivity = Surf_Tria_Connectivity_;
  *Vol_Hex_Connectivity = Vol_Hex_Connectivity_;
  *Vol_ID_Flag = Vol_ID_Flag_;
  *Vol_Pent_5_Connectivity = Vol_Pent_5_Connectivity_;
  *Vol_Pent_6_Connectivity = Vol_Pent_6_Connectivity_;
  *Vol_Tet_Connectivity = Vol_Tet_Connectivity_;
  *Coordinates = Coordinates_;
  *Initial_Normal_Spacing = Initial_Normal_Spacing_;
  *BL_Thickness = BL_Thickness_;

  return (0);

}

void ug_io_save_func_data
 (INT_ Number_of_Func_Nodes,
  INT_ Number_of_U_Scalars,
  INT_ Number_of_U_Vectors,
  CHAR_21 ** U_Scalar_Labels,
  CHAR_21 ** U_Vector_Labels,
  INT_1D ** U_Scalar_Flags,
  INT_1D ** U_Vector_Flags,
  DOUBLE_1D ** U_Scalars,
  DOUBLE_3D ** U_Vectors)

{

  Func_Data_Flag_ = 1;

  Number_of_Func_Nodes_ = Number_of_Func_Nodes;
  Number_of_U_Scalars_ = Number_of_U_Scalars;
  Number_of_U_Vectors_ = Number_of_U_Vectors;

  U_Scalar_Labels_ = *U_Scalar_Labels;
  U_Vector_Labels_ = *U_Vector_Labels;
  U_Scalar_Flags_ = *U_Scalar_Flags;
  U_Vector_Flags_ = *U_Vector_Flags;
  U_Scalars_ = *U_Scalars;
  U_Vectors_ = *U_Vectors;

  return;

}

void ug_io_save_grid_data
 (INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6,
  INT_ Number_of_Vol_Tets,
  INT_1D ** Surf_Grid_BC_Flag,
  INT_1D ** Surf_ID_Flag,
  INT_1D ** Surf_Reconnection_Flag,
  INT_4D ** Surf_Quad_Connectivity,
  INT_3D ** Surf_Tria_Connectivity,
  INT_8D ** Vol_Hex_Connectivity,
  INT_1D ** Vol_ID_Flag,
  INT_5D ** Vol_Pent_5_Connectivity,
  INT_6D ** Vol_Pent_6_Connectivity,
  INT_4D ** Vol_Tet_Connectivity,
  DOUBLE_3D ** Coordinates,
  DOUBLE_1D ** Initial_Normal_Spacing,
  DOUBLE_1D ** BL_Thickness)

{

  Grid_Data_Flag_ = 1;

  Number_of_Nodes_ = Number_of_Nodes;
  Number_of_Surf_Quads_ = Number_of_Surf_Quads;
  Number_of_Surf_Trias_ = Number_of_Surf_Trias;
  Number_of_Vol_Hexs_ = Number_of_Vol_Hexs;
  Number_of_Vol_Pents_5_ = Number_of_Vol_Pents_5;
  Number_of_Vol_Pents_6_ = Number_of_Vol_Pents_6;
  Number_of_Vol_Tets_ = Number_of_Vol_Tets;

  Surf_Grid_BC_Flag_ = *Surf_Grid_BC_Flag;
  Surf_ID_Flag_ = *Surf_ID_Flag;
  Surf_Reconnection_Flag_ = *Surf_Reconnection_Flag;
  Surf_Quad_Connectivity_ = *Surf_Quad_Connectivity;
  Surf_Tria_Connectivity_ = *Surf_Tria_Connectivity;
  Vol_Hex_Connectivity_ = *Vol_Hex_Connectivity;
  Vol_ID_Flag_ = *Vol_ID_Flag;
  Vol_Pent_5_Connectivity_ = *Vol_Pent_5_Connectivity;
  Vol_Pent_6_Connectivity_ = *Vol_Pent_6_Connectivity;
  Vol_Tet_Connectivity_ = *Vol_Tet_Connectivity;
  Coordinates_ = *Coordinates;
  Initial_Normal_Spacing_ = *Initial_Normal_Spacing;
  BL_Thickness_ = *BL_Thickness;
  
  return;

}
