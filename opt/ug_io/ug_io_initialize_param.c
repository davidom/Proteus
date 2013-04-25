#include "UG_IO_LIB.h"

INT_ ug_io_initialize_param
 (char Error_Message[],
  INT_ Set_2D_Types,
  INT_ Set_3D_Types,
  INT_ Set_Func_Types,
  INT_ Set_Geom_Types,
  INT_ Set_Grid_Types,
  INT_ Set_Node_Types,
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr)

{

/*
 * Set the UG_IO file type, format, and compression parameters.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_initialize_param.c,v 1.104 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

//--------------------------------------------------------- "1234567890123456789012345678901234567890"--

  CHAR_41 *File_Compression_Label, *File_Format_Label, *File_Mode_Label,
          *File_Type_Label;
  CHAR_21 *File_Type_Suffix;
  CHAR_11 *File_Compression_Suffix, *File_Format_Suffix, *File_Mode_Suffix;

  INT_1D *File_Data_Type, *File_Format, *File_Mode, *File_Type_Format_Flag;

  INT_ Format_Option, Index, Input_Option, Label_Index, Max_File_Compressions,
       Max_File_Formats, Max_File_Modes, Max_File_Types, Max_IO_Param_Labels,
       Mode_Option, Number_of_File_Compressions, Number_of_File_Formats,
       Number_of_File_Modes, Number_of_File_Types, Output_Option;

  Max_IO_Param_Labels = UG_IO_Param_Struct_Ptr->Max_IO_Param_Labels;

  strcpy (Error_Message, "not enough space for UG_IO type file parameters");

//set file type parameters

  Number_of_File_Types = UG_IO_Param_Struct_Ptr->Number_of_File_Types;

  if (Number_of_File_Types == 0)
  {
    Max_File_Types = UG_IO_Param_Struct_Ptr->Max_File_Types;

    File_Data_Type = UG_IO_Param_Struct_Ptr->File_Data_Type;
    File_Type_Format_Flag = UG_IO_Param_Struct_Ptr->File_Type_Format_Flag;
    File_Type_Label = UG_IO_Param_Struct_Ptr->File_Type_Label;
    File_Type_Suffix = UG_IO_Param_Struct_Ptr->File_Type_Suffix;

    for (Index = 0; Index < Max_File_Types; ++Index)
    {
      for (Label_Index = 0; Label_Index < Max_IO_Param_Labels; ++Label_Index)
      {
        strcpy (File_Type_Label[Label_Index+Index*Max_IO_Param_Labels], "");
      }
    }

    Index = Number_of_File_Types-1;

//  geometry data types

    if (Set_Geom_Types == 1 && Set_3D_Types == 1)
    {
//    SM type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".SM");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "SolidMesh SaveState File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A SaveState file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data.");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    igs type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".igs");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "IGES Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An IGES file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data.");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    iges type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".iges");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "IGES Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An IGES file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data.");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    STEP type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".stp");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "STEP Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An STEP file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    BREP type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".BRep");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "BRep Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An OpenCASCADE BRep file contains");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "geometry definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    Parasolid type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".xmt_txt");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "ParaSolid Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A ParaSolid file contains  geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    UniGraphics type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".prt");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "UniGraphics Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A UniGraphics PRT file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    Pro-Engineer type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".prt");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "ProEngineer Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Pro-E PRT file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    Pro-Engineer type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".asm");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "ProEngineer Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Pro-E ASM file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    CATIA type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".CATPART");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "CatiaV5 Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Catia CATPART file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    CATIA type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".CATPRODUCT");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "CatiaV5 Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Catia CATPRODUCT file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    SolidWorks type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".sldprt");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "SolidWorks Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A SolidWorks SLDPRT file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    SolidWorks type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".sldasm");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "SolidWorks Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A SolidWorks SLDASM file contains geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data (requires CAPRI).");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    p3d type

      ++Index;

      if (Index >= Max_File_Types)
        return (632);

      strcpy (File_Type_Suffix[Index], ".p3d");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "PLOT3D Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A P3D file contains structured geometry");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "definition data.");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    xyz type

      ++Index;

      if (Index >= Max_File_Types)
        return (633);

      strcpy (File_Type_Suffix[Index], ".xyz");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "XYZ Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An XYZ file contains x,y,z coordinate");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "geometry definition data.");

      File_Data_Type[Index] = UG_IO_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;
    }

//  2D geometry data types

    if (Set_Geom_Types == 1 && Set_2D_Types == 1)
    {

//    xy type

      ++Index;

      if (Index >= Max_File_Types)
        return (633);

      strcpy (File_Type_Suffix[Index], ".xy");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "XY 2D Geometry Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An XY file contains x,y coordinate");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "geometry definition data.");

      File_Data_Type[Index] = UG_IO_2D_GEOM_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;
    }

//  node data types

    if (Set_Node_Types == 1 && Set_3D_Types == 1)
    {

//    node type

      ++Index;

      if (Index >= Max_File_Types)
        return (635);

      strcpy (File_Type_Suffix[Index], ".node");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "NODE 3D Node Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A NODE file contains a list of node");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "coordinates.");

      File_Data_Type[Index] = UG_IO_NODE_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    snode type

      ++Index;

      if (Index >= Max_File_Types)
        return (635);

      strcpy (File_Type_Suffix[Index], ".snode");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "SNODE 3D Source Node Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An SNODE file contains a list of node");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "coordinates and spacing function.");

      File_Data_Type[Index] = UG_IO_NODE_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    vnode type

      ++Index;

      if (Index >= Max_File_Types)
        return (635);

      strcpy (File_Type_Suffix[Index], ".vnode");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "VNODE 3D Directional Source Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A VNODE file contains a list of node");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "coordinates, spacing function, two unit");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "direction vectors, and two corresponding");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "aspect-ratios.");

      File_Data_Type[Index] = UG_IO_NODE_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;
    }

//  2D node data types

    if (Set_Node_Types == 1 && Set_2D_Types == 1)
    {

//    node2d type

      ++Index;

      if (Index >= Max_File_Types)
        return (634);

      strcpy (File_Type_Suffix[Index], ".node2d");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "NODE2D 2D Node Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A NODE2D file contains a list of node");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "coordinates.");

      File_Data_Type[Index] = UG_IO_2D_NODE_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    snode2d type

      ++Index;

      if (Index >= Max_File_Types)
        return (635);

      strcpy (File_Type_Suffix[Index], ".snode2d");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "SNODE2D 2D Source Node Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An SNODE2D file contains a list of node");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "coordinates and spacing function.");

      File_Data_Type[Index] = UG_IO_2D_NODE_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    vnode2d type

      ++Index;

      if (Index >= Max_File_Types)
        return (635);

      strcpy (File_Type_Suffix[Index], ".vnode2d");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "VNODE2D 2D Directional Source Data File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A VNODE2D file contains a list of node");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "coordinates, spacing function, unit");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "direction vector, and aspect-ratio.");

      File_Data_Type[Index] = UG_IO_2D_NODE_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;
    }

//  tags data types

    if (Set_Grid_Types == 1 && Set_3D_Types == 1)
    {

//    tags type

      ++Index;

      if (Index >= Max_File_Types)
        return (635);

      strcpy (File_Type_Suffix[Index], ".tags");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "TAGS Surface Tags Data Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A TAGS file contains a list of surface");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "IDs and surface BC properties.");

      File_Data_Type[Index] = UG_IO_TAGS_DATA;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;
    }

//  surface grid types

    if (Set_Grid_Types == 1 && Set_3D_Types == 1)
    {

//    poly type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".poly");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "POLY Unstructured B-Surf Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A POLY file contains a tria/quad face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid. This file type is");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "used by TetGen.");

      File_Data_Type[Index] = UG_IO_SURFACE_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    stl tria type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".stl");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "A 3D stereo-lithography B-Surf Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A STL file contains a tria face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid.");

      File_Data_Type[Index] = UG_IO_SURFACE_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    surf type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".surf");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "SURF 3D Unstructured B-Surf Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A SURF file contains a tria/quad face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid.");

      File_Data_Type[Index] = UG_IO_SURFACE_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    cart3d tria type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".tri");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Cart3D TRI Unstructured B-Surf Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Cart3D TRI file contains a tria face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid. This file type is");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "generated by the Cart3D system.");

      File_Data_Type[Index] = UG_IO_SURFACE_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    volume grid types

//    cfd++ type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".cfd++");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "CFD++ Grid File Set");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A CFD++ grid file set contains a tria/");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "quad face boundary surface grid and a");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "polyhedral element volume grid. This");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "file type uses multiple files for");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "coordinates (nodesin.bin), elements");
      strcpy (File_Type_Label[6+Index*Max_IO_Param_Labels], "(cellsin.bin), and surface faces");
      strcpy (File_Type_Label[7+Index*Max_IO_Param_Labels], "(exbcsin.bin).");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    cgns type

      if (ug_io_cgns_grid_lib_check ())
      {
        ++Index;

        if (Index >= Max_File_Types)
          return (609);

        strcpy (File_Type_Suffix[Index], ".cgns");
        strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "CGNS Data File");
        strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A CGNS data file contains a tria/quad");
        strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "face boundary surface grid and");
        strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "(optionally) a polyhedral element volume");
        strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "grid.");

        File_Data_Type[Index] = UG_IO_VOLUME_GRID;

        Input_Option = 1;
        Output_Option = 1;
        Mode_Option = 0;
        Format_Option = 0;

        File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                     + Mode_Option * 4 + Format_Option * 8;
      }

//    cobalt type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".cobalt.grd");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Cobalt Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Cobalt grid file contains a tria/quad");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "face boundary surface grid and a");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "polyhedral element volume grid.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    egrid ensight type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".egrid");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Ensight Gold Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An Ensight Gold grid file contains a");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "tria/quad face boundary surface grid and");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid. This");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "file type writes an additional case file");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "(case_name.case).");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    fgrid type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".fgrid");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "FGRID 3D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A FGRID file contains a triangulated");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a tetrahedral volume grid. The FGRID");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "file type is derived from the FAST");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "(PLOT3D) unstructured grid file type.");
      strcpy (File_Type_Label[6+Index*Max_IO_Param_Labels], "FGRID and FAST files are the same except");
      strcpy (File_Type_Label[7+Index*Max_IO_Param_Labels], "for optional FGRID boundary");
      strcpy (File_Type_Label[8+Index*Max_IO_Param_Labels], "reconnection, boundary condition, and");
      strcpy (File_Type_Label[9+Index*Max_IO_Param_Labels], "initial normal spacing data records.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    fluent type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".fluent.cas");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Fluent Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Fluent grid file contains a tria/quad");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "face boundary surface grid and a");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "polyhedral element volume grid.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    fieldview type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".fvuns");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Fieldview Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A Fieldview grid file contains");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    gambit type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".gambit");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "GAMBIT (Fluent) Neutral File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A GAMBIT Neutral file contains a tria/");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "quad face boundary surface grid and");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "(optionally) a polyhedral element volume");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "grid.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    mesh type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".mesh");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "MESH 3D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A MESH file contains a tria/quad face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid.");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "This file type is supported by the Gamma");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "Project of INRIA.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    meshb type

      if (ug_io_mesh_grid_lib_check ())
      {
        ++Index;

        if (Index >= Max_File_Types)
          return (609);

        strcpy (File_Type_Suffix[Index], ".meshb");
        strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Binary MESH 3D Unstructured Grid File");
        strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A MESH file contains a tria/quad face");
        strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
        strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid.");
        strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "This file type is supported by the Gamma");
        strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "Project of INRIA.");

        File_Data_Type[Index] = UG_IO_VOLUME_GRID;

        Input_Option = 1;
        Output_Option = 1;
        Mode_Option = 0;
        Format_Option = 0;

        File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                     + Mode_Option * 4 + Format_Option * 8;
      }

//    nas type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".bdf");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "NASTRAN 3D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A NASTRAN file contains a tria/quad face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid.");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "The suffix .nas and .bdf are equivalent.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".nas");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "NASTRAN 3D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A NASTRAN file contains a tria/quad face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid.");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "The suffix .nas and .bdf are equivalent.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    nsu3d type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".mcell");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "NSU3D Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A NSU3D grid file contains a tria/quad");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    OpenFOAM type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".foam");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "OpenFOAM polyMesh Grid Files");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A cellShape file contains a volume grid");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "that is comprised of points and");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "polyhedral elements defined by faces.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    ufast type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".ufast");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "UFAST 3D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A UFAST file contains a triangulated");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a tetrahedral volume grid. The UFAST");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "file type is identical to the FAST");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "(PLOT3D) unstructured grid file type.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    tetgen type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".face");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "TETGEN 3D Unstructured Grid File Set");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A TETGEN file set contains a tria face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a tetrahedral volume grid. This file");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "type uses multiple files for coordinates");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "(.node), elements (.ele), and faces");
      strcpy (File_Type_Label[6+Index*Max_IO_Param_Labels], "(.face). This file type is used by");
      strcpy (File_Type_Label[7+Index*Max_IO_Param_Labels], "TetGen.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    ugrid type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".ugrid");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "UGRID 3D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A UGRID file contains a tria/quad face");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a polyhedral element volume grid.");

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 1;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    vgrid type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".cogsg");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "VGRID 3D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A VGRID file contains a triangulated");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "boundary surface grid and (optionally)");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "a tetrahedral volume grid. This file");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "type uses multiple files for coordinates");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "and volume grid connectivity (.cogsg),");
#if 0
      strcpy (File_Type_Label[6+Index*Max_IO_Param_Labels], "coordinates only (.grd),");
      strcpy (File_Type_Label[7+Index*Max_IO_Param_Labels], "volume grid connectivity only (.int),");
      strcpy (File_Type_Label[8+Index*Max_IO_Param_Labels], "boundary face connectivity (.bc), and");
      strcpy (File_Type_Label[9+Index*Max_IO_Param_Labels], "boundary conditions (.mapbc).");
#else
      strcpy (File_Type_Label[6+Index*Max_IO_Param_Labels], "boundary face connectivity (.bc), and");
      strcpy (File_Type_Label[7+Index*Max_IO_Param_Labels], "boundary conditions (.mapbc).");
#endif

      File_Data_Type[Index] = UG_IO_VOLUME_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;
    }

//  2D edge grid types

    if (Set_Grid_Types == 1 && Set_2D_Types == 1)
    {

//    bedge type

      ++Index;

      if (Index >= Max_File_Types)
        return (603);

      strcpy (File_Type_Suffix[Index], ".bedge");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "BEDGE 2D B-Edge Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A BEDGE file contains a boundary edge");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "grid.");

      File_Data_Type[Index] = UG_IO_2D_EDGE_GRID;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    2D grid types

//    fgrid type

      ++Index;

      if (Index >= Max_File_Types)
        return (603);

      strcpy (File_Type_Suffix[Index], ".fgrid");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "FGRID 2D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A FGRID file contains a boundary edge");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "grid and (optionally) a tria face planar");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "grid. The FGRID 2D file type is derived");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "from the 3D FGRID file type. Differences");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "include; additional boundary edge grid");
      strcpy (File_Type_Label[6+Index*Max_IO_Param_Labels], "data, no volume elements, and all");
      strcpy (File_Type_Label[7+Index*Max_IO_Param_Labels], "z-coordinates are zero.");

      File_Data_Type[Index] = UG_IO_2D_GRID;

      Input_Option = 1;
      Output_Option = 0;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    ugrid type

      ++Index;

      if (Index >= Max_File_Types)
        return (603);

      strcpy (File_Type_Suffix[Index], ".ugrid");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "UGRID 2D Unstructured Grid File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A UGRID file contains a boundary edge");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "grid and (optionally) a mixed tria/quad");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "face planar grid. The UGRID 2D file type");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "is derived from the 3D UGRID file type.");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "Differences include; additional boundary");
      strcpy (File_Type_Label[6+Index*Max_IO_Param_Labels], "edge grid data, no volume elements, and");
      strcpy (File_Type_Label[7+Index*Max_IO_Param_Labels], "all z-coordinates are zero.");

      File_Data_Type[Index] = UG_IO_2D_GRID;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    mesh type

      if (ug_io_mesh_2d_grid_lib_check ())
      {
        ++Index;

        if (Index >= Max_File_Types)
          return (609);

        strcpy (File_Type_Suffix[Index], ".mesh");
        strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "MESH 2D Unstructured Grid File");
        strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A MESH file contains a boundary edge");
        strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "grid and (optionally) a tria/quad face");
        strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "planar grid.");
        strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "This file type is supported by the Gamma");
        strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "Project of INRIA.");

        File_Data_Type[Index] = UG_IO_2D_GRID;

        Input_Option = 1;
        Output_Option = 1;
        Mode_Option = 0;
        Format_Option = 0;

        File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                     + Mode_Option * 4 + Format_Option * 8;
      }

//    meshb type

      if (ug_io_mesh_2d_grid_lib_check ())
      {
        ++Index;

        if (Index >= Max_File_Types)
          return (609);

        strcpy (File_Type_Suffix[Index], ".meshb");
        strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Binary MESH 2D Unstructured Grid File");
        strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A MESH file contains a boundary edge");
        strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "grid and (optionally) a tria/quad face");
        strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "planar grid.");
        strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "This file type is supported by the Gamma");
        strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "Project of INRIA.");

        File_Data_Type[Index] = UG_IO_2D_GRID;

        Input_Option = 1;
        Output_Option = 1;
        Mode_Option = 0;
        Format_Option = 0;

        File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                     + Mode_Option * 4 + Format_Option * 8;
      }
    }

//  function data types

    if (Set_Func_Types == 1)
    {

//    ensight efunc type

      ++Index;

      if (Index >= Max_File_Types)
        return (609);

      strcpy (File_Type_Suffix[Index], ".efunc");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "Ensight Gold Function File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "An Ensight Gold function file contains a");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "data file. This file type assumes that a");
      strcpy (File_Type_Label[3+Index*Max_IO_Param_Labels], "grid data file will also be written out.");
      strcpy (File_Type_Label[4+Index*Max_IO_Param_Labels], "It also writes to an additional case");
      strcpy (File_Type_Label[5+Index*Max_IO_Param_Labels], "file (case_name.case).");

      File_Data_Type[Index] = UG_IO_FUNCTION_DATA;

      Input_Option = 0;
      Output_Option = 1;
      Mode_Option = 0;
      Format_Option = 0;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;

//    ufunc type

      ++Index;

      if (Index >= Max_File_Types)
        return (605);

      strcpy (File_Type_Suffix[Index], ".ufunc");
      strcpy (File_Type_Label[  Index*Max_IO_Param_Labels], "UFUNC 3D Function File");
      strcpy (File_Type_Label[1+Index*Max_IO_Param_Labels], "A UFUNC file contains a field function");
      strcpy (File_Type_Label[2+Index*Max_IO_Param_Labels], "(scalar and/or vector) data file.");

      File_Data_Type[Index] = UG_IO_FUNCTION_DATA;

      Input_Option = 1;
      Output_Option = 1;
      Mode_Option = 1;
      Format_Option = 1;

      File_Type_Format_Flag[Index] = Input_Option + Output_Option * 2
                                   + Mode_Option * 4 + Format_Option * 8;
    }

//  set number of file types

    Number_of_File_Types = Index+1;

    UG_IO_Param_Struct_Ptr->Number_of_File_Types = Number_of_File_Types;
  }

//set file format parameters

  Number_of_File_Formats = UG_IO_Param_Struct_Ptr->Number_of_File_Formats;

  if (Number_of_File_Formats == 0)
  {
    Max_File_Formats = UG_IO_Param_Struct_Ptr->Max_File_Formats;

    File_Format = UG_IO_Param_Struct_Ptr->File_Format;
    File_Format_Label = UG_IO_Param_Struct_Ptr->File_Format_Label;
    File_Format_Suffix = UG_IO_Param_Struct_Ptr->File_Format_Suffix;

    for (Index = 0; Index < Max_File_Formats; ++Index)
    {
      for (Label_Index = 0; Label_Index < Max_IO_Param_Labels; ++Label_Index)
      {
        strcpy (File_Format_Label[Label_Index+Index*Max_IO_Param_Labels], "");
      }
    }

    Index = Number_of_File_Formats-1;

//  formatted ASCII format

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], "");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "Formatted ASCII File or Binary Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "Specific to File Type");

    File_Format[Index] = UG_IO_FORMATTED;

//  unformatted real*8 format with big endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".r8");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "FORTRAN Unformatted Real*8 File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Big Endian Ordering");

    File_Format[Index] = UG_IO_UNFORMATTED_DOUBLE;

//  unformatted real*4 format with big endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".r4");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "FORTRAN Unformatted Real*4 File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Big Endian Ordering");

    File_Format[Index] = UG_IO_UNFORMATTED_SINGLE;

//  binary double format with big endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".b8");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "C Binary Double File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Big Endian Ordering");

    File_Format[Index] = UG_IO_BINARY_DOUBLE;

//  binary float format with big endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".b4");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "C Binary Float File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Big Endian Ordering");

    File_Format[Index] = UG_IO_BINARY_SINGLE;

//  unformatted real*8 format with little endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".lr8");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "FORTRAN Unformatted Real*8 File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Little Endian Ordering");

    File_Format[Index] = -UG_IO_UNFORMATTED_DOUBLE;

//  unformatted real*4 format with little endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".lr4");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "FORTRAN Unformatted Real*4 File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Little Endian Ordering");

    File_Format[Index] = -UG_IO_UNFORMATTED_SINGLE;

//  binary double format with little endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".lb8");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "C Binary Double File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Little Endian Ordering");

    File_Format[Index] = -UG_IO_BINARY_DOUBLE;

//  binary float format with little endian ordering

    ++Index;

    if (Index >= Max_File_Formats)
      return (606);

    strcpy (File_Format_Suffix[Index], ".lb4");
    strcpy (File_Format_Label[  Index*Max_IO_Param_Labels], "C Binary Float File Format");
    strcpy (File_Format_Label[1+Index*Max_IO_Param_Labels], "with Little Endian Ordering");

    File_Format[Index] = -UG_IO_BINARY_SINGLE;

//  set number of file formats

    Number_of_File_Formats = Index+1;

    UG_IO_Param_Struct_Ptr->Number_of_File_Formats = Number_of_File_Formats;
  }

//set file mode parameters

  Number_of_File_Modes = UG_IO_Param_Struct_Ptr->Number_of_File_Modes;

  if (Number_of_File_Modes == 0)
  {
    Max_File_Modes = UG_IO_Param_Struct_Ptr->Max_File_Modes;

    File_Mode = UG_IO_Param_Struct_Ptr->File_Mode;
    File_Mode_Label = UG_IO_Param_Struct_Ptr->File_Mode_Label;
    File_Mode_Suffix = UG_IO_Param_Struct_Ptr->File_Mode_Suffix;

    for (Index = 0; Index < Max_File_Modes; ++Index)
    {
      for (Label_Index = 0; Label_Index < Max_IO_Param_Labels; ++Label_Index)
      {
        strcpy (File_Mode_Label[Label_Index+Index*Max_IO_Param_Labels], "");
      }
    }

    Index = Number_of_File_Modes-1;

//  standard file mode

    ++Index;

    if (Index >= Max_File_Modes)
      return (606);

    strcpy (File_Mode_Suffix[Index], "");
    strcpy (File_Mode_Label[  Index*Max_IO_Param_Labels], "Standard File Mode");

    File_Mode[Index] = UG_FIO_STANDARD_FILE_MODE;

//  binary tmp file mode

    ++Index;

    if (Index >= Max_File_Modes)
      return (606);

    strcpy (File_Mode_Suffix[Index], ".tmp");
    strcpy (File_Mode_Label[  Index*Max_IO_Param_Labels], "Binary TMP File Mode");
    strcpy (File_Mode_Label[1+Index*Max_IO_Param_Labels], "Binary TMP files are not directly");
    strcpy (File_Mode_Label[2+Index*Max_IO_Param_Labels], "accessible and exist only temporarily in");
    strcpy (File_Mode_Label[3+Index*Max_IO_Param_Labels], "computer disk scratch space.");

    File_Mode[Index] = UG_FIO_TMP_FILE_MODE;

//  set number of file formats

    Number_of_File_Modes = Index+1;

    UG_IO_Param_Struct_Ptr->Number_of_File_Modes = Number_of_File_Modes;
  }

//set file compression parameters

  Number_of_File_Compressions = UG_IO_Param_Struct_Ptr->Number_of_File_Compressions;

  if (Number_of_File_Compressions == 0)
  {
    Max_File_Compressions = UG_IO_Param_Struct_Ptr->Max_File_Compressions;

    File_Compression_Label = UG_IO_Param_Struct_Ptr->File_Compression_Label;
    File_Compression_Suffix = UG_IO_Param_Struct_Ptr->File_Compression_Suffix;

    for (Index = 0; Index < Max_File_Compressions; ++Index)
    {
      for (Label_Index = 0; Label_Index < Max_IO_Param_Labels; ++Label_Index)
      {
        strcpy (File_Compression_Label[Label_Index+Index*Max_IO_Param_Labels], "");
      }
    }

    Index = Number_of_File_Compressions-1;

//  no compression

    ++Index;

    if (Index >= Max_File_Compressions)
      return (604);

    strcpy (File_Compression_Suffix[Index], "");
    strcpy (File_Compression_Label[  Index*Max_IO_Param_Labels], "No File Compression");

//  bzip2 compression

    ++Index;

    if (Index >= Max_File_Compressions)
      return (604);

    strcpy (File_Compression_Suffix[Index], ".bz2");
    strcpy (File_Compression_Label[  Index*Max_IO_Param_Labels], "BZIP2 File Compression");

//  gzip compression

    ++Index;

    if (Index >= Max_File_Compressions)
      return (604);

    strcpy (File_Compression_Suffix[Index], ".gz");
    strcpy (File_Compression_Label[  Index*Max_IO_Param_Labels], "GZIP File Compression");

//  compress compression

    ++Index;

    if (Index >= Max_File_Compressions)
      return (604);

    strcpy (File_Compression_Suffix[Index], ".Z");
    strcpy (File_Compression_Label[  Index*Max_IO_Param_Labels], "COMPRESS File Compression");

//  set number of file compressions

    Number_of_File_Compressions = Index+1;

    UG_IO_Param_Struct_Ptr->Number_of_File_Compressions = Number_of_File_Compressions;
  }

//set version flag

  UG_IO_Param_Struct_Ptr->Version_Flag = 0;

//done

  strcpy (Error_Message, "");

  return (0);

}
