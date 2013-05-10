#include "UG_IO_LIB.h"

INT_ ug_io_write_cobalt
 (FILE * Grid_File,
  char Error_Message[],
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
 * Write grid data to a big endian unformatted real*8 COBALT volume grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_cobalt.c,v 1.13 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ i2, ndm, nitems, npatch, nzones, mxppf, mxfpc,
       Error_Flag, Index, Max_SurfID, Number_of_Bytes, Number_of_Cells, 
       Number_of_Faces, Number_of_Quad_Faces, Number_of_Tria_Faces, Write_Flag,
       Number_of_Surfaces;
  INT_ tmp_array[7];

  INT_1D *Groups = NULL;
  INT_5D *Tria_Faces = NULL;
  INT_6D *Quad_Faces = NULL;
  
  Error_Flag = ug3_extract_faces_from_volume_elements
		(Number_of_Surf_Quads,
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
		 &Number_of_Tria_Faces,
		 &Number_of_Quad_Faces,
		 &Number_of_Surfaces,
		 &Groups,
		 &Tria_Faces,
		 &Quad_Faces);

  if(Error_Flag){
    strcpy (Error_Message, "unable to extract faces for COBALT grid file");
    return (1);
  }

  Max_SurfID = 0;
  Error_Flag = 0;
  Write_Flag = 0;

  Number_of_Faces = Number_of_Tria_Faces + Number_of_Quad_Faces;

  Number_of_Cells = Number_of_Vol_Tets
                  + Number_of_Vol_Pents_5 + Number_of_Vol_Pents_6
                  + Number_of_Vol_Hexs;

  ndm = 3;
  npatch = Number_of_Surfaces;
  nzones = 1;
  mxppf = 4;
  mxfpc = 6;

  /* Write the header information */
  nitems = 3;
  Number_of_Bytes = nitems * ((INT_) (sizeof (INT_)));
  tmp_array[0] = ndm;
  tmp_array[1] = nzones;
  tmp_array[2] = npatch;
  Write_Flag = Write_Flag
             + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
  Write_Flag = Write_Flag
             + ug_fwrite (tmp_array, sizeof(INT_), nitems, Grid_File);
  Write_Flag = Write_Flag
             + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
  Write_Flag = Write_Flag - nitems - 2;

  nitems = 5;
  Number_of_Bytes = nitems * ((INT_) (sizeof (INT_)));
  tmp_array[0] = Number_of_Nodes;
  tmp_array[1] = Number_of_Faces;
  tmp_array[2] = Number_of_Cells;
  tmp_array[3] = mxppf;
  tmp_array[4] = mxfpc;
  Write_Flag = Write_Flag
             + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
  Write_Flag = Write_Flag
             + ug_fwrite (tmp_array, sizeof(INT_), nitems, Grid_File);
  Write_Flag = Write_Flag
             + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
  Write_Flag = Write_Flag - nitems - 2;

  /* Write the coordinates */
  nitems = 3;
  Number_of_Bytes = nitems * ((INT_) (sizeof (double)));
  for (Index = 1; Index <= Number_of_Nodes; ++Index)
  {
    Write_Flag = Write_Flag
               + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
    Write_Flag = Write_Flag
               + ug_fwrite (&Coordinates[Index][0], sizeof (double), nitems, Grid_File);
    Write_Flag = Write_Flag
               + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
    Write_Flag = Write_Flag - nitems - 2;
  }

  /* Write the face information */
  nitems = 6;
  Number_of_Bytes = nitems * ((INT_) (sizeof (INT_)));
  tmp_array[0] = 3;
  for (Index = 0; Index < Number_of_Tria_Faces; ++Index)
  {
    i2 = Index*2;
    if(Tria_Faces[i2][0] != Tria_Faces[i2+1][0] ||
       Tria_Faces[i2][1] != Tria_Faces[i2+1][1] ||
       Tria_Faces[i2][2] != Tria_Faces[i2+1][2] ||
       (Tria_Faces[i2][3] < 0 && Tria_Faces[i2+1][3] < 0))
    {
      strcpy (Error_Message, "unable to match tria faces in COBALT grid data");
      ug_free (Groups);
      ug_free (Tria_Faces);
      ug_free (Quad_Faces);
      return (1);
    }

    tmp_array[1] = Tria_Faces[i2][0];
    tmp_array[2] = Tria_Faces[i2][1];
    tmp_array[3] = Tria_Faces[i2][2];
    if (Tria_Faces[i2][4])
    {
      tmp_array[4] = Tria_Faces[i2+1][3];
      tmp_array[5] = Tria_Faces[i2][3];
    }
    else
    {
      tmp_array[4] = Tria_Faces[i2][3];
      tmp_array[5] = Tria_Faces[i2+1][3];
    }

    Write_Flag = Write_Flag
               + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
    Write_Flag = Write_Flag
               + ug_fwrite (tmp_array, sizeof(INT_), nitems, Grid_File);
    Write_Flag = Write_Flag
               + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
    Write_Flag = Write_Flag - nitems - 2;
  }

  nitems = 7;
  Number_of_Bytes = nitems * ((INT_) (sizeof (INT_)));
  tmp_array[0] = 4;
  for (Index = 0; Index < Number_of_Quad_Faces; ++Index)
  {
    i2 = Index*2;
    if(Quad_Faces[i2][0] != Quad_Faces[i2+1][0] ||
       Quad_Faces[i2][1] != Quad_Faces[i2+1][1] ||
       Quad_Faces[i2][2] != Quad_Faces[i2+1][2] ||
       Quad_Faces[i2][3] != Quad_Faces[i2+1][3] ||
       (Quad_Faces[i2][4] < 0 && Quad_Faces[i2+1][4] < 0))
    {
      strcpy (Error_Message, "unable to match quad faces in COBALT grid data");
      ug_free (Groups);
      ug_free (Tria_Faces);
      ug_free (Quad_Faces);
      return (1);
    }
    tmp_array[1] = Quad_Faces[i2][0];
    tmp_array[2] = Quad_Faces[i2][1];
    tmp_array[3] = Quad_Faces[i2][2];
    tmp_array[4] = Quad_Faces[i2][3];
    if (Quad_Faces[i2][5])
    {
      tmp_array[5] = Quad_Faces[i2+1][4];
      tmp_array[6] = Quad_Faces[i2][4];
    }
    else
    {
      tmp_array[5] = Quad_Faces[i2][4];
      tmp_array[6] = Quad_Faces[i2+1][4];
    }

    Write_Flag = Write_Flag
               + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
    Write_Flag = Write_Flag
               + ug_fwrite (tmp_array, sizeof(INT_), nitems, Grid_File);
    Write_Flag = Write_Flag
               + ug_fwrite (&Number_of_Bytes, sizeof(INT_), 1, Grid_File);
    Write_Flag = Write_Flag - nitems - 2;
  }

  /* Free working memory */
  ug_free (Groups);
  ug_free (Tria_Faces);
  ug_free (Quad_Faces);

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing COBALT grid file");
    return(1);
  }

  return (0);
}
