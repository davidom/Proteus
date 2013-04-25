#include "UG_IO_LIB.h"

INT_ ug_io_write_cfdpp
 (char Error_Message[],
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6,
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D * Vol_Hex_Connectivity,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)
{

/*
 * Write grid data to a binary CFD++ volume grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_cfdpp.c,v 1.10 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Data_File;

  INT_ *ID_Map;

  INT_ celtyp, Error_Flag, Found_Flag, ID, Index, Index1, Index2, Index3,
       Index4, Index5, Index6, Index7, Index8, info_length, Max_ID, mcells,
       mexbcs, Min_ID, New_ID, no_of_vertices, node_type, nodvar_l, Write_Flag;

  INT_ vernum = 0;

  mcells = Number_of_Vol_Tets 
         + Number_of_Vol_Pents_5 
         + Number_of_Vol_Pents_6 
         + Number_of_Vol_Hexs ;

  if (mcells == 0)
  {
    strcpy (Error_Message, "CFD++ grid data must include volume elements");
    return (1);
  }

  // Write out the cells in cellsin.bin

  Data_File = ug_fopen ("cellsin.bin", "w");
  
  if (Data_File == NULL)
  {
    strcpy (Error_Message, "unable to open CFD++ cellsin.bin file");
    return (1);
  }

  mcells = Number_of_Vol_Tets 
         + Number_of_Vol_Pents_5 
         + Number_of_Vol_Pents_6 
         + Number_of_Vol_Hexs ;

  info_length = Number_of_Vol_Tets*4
              + Number_of_Vol_Pents_5*5
              + Number_of_Vol_Pents_6*6
              + Number_of_Vol_Hexs*8;

  Write_Flag = - 3 - Number_of_Vol_Tets*5
                   - Number_of_Vol_Pents_5*6
                   - Number_of_Vol_Pents_6*7
                   - Number_of_Vol_Hexs*9;

  Write_Flag += ug_fwrite (&vernum, sizeof (INT_), 1, Data_File);
  Write_Flag += ug_fwrite (&mcells, sizeof (INT_), 1, Data_File);
  Write_Flag += ug_fwrite (&info_length, sizeof (INT_), 1, Data_File);

  celtyp = 2;

  for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
  {
    Index1 = Vol_Tet_Connectivity[Index][0]-1;
    Index2 = Vol_Tet_Connectivity[Index][1]-1;
    Index3 = Vol_Tet_Connectivity[Index][2]-1;
    Index4 = Vol_Tet_Connectivity[Index][3]-1;

    Write_Flag += ug_fwrite (&celtyp, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index1, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index2, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index3, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index4, sizeof (INT_), 1, Data_File);
  }

  celtyp = 6;

  for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
  {
    Index1 = Vol_Pent_5_Connectivity[Index][0]-1;
    Index2 = Vol_Pent_5_Connectivity[Index][1]-1;
    Index3 = Vol_Pent_5_Connectivity[Index][3]-1;
    Index4 = Vol_Pent_5_Connectivity[Index][4]-1;
    Index5 = Vol_Pent_5_Connectivity[Index][2]-1;

    Write_Flag += ug_fwrite (&celtyp, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index1, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index2, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index3, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index4, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index5, sizeof (INT_), 1, Data_File);
  }

  celtyp = 1;

  for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
  {
    Index1 = Vol_Pent_6_Connectivity[Index][0]-1;
    Index2 = Vol_Pent_6_Connectivity[Index][1]-1;
    Index3 = Vol_Pent_6_Connectivity[Index][2]-1;
    Index4 = Vol_Pent_6_Connectivity[Index][3]-1;
    Index5 = Vol_Pent_6_Connectivity[Index][4]-1;
    Index6 = Vol_Pent_6_Connectivity[Index][5]-1;

    Write_Flag += ug_fwrite (&celtyp, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index1, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index2, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index3, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index4, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index5, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index6, sizeof (INT_), 1, Data_File);
  }

  celtyp = 0;

  for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
  {
    Index1 = Vol_Hex_Connectivity[Index][0]-1;
    Index2 = Vol_Hex_Connectivity[Index][1]-1;
    Index3 = Vol_Hex_Connectivity[Index][3]-1;
    Index4 = Vol_Hex_Connectivity[Index][2]-1;
    Index5 = Vol_Hex_Connectivity[Index][4]-1;
    Index6 = Vol_Hex_Connectivity[Index][5]-1;
    Index7 = Vol_Hex_Connectivity[Index][7]-1;
    Index8 = Vol_Hex_Connectivity[Index][6]-1;

    Write_Flag += ug_fwrite (&celtyp, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index1, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index2, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index3, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index4, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index5, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index6, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index7, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index8, sizeof (INT_), 1, Data_File);
  }

  ug_fclose (Data_File);

  if (Write_Flag != 0)
  {
    strcpy (Error_Message, "error writing CFD++ cellsin.bin file");
    return (1);
  }

  // Write out the nodes in nodesin.bin

  Data_File = ug_fopen ("nodesin.bin", "w");
  
  if (Data_File == NULL)
  {
    strcpy (Error_Message, "unable to open CFD++ nodesin.bin file");
    return (1);
  }

  node_type = 0;

  nodvar_l = Number_of_Nodes*3;

  Write_Flag = - 3 - Number_of_Nodes*4;

  Write_Flag += ug_fwrite (&vernum, sizeof (INT_), 1, Data_File);
  Write_Flag += ug_fwrite (&Number_of_Nodes, sizeof (INT_), 1, Data_File);
  Write_Flag += ug_fwrite (&nodvar_l, sizeof (INT_), 1, Data_File);

  for (Index = 1; Index <= Number_of_Nodes; ++Index)
  {
    Write_Flag += ug_fwrite (&node_type, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Coordinates[Index][0],
                             sizeof (double), 3, Data_File);
  }

  ug_fclose (Data_File);

  if (Write_Flag != 0)
  {
    strcpy (Error_Message, "error writing CFD++ nodesin.bin file");
    return (1);
  }

  // Write out external boundary conditions (grid groups) in exbcsin.bin

  Max_ID = Surf_ID_Flag[1];
  Min_ID = Surf_ID_Flag[1];

  for (Index = 1; Index <= Number_of_Surf_Trias+Number_of_Surf_Quads; ++Index)
  {
    ID = Surf_ID_Flag[Index];

    Max_ID = MAX (ID, Max_ID);
    Min_ID = MIN (ID, Min_ID);
  }

  Error_Flag = 0;

  ID_Map = (INT_ *) ug_malloc (&Error_Flag, (Max_ID-Min_ID+1) * sizeof (INT_));

  if (Error_Flag > 0)
  {
    ug_free (ID_Map);
    strcpy (Error_Message, "unable to malloc CFD++ work array");
    return (1);
  }

  New_ID = 0;

  for (ID = Min_ID; ID <= Max_ID; ++ID)
  {
    Found_Flag = 0;

    Index = 1;

    while (Index <= Number_of_Surf_Trias+Number_of_Surf_Quads &&
           Found_Flag == 0)
    {
      if (Surf_ID_Flag[Index] == ID)
        Found_Flag = 1;

      ++Index;
    }

    if (Found_Flag == 1)
    {
      ++New_ID;

      ID_Map[ID-Min_ID] = New_ID;
    }
    else
      ID_Map[ID-Min_ID] = 0;
  }

  Data_File = ug_fopen ("exbcsin.bin", "w");
  
  if (Data_File == NULL)
  {
    ug_free (ID_Map);
    strcpy (Error_Message, "unable to open CFD++ exbcsin.bin file");
    return (1);
  }

  mexbcs = Number_of_Surf_Trias + Number_of_Surf_Quads;

  Write_Flag = - 2 - Number_of_Surf_Trias*6 - Number_of_Surf_Quads*7;

  Write_Flag += ug_fwrite (&vernum, sizeof (INT_), 1, Data_File);
  Write_Flag += ug_fwrite (&mexbcs, sizeof (INT_), 1, Data_File);

  no_of_vertices = 3;

  for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
  {
    ID = Surf_ID_Flag[Index];

    New_ID = ID_Map[ID-Min_ID];

    Index1 = Surf_Tria_Connectivity[Index][0]-1;
    Index2 = Surf_Tria_Connectivity[Index][1]-1;
    Index3 = Surf_Tria_Connectivity[Index][2]-1;

    Write_Flag += ug_fwrite (&New_ID, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&New_ID, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&no_of_vertices, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index1, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index2, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index3, sizeof (INT_), 1, Data_File);
  }

  no_of_vertices = 4;

  for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
  {
    ID = Surf_ID_Flag[Index+Number_of_Surf_Trias];

    New_ID = ID_Map[ID-Min_ID];

    Index1 = Surf_Quad_Connectivity[Index][0]-1;
    Index2 = Surf_Quad_Connectivity[Index][1]-1;
    Index3 = Surf_Quad_Connectivity[Index][2]-1;
    Index4 = Surf_Quad_Connectivity[Index][3]-1;

    Write_Flag += ug_fwrite (&New_ID, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&New_ID, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&no_of_vertices, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index1, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index2, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index3, sizeof (INT_), 1, Data_File);
    Write_Flag += ug_fwrite (&Index4, sizeof (INT_), 1, Data_File);
  }

  ug_fclose (Data_File);

  ug_free (ID_Map);

  if (Write_Flag != 0)
  {
    strcpy (Error_Message, "error writing CFD++ exbcsin.bin file");
    return (1);
  }

  return (0);
}
