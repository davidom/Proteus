#include "UG_IO_LIB.h"

INT_ ug_io_read_nsu3d
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Bnd_Nodes,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Hexs,
  INT_ *Number_of_Vol_Pents_5,
  INT_ *Number_of_Vol_Pents_6,
  INT_ *Number_of_Vol_Tets,
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
 * Read grid data from a NSU3D volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_nsu3d.c,v 1.3 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index, Mode_Flag, Number_of_Bytes, Number_of_Read_Items,
       Number_of_Surf_Faces, Number_of_Total_Nodes, Read_Flag,
       Special_Read_Items;

  INT_ Number_of_Hex8, Number_of_Pen5, Number_of_Pen6, Number_of_Tet4;

  /* NSU3D cell ordering to AFLR cell ordering */
  INT_ tet4_conn[4] = {0,1,2,3};
  INT_ pen5_conn[5] = {1,0,3,4,2}; 
  INT_ pen6_conn[6] = {4,3,5,1,0,2};
  INT_ hex8_conn[8] = {0,1,2,3,4,5,6,7};


  INT_ Number_of_Patches, Number_of_Components, Number_of_Bodies, nfrpts;

  INT_ TMP_int, j, k;
  float TMP_float;
  double TMP_double;

  if (File_Format == UG_IO_FORMATTED)
  {
    strcpy (Error_Message, "NSU3D format not supported");
    return(1);
  }
  else
  {
    Mode_Flag = ug_get_file_stream_mode_flag (Grid_File);

    Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                           File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    if (Read_Task_Flag == 1 || Mode_Flag == UG_FIO_STANDARD_FILE_MODE)
    {
      /* Header information */
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Tets,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Pents_5,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Pents_6,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Hexs,
                                        sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = 4 + 2 * Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items)
      {
        strcpy (Error_Message, "error reading NSU3D grid file");
        return (1);
      }

      Number_of_Tet4 = *Number_of_Vol_Tets;
      Number_of_Pen5 = *Number_of_Vol_Pents_5;
      Number_of_Pen6 = *Number_of_Vol_Pents_6;
      Number_of_Hex8 = *Number_of_Vol_Hexs;

      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Surf_Trias,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Surf_Quads,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (&Number_of_Patches,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (&Number_of_Components,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (&Number_of_Bodies,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (&nfrpts,
                                        sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = 7 + 2 * Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items)
      {
        strcpy (Error_Message, "error reading NSU3D grid file");
        return (1);
      }
    }

    /* BEGIN Read_Task_Flag == 1 && *Number_of_Bnd_Nodes = -1 */
    
    if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
    {
      Read_Flag = 0;

      *Number_of_Bnd_Nodes = 0;

      /* Tetrahedra */
      if (Number_of_Tet4 > 0)
      {
	Number_of_Bytes =  Number_of_Tet4 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 4; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  /* k = tet4_conn[j]; */
	  for (Index = 1; Index <= Number_of_Tet4; ++Index)
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Tet4 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* 5-Pentahedra */
      if (Number_of_Pen5 > 0)
      {
	Number_of_Bytes =  Number_of_Pen5 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 5; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  /* k = pen5_conn[j]; */
	  for (Index = 1; Index <= Number_of_Pen5; ++Index)
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Pen5 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* 6-Pentahedra */
      if (Number_of_Pen6 > 0)
      {
	Number_of_Bytes =  Number_of_Pen6 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 6; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  /* k = pen6_conn[j]; */
	  for (Index = 1; Index <= Number_of_Pen6; ++Index)
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Pen6 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Hexahedra */
      if (Number_of_Hex8 > 0)
      {
	Number_of_Bytes =  Number_of_Hex8 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 8; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  /* k = hex8_conn[j]; */
	  for (Index = 1; Index <= Number_of_Hex8; ++Index)
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Hex8 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }
      
      /* Coordinates */
      if (*Number_of_Nodes > 0)
      {
	Number_of_Bytes =  *Number_of_Nodes * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
					       ? ((INT_) (sizeof (double)))
					       : ((INT_) (sizeof (float))));

	for (j = 0; j < 3; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
	      File_Format == UG_IO_BINARY_SINGLE)
	  {
	    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_float,
						sizeof (float), 1, Grid_File);
	    }
	  }
	  else
	  {
	    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
	      Read_Flag = Read_Flag + ug_fread (&TMP_double,
						sizeof (double), 1, Grid_File);
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = *Number_of_Nodes + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Triangles */
      if (*Number_of_Surf_Trias > 0)
      {
	Number_of_Bytes =  *Number_of_Surf_Trias * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 4; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
	  {
	    if ( j < 3 )
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_int,
						sizeof (INT_), 1, Grid_File);

	      *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, TMP_int);
	    }
	    else /* ifbc */
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_int,
						sizeof (INT_), 1, Grid_File);
	    }
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = *Number_of_Surf_Trias + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Quadrilaterals */
      if (*Number_of_Surf_Quads > 0)
      {
	Number_of_Bytes =  *Number_of_Surf_Quads * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 5; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
	  {
	    if ( j < 4 )
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_int,
						sizeof (INT_), 1, Grid_File);

	      *Number_of_Bnd_Nodes = MAX (*Number_of_Bnd_Nodes, TMP_int);
	    }
	    else /* ifbc */
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_int,
						sizeof (INT_), 1, Grid_File);
	    }
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = *Number_of_Surf_Quads + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}
      }
    }  /* END Read_Task_Flag == 1 && *Number_of_Bnd_Nodes = -1 */

    Number_of_Total_Nodes = *Number_of_Nodes;

    Number_of_Surf_Faces = *Number_of_Surf_Trias + *Number_of_Surf_Quads; 

    if (*Number_of_Bnd_Nodes > 0)
    {
      *Number_of_Nodes = *Number_of_Bnd_Nodes;
      *Number_of_Vol_Hexs = 0;
      *Number_of_Vol_Pents_5 = 0;
      *Number_of_Vol_Pents_6 = 0;
      *Number_of_Vol_Tets = 0;
    }

    /* BEGIN full read */

    if (Read_Task_Flag == 2)
    {
      /* Tetrahedra */
      if (Number_of_Tet4 > 0)
      {
	Number_of_Bytes =  Number_of_Tet4 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 4; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  k = tet4_conn[j];
	  for (Index = 1; Index <= Number_of_Tet4; ++Index)
	  {
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);
	    if (*Number_of_Vol_Tets > 0) Vol_Tet_Connectivity[Index][k] = TMP_int;
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Tet4 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* 5-Pentahedra */
      if (Number_of_Pen5 > 0)
      {
	Number_of_Bytes =  Number_of_Pen5 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 5; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  k = pen5_conn[j];
	  for (Index = 1; Index <= Number_of_Pen5; ++Index)
	  {
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);
	    if (*Number_of_Vol_Pents_5 > 0) Vol_Pent_5_Connectivity[Index][k] = TMP_int;
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Pen5 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* 6-Pentahedra */
      if (Number_of_Pen6 > 0)
      {
	Number_of_Bytes =  Number_of_Pen6 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 6; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  k = pen6_conn[j];
	  for (Index = 1; Index <= Number_of_Pen6; ++Index)
	  {
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);
	    if (*Number_of_Vol_Pents_6 > 0) Vol_Pent_6_Connectivity[Index][k] = TMP_int;
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Pen6 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Hexahedra */
      if (Number_of_Hex8 > 0)
      {
	Number_of_Bytes =  Number_of_Hex8 * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 8; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  k = hex8_conn[j];
	  for (Index = 1; Index <= *Number_of_Vol_Hexs; ++Index)
	  {
	    Read_Flag = Read_Flag + ug_fread (&TMP_int,
					      sizeof (INT_), 1, Grid_File);
	    if (*Number_of_Vol_Hexs > 0) Vol_Hex_Connectivity[Index][k] = TMP_int;
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Hex8 + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Coordinates */
      if (Number_of_Total_Nodes > 0)
      {
	for (j = 0; j < 3; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
	      File_Format == UG_IO_BINARY_SINGLE)
	  {
	    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_float,
						sizeof (float), 1, Grid_File);
	      Coordinates[Index][j] = (double) TMP_float;
	    }

	    for (Index = 1;
		 Index <= Number_of_Total_Nodes-(*Number_of_Nodes); ++Index)
	    {
	      Read_Flag = Read_Flag
		+ ug_fread (&TMP_float, sizeof (float), 1, Grid_File);
	    }
	  }
	  else
	  {
	    for (Index = 1; Index <= *Number_of_Nodes; ++Index)
	    {
	      Read_Flag = Read_Flag + ug_fread (&Coordinates[Index][j],
						sizeof (double), 1, Grid_File);
	    }

	    for (Index = 1;
		 Index <= Number_of_Total_Nodes-(*Number_of_Nodes); ++Index)
	    {
	      Read_Flag = Read_Flag
		+ ug_fread (&TMP_double, sizeof (double), 1, Grid_File);
	    }
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = Number_of_Total_Nodes + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error reading NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Triangles */
      if (*Number_of_Surf_Trias > 0)
      {
	Number_of_Bytes =  *Number_of_Surf_Trias * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 4; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
	  {
	    if ( j < 3 )
	    {
	      Read_Flag = Read_Flag + ug_fread (&Surf_Tria_Connectivity[Index][j],
						sizeof (INT_), 1, Grid_File);
	    }
	    else /* ifbc */
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_int,
						sizeof (INT_), 1, Grid_File);
	    }
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = *Number_of_Surf_Trias + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Quadrilaterals */
      if (*Number_of_Surf_Quads > 0)
      {
	Number_of_Bytes =  *Number_of_Surf_Quads * ((INT_) (sizeof (INT_)));

	for (j = 0; j < 5; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
	  {
	    if ( j < 4 )
	    {
	      Read_Flag = Read_Flag + ug_fread (&Surf_Quad_Connectivity[Index][j],
						sizeof (INT_), 1, Grid_File);
	    }
	    else /* ifbc */
	    {
	      Read_Flag = Read_Flag + ug_fread (&TMP_int,
						sizeof (INT_), 1, Grid_File);
	    }
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = *Number_of_Surf_Quads + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Triangle Surface IDs */
      if (*Number_of_Surf_Trias > 0)
      {
	Number_of_Bytes =  *Number_of_Surf_Trias * ((INT_) (sizeof (INT_)));

	Read_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					    sizeof (INT_), 1, Grid_File);

	for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
	  Read_Flag = Read_Flag + ug_fread (&Surf_ID_Flag[Index],
					    sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					    sizeof (INT_), 1, Grid_File);

	Number_of_Read_Items = *Number_of_Surf_Trias + 2 * Special_Read_Items;

	if (Read_Flag != Number_of_Read_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}

	Number_of_Bytes =  *Number_of_Surf_Trias * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
						    ? ((INT_) (sizeof (double)))
						    : ((INT_) (sizeof (float))));
      
	/* tri fspln */
	for (j = 0; j < 6; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
	      File_Format == UG_IO_BINARY_SINGLE)
	  {
	    for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
	      Read_Flag = Read_Flag + ug_fread (&TMP_float,
						sizeof (float), 1, Grid_File);
	  }
	  else
	  {
	    for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
	      Read_Flag = Read_Flag + ug_fread (&TMP_double,
						sizeof (double), 1, Grid_File);
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = *Number_of_Surf_Trias + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}
      }

      /* Quad Surface IDs */
      if (*Number_of_Surf_Quads > 0)
      {
	Number_of_Bytes =  *Number_of_Surf_Quads * ((INT_) (sizeof (INT_)));

	Read_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					    sizeof (INT_), 1, Grid_File);

	for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
	  Read_Flag = Read_Flag + ug_fread (&Surf_ID_Flag[*Number_of_Surf_Trias + Index],
					    sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					    sizeof (INT_), 1, Grid_File);

	Number_of_Read_Items = *Number_of_Surf_Quads + 2 * Special_Read_Items;

	if (Read_Flag != Number_of_Read_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}

	Number_of_Bytes =  *Number_of_Surf_Quads * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
						    ? ((INT_) (sizeof (double)))
						    : ((INT_) (sizeof (float))));

	/* quad fspln */
	for (j = 0; j < 8; j++)
	{
	  Read_Flag = 0;

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
	      File_Format == UG_IO_BINARY_SINGLE)
	  {
	    for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
	      Read_Flag = Read_Flag + ug_fread (&TMP_float,
						sizeof (float), 1, Grid_File);
	  }
	  else
	  {
	    for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
	      Read_Flag = Read_Flag + ug_fread (&TMP_double,
						sizeof (double), 1, Grid_File);
	  }

	  if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	      File_Format == UG_IO_UNFORMATTED_SINGLE)
	    Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
					      sizeof (INT_), 1, Grid_File);

	  Number_of_Read_Items = *Number_of_Surf_Quads + 2 * Special_Read_Items;

	  if (Read_Flag != Number_of_Read_Items)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}
      } /* if (Read_Task_Flag == 2) */
    }
  }

  return (0);
}
