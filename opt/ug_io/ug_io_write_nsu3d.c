#include "UG_IO_LIB.h"

INT_ ug_io_write_nsu3d
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
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
 * Write grid data to a NSU3D volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_nsu3d.c,v 1.8 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  /* http://wwweng.uwyo.edu/mechanical/faculty/Dimitri_Mavriplis/DOCUMENTATION/edges.pzm.jpg */

  INT_ j, k, nfrpts, nval, Index, Number_of_Bytes, Number_of_Bodies,
       Number_of_Components, Number_of_Patches, Number_of_Surf_Faces,
       Number_of_Write_Items, Special_Write_Items, TMP_int, Write_Flag;
  INT_ tet4_conn[4] = {0,1,2,3};
  INT_ pen5_conn[5] = {1,0,3,4,2};
  INT_ pen6_conn[6] = {4,3,5,1,0,2};
  INT_ hex8_conn[8] = {0,1,2,3,4,5,6,7};

  float TMP_float;

  double TMP_double;

  nfrpts = 0;
  Number_of_Bodies = 1;
  Number_of_Components = 1;

  Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;

  /* Assume Surface IDs are numbered 1,2,3... */
  Number_of_Patches = ug_max_int (1, Number_of_Surf_Faces, Surf_ID_Flag);

  if (File_Format == UG_IO_FORMATTED)
  {
    nval = 20;

    /* Write the header information */
    Write_Flag = fprintf (Grid_File, "%i %i %i %i\n",
                          Number_of_Vol_Tets, Number_of_Vol_Pents_5,
                          Number_of_Vol_Pents_6, Number_of_Vol_Hexs);

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing NSU3D grid file");
      return (1);
    }

    Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i %i\n", Number_of_Nodes,
                          Number_of_Surf_Trias, Number_of_Surf_Quads,
                          Number_of_Patches, Number_of_Components, Number_of_Bodies, nfrpts);

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing NSU3D grid file");
      return (1);
    }

    /* Tetrahedra */
    if (Number_of_Vol_Tets > 0)
    {
      for (j = 0; j < 4; j++)
      {
	k = tet4_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
	{
	  Write_Flag = fprintf (Grid_File, "%i ",
				Vol_Tet_Connectivity[Index][k]);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* 5-Pentahedra */
    if (Number_of_Vol_Pents_5 > 0)
    {
      for (j = 0; j < 5; j++)
      {
	k = pen5_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
	{
	  Write_Flag = fprintf (Grid_File, "%i ",
				Vol_Pent_5_Connectivity[Index][k]);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* 6-Pentahedra */
    if (Number_of_Vol_Pents_6 > 0)
    {
      for (j = 0; j < 6; j++)
      {
	k = pen6_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
	{
	  Write_Flag = fprintf (Grid_File, "%i ",
				Vol_Pent_6_Connectivity[Index][k]);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}
	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* Hexahedra */
    if (Number_of_Vol_Hexs > 0)
    {
      for (j = 0; j < 8; j++)
      {
	k = hex8_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
	{
	  Write_Flag = fprintf (Grid_File, "%i ",
				Vol_Hex_Connectivity[Index][k]);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* Coordinates */
    if (Number_of_Nodes > 0)
    {
      for (j = 0; j < 3; j++)
      {
	for (Index = 1; Index <= Number_of_Nodes; ++Index)
	{
	  Write_Flag = fprintf (Grid_File, "%.15g ",
				Coordinates[Index][j]);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* Triangles */
    if (Number_of_Surf_Trias > 0)
    {
      for (j = 0; j < 4; j++)
      {
	for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
	{
	  TMP_int = (j < 3) ? Surf_Tria_Connectivity[Index][j] : 0;

	  Write_Flag = fprintf (Grid_File, "%i ", TMP_int);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* Quadrilaterals */
    if (Number_of_Surf_Quads > 0)
    {
      for (j = 0; j < 5; j++)
      {
	for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
	{
	  TMP_int = (j < 4) ? Surf_Quad_Connectivity[Index][j] : 0;

	  Write_Flag = fprintf (Grid_File, "%i ", TMP_int);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* Triangle Surface IDs */
    if (Number_of_Surf_Trias > 0)
    {
      for (j = 0; j < 7; j++)
      {
	for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
	{
	  TMP_int = (j == 0) ? Surf_ID_Flag[Index]: 0;

	  Write_Flag = fprintf (Grid_File, "%i ", TMP_int);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

    /* Quad Surface IDs */
    if (Number_of_Surf_Quads > 0)
    {
      for (j = 0; j < 9; j++)
      {
	for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
	{
	  TMP_int = (j == 0) ? Surf_ID_Flag[Index+Number_of_Surf_Trias]: 0;

	  Write_Flag = fprintf (Grid_File, "%i ", TMP_int);

	  if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");

	  if (Write_Flag < 0)
	  {
	    strcpy (Error_Message, "error writing NSU3D grid file");
	    return (1);
	  }
	}

	if (!(Index % nval)) Write_Flag = fprintf (Grid_File, "\n");
      }
    }

   /* Component definition */
    if (Number_of_Patches > 0)
    {
      for (Index = 1; Index <= Number_of_Patches; ++Index)
      {
	Write_Flag = fprintf (Grid_File, "%i ", 1);
	if (Write_Flag < 0)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
	Write_Flag = fprintf (Grid_File, "\n");
      }
    }

   /* Body definition */
   Write_Flag = fprintf (Grid_File, "%i %i\n", 1, 1);

   if (Write_Flag < 0)
   {
     strcpy (Error_Message, "error writing NSU3D grid file");
     return (1);
   }
   Write_Flag = fprintf (Grid_File, "\n");
  }
  else
  {
    Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                            File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    /* Header information */

    Number_of_Bytes = 4 * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Tets,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Pents_5,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Pents_6,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Hexs,
                                         sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = 4 + 2 * Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing NSU3D grid file");
      return (1);
    }

    Number_of_Bytes = 7 * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Trias,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Quads,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Patches,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Components,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bodies,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&nfrpts,
                                         sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = 7 + 2 * Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing NSU3D grid file");
      return (1);
    }

    /* Tetrahedra */
    if (Number_of_Vol_Tets > 0)
    {
      Number_of_Bytes =  Number_of_Vol_Tets * ((INT_) (sizeof (INT_)));

      for (j = 0; j < 4; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	k = tet4_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
	  Write_Flag = Write_Flag + ug_fwrite (&Vol_Tet_Connectivity[Index][k],
					       sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Vol_Tets + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* 5-Pentahedra */
    if (Number_of_Vol_Pents_5 > 0)
    {
      Number_of_Bytes =  Number_of_Vol_Pents_5 * ((INT_) (sizeof (INT_)));

      for (j = 0; j < 5; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	k = pen5_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
	  Write_Flag = Write_Flag + ug_fwrite (&Vol_Pent_5_Connectivity[Index][k],
					       sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Vol_Pents_5 + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
        {
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* 6-Pentahedra */
    if (Number_of_Vol_Pents_6 > 0)
    {
      Number_of_Bytes =  Number_of_Vol_Pents_6 * ((INT_) (sizeof (INT_)));

      for (j = 0; j < 6; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	k = pen6_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
	  Write_Flag = Write_Flag + ug_fwrite (&Vol_Pent_6_Connectivity[Index][k],
					       sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Vol_Pents_6 + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* Hexahedra */
    if (Number_of_Vol_Hexs > 0)
    {
      Number_of_Bytes =  Number_of_Vol_Hexs * ((INT_) (sizeof (INT_)));

      for (j = 0; j < 8; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	k = hex8_conn[j];
	for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
	  Write_Flag = Write_Flag + ug_fwrite (&Vol_Hex_Connectivity[Index][k],
					       sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Vol_Hexs + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* Coordinates */
    if (Number_of_Nodes > 0)
    {
      Number_of_Bytes =  Number_of_Nodes * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
					    ? ((INT_) (sizeof (double)))
					    : ((INT_) (sizeof (float))));

      for (j = 0; j < 3; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
	    File_Format == UG_IO_BINARY_SINGLE)
	{
	  for (Index = 1; Index <= Number_of_Nodes; ++Index)
	  {
	    TMP_float = (float) Coordinates[Index][j];

	    Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
						 sizeof (float), 1, Grid_File);
	  }
	}
	else
	{
	  for (Index = 1; Index <= Number_of_Nodes; ++Index)
	    Write_Flag = Write_Flag + ug_fwrite (&Coordinates[Index][j],
						 sizeof (double), 1, Grid_File);
	}

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Nodes + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* Triangles */
    if (Number_of_Surf_Trias > 0)
    {
      Number_of_Bytes =  Number_of_Surf_Trias * ((INT_) (sizeof (INT_)));

      for (j = 0; j < 4; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
	{
	  TMP_int = (j < 3) ? Surf_Tria_Connectivity[Index][j] : 0;

	  Write_Flag = Write_Flag + ug_fwrite (&TMP_int,
					       sizeof (INT_), 1, Grid_File);
	}

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Surf_Trias + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }


    /* Quadrilaterals */
    if (Number_of_Surf_Quads > 0)
    {
      Number_of_Bytes =  Number_of_Surf_Quads * ((INT_) (sizeof (INT_)));

      for (j = 0; j < 5; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
	{
	  TMP_int = (j < 4) ? Surf_Quad_Connectivity[Index][j] : 0;

	  Write_Flag = Write_Flag + ug_fwrite (&TMP_int,
					       sizeof (INT_), 1, Grid_File);
	}

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Surf_Quads + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* Triangle Surface IDs */
    if (Number_of_Surf_Trias > 0)
    {
      Number_of_Bytes =  Number_of_Surf_Trias * ((INT_) (sizeof (INT_)));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	  File_Format == UG_IO_UNFORMATTED_SINGLE)
	Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					     sizeof (INT_), 1, Grid_File);

      for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
      {
	Write_Flag = Write_Flag + ug_fwrite (&Surf_ID_Flag[Index],
					     sizeof (INT_), 1, Grid_File);
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	  File_Format == UG_IO_UNFORMATTED_SINGLE)
	Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					     sizeof (INT_), 1, Grid_File);

      Number_of_Write_Items = Number_of_Surf_Trias + 2 * Special_Write_Items;

      if (Write_Flag != Number_of_Write_Items)
      {
	strcpy (Error_Message, "error writing NSU3D grid file");
	return (1);
      }

      Number_of_Bytes =  Number_of_Surf_Trias * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
						 ? ((INT_) (sizeof (double)))
						 : ((INT_) (sizeof (float))));
      
      for (j = 0; j < 6; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
	    File_Format == UG_IO_BINARY_SINGLE)
	{
	  TMP_float = 0.0;
	  for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
	    Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
						 sizeof (float), 1, Grid_File);
	}
	else
	{
	  TMP_double = 0.0;
	  for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
	    Write_Flag = Write_Flag + ug_fwrite (&TMP_double,
						 sizeof (double), 1, Grid_File);
	}

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Surf_Trias + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* Quad Surface IDs */
    if (Number_of_Surf_Quads > 0)
    {
      Number_of_Bytes =  Number_of_Surf_Quads * ((INT_) (sizeof (INT_)));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	  File_Format == UG_IO_UNFORMATTED_SINGLE)
	Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

      for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
      {
	Write_Flag = Write_Flag + ug_fwrite (&Surf_ID_Flag[Index+Number_of_Surf_Trias],
					     sizeof (INT_), 1, Grid_File);
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	  File_Format == UG_IO_UNFORMATTED_SINGLE)
	Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					     sizeof (INT_), 1, Grid_File);

      Number_of_Write_Items = Number_of_Surf_Quads + 2 * Special_Write_Items;

      if (Write_Flag != Number_of_Write_Items)
      {
	strcpy (Error_Message, "error writing NSU3D grid file");
	return (1);
      }

      Number_of_Bytes =  Number_of_Surf_Quads * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
						 ? ((INT_) (sizeof (double)))
						 : ((INT_) (sizeof (float))));

      for (j = 0; j < 8; j++)
      {
	Write_Flag = 0;

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
	    File_Format == UG_IO_BINARY_SINGLE)
	{
	  TMP_float = 0.0;
	  for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
	    Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
						 sizeof (float), 1, Grid_File);
	}
	else
	{
	  TMP_double = 0.0;
	  for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
	    Write_Flag = Write_Flag + ug_fwrite (&TMP_double,
						 sizeof (double), 1, Grid_File);
	}

	if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	    File_Format == UG_IO_UNFORMATTED_SINGLE)
	  Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					       sizeof (INT_), 1, Grid_File);

	Number_of_Write_Items = Number_of_Surf_Quads + 2 * Special_Write_Items;

	if (Write_Flag != Number_of_Write_Items)
	{
	  strcpy (Error_Message, "error writing NSU3D grid file");
	  return (1);
	}
      }
    }

    /* Component definition */
    if (Number_of_Patches > 0)
    {
      Number_of_Bytes =  Number_of_Patches * ((INT_) (sizeof (INT_)));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	  File_Format == UG_IO_UNFORMATTED_SINGLE)
	Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					     sizeof (INT_), 1, Grid_File);

      TMP_int = 1;
      for (Index = 1; Index <= Number_of_Patches; ++Index)
	Write_Flag = Write_Flag + ug_fwrite (&TMP_int,
					     sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
	  File_Format == UG_IO_UNFORMATTED_SINGLE)
	Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
					     sizeof (INT_), 1, Grid_File);

      Number_of_Write_Items = Number_of_Patches + 2 * Special_Write_Items;

      if (Write_Flag != Number_of_Write_Items)
      {
	strcpy (Error_Message, "error writing NSU3D grid file");
	return (1);
      }
    }

    /* Body definition */
    Number_of_Bytes =  2 * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);
    TMP_int = 1;
    Write_Flag = Write_Flag + ug_fwrite (&TMP_int,
                                         sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + ug_fwrite (&TMP_int,
                                         sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = 2 + 2 * Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing NSU3D grid file");
      return (1);
    }
  }

  return (0);

}
