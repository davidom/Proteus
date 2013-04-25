#include "UG_IO_LIB.h"

/* Include the defines for the FV_* codes and wall_info values. */
#include "fv_reader_tags.h"

/* Don't change these - used by fv_encode_elem_header ! */
#define MAX_NUM_ELEM_FACES     6
#define BITS_PER_WALL  3
#define ELEM_TYPE_BIT_SHIFT    (MAX_NUM_ELEM_FACES*BITS_PER_WALL)

#ifdef __STDC__
size_t fwrite_str80 
#else
int fwrite_str80
#endif
 (char *, 
  FILE *);

unsigned int fv_encode_elem_header 
 (INT_ ,
  INT_1D *);

INT_ ug_io_write_fvuns
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
 * Write grid data to a Fieldview volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_fvuns.c,v 1.3 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ j, k, Index, Number_of_Patches, Number_of_Surf_Faces, Number_of_Vols,
       Write_Flag;
  INT_ tet4_conn[4] = {0,1,2,3};
  INT_ pen5_conn[5] = {1,0,3,4,2};
  INT_ pen6_conn[6] = {4,1,0,3,2,5};  // AFLR -> FV: FVReference_Manual 12.01 pg. 505
  INT_ hex8_conn[8] = {0,1,2,3,4,5,6,7};

  float TMP_float;

  //INT_1D *Surf_ID_Flag_ = NULL;

  INT_ Number_of_Grids, grid, patch, nface, nvert, ntype;
  INT_ ibuf[10];
  unsigned int elem_header;
  char face_name[80] = "patch    ";

  Number_of_Patches = 0;
  Number_of_Grids   = 1;

  Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;
  Number_of_Vols       = Number_of_Vol_Tets + 
                         Number_of_Vol_Hexs +
                         Number_of_Vol_Pents_6 +
                         Number_of_Vol_Pents_5;

  Number_of_Patches = ug_max_int (1, Number_of_Surf_Faces, Surf_ID_Flag);
#if 0
  Error_Flag = 0;
  Surf_ID_Flag_ = (INT_1D *) ug_malloc (&Error_Flag, 
				       (Number_of_Surf_Faces + 1) * sizeof (INT_1D));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc Fieldview I/O work array");
    return (1);
  }

  for (Index = 1; Index <= Number_of_Surf_Faces; Index++)
    Surf_ID_Flag_[Index] = Surf_ID_Flag[Index];

  if (ug_io_count_number_of_groups (Error_Message,
				    Number_of_Surf_Trias,
				    Number_of_Surf_Quads,
				    &Number_of_Patches,
				    Surf_ID_Flag_))
  {
    ug_free (Surf_ID_Flag_);
    return (1);
  }
#endif
  if (File_Format == UG_IO_FORMATTED)
  {
    /* Header information */

/*  Output file header and version number.
 *  This version of the FIELDVIEW unstructured file is "3.0".
 *  The tag 'FIELDVIEW' must be in upper case characters and
 *  must start in the first column.
 *  Case and start column are optional for all other tags.

 *  grids only file is identified with the tag 'FIELDVIEW_Grids'.
 *  results only file is dentified with the tag 'FIELDVIEW_Results'. */
    Write_Flag = fprintf (Grid_File, "FIELDVIEW_Grids 3 0\n");

    /* Output the number of grids. */
    Write_Flag = fprintf(Grid_File, "Grids %i\n", Number_of_Grids);

/*  Output the table of boundary types, starting with the number of types.
 *  Note that this differs from the binary/unformatted specification.
 *  Each boundary type name is preceded by 3 integer flags.
 *  The first flag indicates whether this boundary type is a wall.
 *  A flag value of 1 indicates a wall, and a value of 0 indicates
 *  a non-wall.  Walls are significant for streamline calculation.
 *  The second flag indicates whether the boundary type has surface
 *  results.  A value of 1 means surface results will be present for
 *  this boundary type (if any boundary variables are specified in the
 *  Boundary Variable Names section below).  A value of 0 means no surface
 *  results will be present.
 *  The third flag indicates whether boundary faces of this type have
 *  consistent "clockness" for the purpose of calculating a surface
 *  normal.  A value of 1 means that all faces of this type are
 *  written following a "right hand rule" for clockness.  In other
 *  words, if the vertices are written on counter-clockwise:
 *         4 --- 3
 *         |     |
 *         1 --- 2
 *  then the normal to the face is pointing towards you (not away
 *  from you).  A value of 0 means that the faces do not have any
 *  consistent clockness.  The "clockness" of surface normals is
 *  only used for calculating certain special surface integrals
 *  that involve surface normals.  If the surface normals flag
 *  is 0, these special integrals will not be available. */
    Write_Flag = fprintf(Grid_File, "Boundary Table %i\n", Number_of_Patches);

    /* Boundary surface data */

    Write_Flag = 0;

    for (patch = 1; patch <= Number_of_Patches; patch++)
    {
      /* wall, results_flag, clockness */
      sprintf(face_name+7,"%i",patch);
      Write_Flag = fprintf(Grid_File, "%i %i %i %s\n", 0,0,1, face_name);
    }

    /* Output grid data. */

    for (grid = 0; grid < Number_of_Grids; grid++)
    {

      /* Output the node definition section for this grid. */
      Write_Flag = fprintf(Grid_File, "Nodes %i\n", Number_of_Nodes);

      /* Coordinates */
      if (Number_of_Nodes > 0)
      {

/*      Output the X, Y, Z coordinates of successive nodes.
 *      Note that this differs from the binary/unformatted specification. */
        Write_Flag = 0;
        for (Index = 1; Index <= Number_of_Nodes; Index++)
           Write_Flag = Write_Flag + fprintf(Grid_File, "%f %f %f\n", 
                                             (float) Coordinates[Index][0],
                                             (float) Coordinates[Index][1],
                                             (float) Coordinates[Index][2]);
      }

/*    Output boundary faces of 3 different types.
 *    Note that this differs from the binary/unformatted specification.
 *    Each face is preceded by an index into the boundary table at the
 *    top of the file and the number of face vertices, 3 or 4.
 *    TIP: FIELDVIEW assumes that boundary faces are not in random
 *    order.  It assumes that faces of the same type tend to occur
 *    in groups.  If your boundary faces are in random order, you
 *    may want to output them one boundary type at a time.  This
 *    will give you better performance (less memory, greater speed)
 *    in FIELDVIEW. */
/*    NOTE: FV Reference Guide says nvert is always 4, but this is only
 *    true for unformatted */
      Write_Flag = fprintf(Grid_File, "Boundary Faces %i\n", Number_of_Surf_Faces);

      for (patch = 1; patch <= Number_of_Patches; patch++)
      {

        /* Triangles */
        if (Number_of_Surf_Trias > 0)
        {
          nvert = 3;
          Write_Flag = 0;
          for (Index = 1; Index <= Number_of_Surf_Trias; Index++)
          {
            if (Surf_ID_Flag[Index] == patch)
            {
              Write_Flag = fprintf(Grid_File, "%i %i", patch, nvert);
              for (j = 0; j < nvert; j++)
                Write_Flag = Write_Flag + fprintf(Grid_File, " %i",
                             Surf_Tria_Connectivity[Index][j]);
	      Write_Flag = fprintf(Grid_File, "\n");
            }
          }
        }

        /* Quadrilaterals */
        if (Number_of_Surf_Quads > 0)
        {
          nvert = 4;
          Write_Flag = 0;
          for (Index = 1; Index <= Number_of_Surf_Quads; Index++)
          {
            if ( Surf_ID_Flag[Number_of_Surf_Trias + Index] == patch )
            {
              Write_Flag = fprintf(Grid_File, "%i %i", patch, nvert);
              for (j = 0; j < nvert; j++)
                Write_Flag = Write_Flag + fprintf(Grid_File, " %i",
                             Surf_Quad_Connectivity[Index][j]);
	      Write_Flag = fprintf(Grid_File, "\n");
            }
          }
        }
      }

/*    Output the elements section for this grid.
 *    Note that this differs from the binary/unformatted specification.
 *    It contains the headers and node definitions of all elements.
 *    In this example, each element starts with 2 for type 'hex',
 *    with a subtype of 1 (the only subtype currently supported).
 *    This is followed by the node indices for the element. */
      Write_Flag = fprintf(Grid_File, "Elements\n");

      /* Tetrahedra */
      if (Number_of_Vol_Tets > 0)
      {
        nvert = 4;
        ntype = 1;
        Write_Flag = 0;
        for (Index = 1; Index <= Number_of_Vol_Tets; Index++)
        {
	  Write_Flag = Write_Flag + fprintf(Grid_File, "%i %i", ntype,1);
          for (j = 0; j < nvert; j++)
          {
	    k = tet4_conn[j];
	    Write_Flag = Write_Flag + fprintf(Grid_File, " %i",
                                              Vol_Tet_Connectivity[Index][k]);
          }
	  Write_Flag = fprintf(Grid_File, "\n");
        }
      }

      /* Hexahedra */
      if (Number_of_Vol_Hexs > 0)
      {
        nvert = 8;
        ntype = 2;
        Write_Flag = 0;
        for (Index = 1; Index <= Number_of_Vol_Hexs; Index++)
        {
	  Write_Flag = Write_Flag + fprintf(Grid_File, "%i %i", ntype,1);
          for (j = 0; j < nvert; j++)
          {
	    k = hex8_conn[j];
	    Write_Flag = Write_Flag + fprintf(Grid_File, " %i",
                                              Vol_Hex_Connectivity[Index][k]);
          }
	  Write_Flag = fprintf(Grid_File, "\n");
        }
      }

      /* 6-Pentahedra - Prism */
      if (Number_of_Vol_Pents_6 > 0)
      {
        nvert = 6;
        ntype = 3;
        Write_Flag = 0;
        for (Index = 1; Index <= Number_of_Vol_Pents_6; Index++)
        {
	  Write_Flag = Write_Flag + fprintf(Grid_File, "%i %i", ntype,1);
          for (j = 0; j < nvert; j++)
          {
 	    k = pen6_conn[j];
	    Write_Flag = Write_Flag + fprintf(Grid_File, " %i",
                                              Vol_Pent_6_Connectivity[Index][k]);
          }
	  Write_Flag = fprintf(Grid_File, "\n");
        }
      }

      /* 5-Pentahedra - Pyramid */
      if (Number_of_Vol_Pents_5 > 0)
      {
        nvert = 5;
        ntype = 4;
        Write_Flag = 0;
        for (Index = 1; Index <= Number_of_Vol_Pents_5; Index++)
        {
	  Write_Flag = Write_Flag + fprintf(Grid_File, "%i %i", ntype,1);
          for (j = 0; j < nvert; j++)
          {
 	    k = pen5_conn[j];
	    Write_Flag = Write_Flag + fprintf(Grid_File, " %i",
                                              Vol_Pent_5_Connectivity[Index][k]);
          }
	  Write_Flag = fprintf(Grid_File, "\n");
        }
      }

      /* Dummy element for surface mesh only */
      if (Number_of_Vols == 0)
	Write_Flag = fprintf(Grid_File, "%i %i %i %i %i %i\n", 1,1,1,1,1,1);

    } /* for (grid = 0; grid < Number_of_Grids; grid++) */
  }

  else if (File_Format == UG_IO_BINARY_SINGLE) 
  {
    /* Header information */

    Write_Flag = 0;

    /* Output the magic number. */
    ibuf[0] = FV_MAGIC;
    Write_Flag = Write_Flag + ug_fwrite (ibuf, sizeof(INT_), 1, Grid_File);

    /* Output file header and version number. */
    fwrite_str80("FIELDVIEW", Grid_File);
  
    /* This version of the FIELDVIEW unstructured file is "3.0".
    ** This is written as two integers. */
    ibuf[0] = 3;
    ibuf[1] = 0;
    Write_Flag = Write_Flag + ug_fwrite (ibuf, sizeof(INT_), 2, Grid_File);

    /* File type code - new in version 2.7 */
    ibuf[0] = FV_GRIDS_FILE;
    Write_Flag = Write_Flag + ug_fwrite (ibuf, sizeof(INT_), 1, Grid_File);

    /* Reserved field, always write a zero - new in version 2.6 */
    ibuf[0] = 0;
    Write_Flag = Write_Flag + ug_fwrite (ibuf, sizeof(INT_), 1, Grid_File);

    /* Output the number of grids. */
    ibuf[0] = Number_of_Grids;
    Write_Flag = Write_Flag + ug_fwrite(ibuf, sizeof(INT_), 1, Grid_File);

    /* Output grid data. */

    for (grid = 0; grid < Number_of_Grids; grid++)
    {

      /* Output the table of boundary types.
      ** Each boundary type is preceded by 2 integer flags.
      ** The first flag is an "surface results flag".
      ** A value of 1 means surface results will be present for this
      ** boundary type (if any boundary variables are specified in the
      ** boundary variable names table below).
      ** A value of 0 means no surface results will be present.
      ** The second flag indicates whether boundary faces of this type have
      ** consistent "clockness" for the purpose of calculating a surface
      ** normal.  A value of 1 means that all faces of this type are
      ** written following a "right hand rule" for clockness.  In other
      ** words, if the vertices are written on counter-clockwise:
      ** 4 --- 3
      ** |     |
      ** 1 --- 2
      ** then the normal to the face is pointing towards you (not away
      ** from you).  A value of 0 means that the faces do not have any
      ** consistent clockness.  The "clockness" of surface normals is
      ** only used for calculating certain special surface integrals
      ** that involve surface normals.  If the surface normals flag
      ** is 0, these special integrals will not be available. */

      /* Boundary surface data */

      Write_Flag = 0;

      /* Boundary surface flags */
      ibuf[0] = Number_of_Patches;
      Write_Flag = Write_Flag + ug_fwrite (ibuf, sizeof(INT_), 1, Grid_File);

      for (patch = 1; patch <= Number_of_Patches; patch++)
      {
        ibuf[0] = 0;  /* results_flag */
	ibuf[1] = 1;  /* clockness */
        Write_Flag = Write_Flag + ug_fwrite (ibuf, sizeof(INT_), 2, Grid_File);
        sprintf(face_name+7,"%i",patch);
        fwrite_str80(face_name, Grid_File);
      }

      /* Output the node definition section for this grid. */
      ibuf[0] = FV_NODES;
      ibuf[1] = Number_of_Nodes;
      Write_Flag = Write_Flag + ug_fwrite (ibuf, sizeof(INT_), 2, Grid_File);

      /* Coordinates */
      if (Number_of_Nodes > 0)
      {

        /* Output the X, then Y, then Z node coordinates.
        ** Note that all of the X coordinates are output before any of
        ** the Y coordinates. */
        for (j = 0; j < 3; j++)
        {
  	  Write_Flag = 0;
    
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
        }
      }

      /* Output boundary faces of the 3 different types.
      ** All faces have 4 vertices.  If the face is triangular,
      ** the last vertex should be zero.
      ** TIP: A single boundary type can be broken into several sections
      ** if you prefer.  Also, boundary face sections do not have to
      ** be in order.  You may have a section of 10 faces of type 3,
      ** followed by a section of 20 faces of type 2, followed by a
      ** section of 15 more faces of type 3.  Breaking a boundary
      ** type into very many short sections is less efficient.  The
      ** boundaries will require more memory and be somewhat
      ** slower to calculate in FIELDVIEW. */

      for ( patch = 1; patch <= Number_of_Patches; patch++ )
      {

        /* Triangles */
        if (Number_of_Surf_Trias > 0)
        {

          Write_Flag = 0;

          nface = 0;
          for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
            if ( Surf_ID_Flag[Index] == patch ) nface++;
  
          ibuf[0] = FV_FACES;
          ibuf[1] = patch;  /* boundary type */
          ibuf[2] = nface;  /* number of tri faces of this type */
          Write_Flag = Write_Flag + ug_fwrite (ibuf,
                                               sizeof (INT_), 3, Grid_File);

          for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
          {
            if ( Surf_ID_Flag[Index] == patch )
            {
              ibuf[0] = Surf_Tria_Connectivity[Index][0];
              ibuf[1] = Surf_Tria_Connectivity[Index][1];
              ibuf[2] = Surf_Tria_Connectivity[Index][2];
              ibuf[3] = 0;
              Write_Flag = Write_Flag + ug_fwrite (ibuf,
                                                   sizeof (INT_), 4, Grid_File);
            }
          }
        }

        /* Quadrilaterals */
        if (Number_of_Surf_Quads > 0)
        {
  
          Write_Flag = 0;

          nface = 0;
          for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
            if ( Surf_ID_Flag[Number_of_Surf_Trias + Index] == patch ) nface++;
//        printf("%i QUAD %i\n",patch,nface);

          ibuf[0] = FV_FACES;
          ibuf[1] = patch;  /* boundary type */
          ibuf[2] = nface;  /* number of quad faces of this type */
          Write_Flag = Write_Flag + ug_fwrite (ibuf,
                                               sizeof (INT_), 3, Grid_File);

          for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
          {
            if ( Surf_ID_Flag[Number_of_Surf_Trias + Index] == patch )
            {
              ibuf[0] = Surf_Quad_Connectivity[Index][0];
              ibuf[1] = Surf_Quad_Connectivity[Index][1];
              ibuf[2] = Surf_Quad_Connectivity[Index][2];
              ibuf[3] = Surf_Quad_Connectivity[Index][3];
              Write_Flag = Write_Flag + ug_fwrite (ibuf,
  	                                           sizeof (INT_), 4, Grid_File);
            }
          }
        }
      }

      /* Start an elements section.
      ** There may be as many elements sections as needed.
      ** Each section may contain a single element type or a
      ** mixture of element types.
      ** For maximum efficiency, each section should contain
      ** a significant percentage of the elements in the grid.
      ** The most efficient case is a single section containing
      ** all elements in the grid. */

      ibuf[0] = FV_ELEMENTS;
      ibuf[1] = Number_of_Vol_Tets;     /* tet count */
      if (Number_of_Vols == 0) ibuf[1] = 1; /* no element dummy case */
      ibuf[2] = Number_of_Vol_Hexs;     /* hex count */
      ibuf[3] = Number_of_Vol_Pents_6;  /* prism count */
      ibuf[4] = Number_of_Vol_Pents_5;  /* pyramid count */
      Write_Flag = Write_Flag + ug_fwrite (ibuf,
					   sizeof (INT_), 5, Grid_File);

      /* Tetrahedra */
      if (Number_of_Vol_Tets > 0)
      {
        nvert = 4;
        ibuf[0] = NOT_A_WALL;
        ibuf[1] = NOT_A_WALL;
        ibuf[2] = NOT_A_WALL;
        ibuf[3] = NOT_A_WALL;
        elem_header = fv_encode_elem_header(FV_TET_ELEM_ID, ibuf);
        if(elem_header== 0)
        {
            strcpy (Error_Message, "invalid element type and/or wall value for Fieldview unstructured grid file");
            return (1);
        }

        for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
        {
	  Write_Flag = Write_Flag + ug_fwrite (&elem_header,
					       sizeof (elem_header), 1, Grid_File);
          for (j = 0; j < nvert; j++)
          {
	    k = tet4_conn[j];
            ibuf[j] = Vol_Tet_Connectivity[Index][k];
          }

	  Write_Flag = Write_Flag + ug_fwrite (ibuf,
	                                       sizeof (INT_), 4, Grid_File);
        }
      }

      /* Hexahedra */
      if (Number_of_Vol_Hexs > 0)
      {
        nvert = 8;
        ibuf[0] = NOT_A_WALL;
        ibuf[1] = NOT_A_WALL;
        ibuf[2] = NOT_A_WALL;
        ibuf[3] = NOT_A_WALL;
        ibuf[4] = NOT_A_WALL;
        ibuf[5] = NOT_A_WALL;
        elem_header = fv_encode_elem_header(FV_HEX_ELEM_ID, ibuf);
        if(elem_header== 0)
        {
            strcpy (Error_Message, "invalid element type and/or wall value for Fieldview unstructured grid file");
            return (1);
        }

        for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
        {
	  Write_Flag = Write_Flag + ug_fwrite (&elem_header,
					       sizeof (elem_header), 1, Grid_File);
          for (j = 0; j < nvert; j++)
          {
	    k = hex8_conn[j];
            ibuf[j] = Vol_Hex_Connectivity[Index][k];
          }

	  Write_Flag = Write_Flag + ug_fwrite (ibuf,
					       sizeof (INT_), nvert, Grid_File);
        }
      }

      /* 5-Pentahedra - Pyramid */
      if (Number_of_Vol_Pents_5 > 0)
      {
        nvert = 5;
        ibuf[0] = NOT_A_WALL;
        ibuf[1] = NOT_A_WALL;
        ibuf[2] = NOT_A_WALL;
        ibuf[3] = NOT_A_WALL;
        ibuf[4] = NOT_A_WALL;
        elem_header = fv_encode_elem_header(FV_PYRA_ELEM_ID, ibuf);
        if(elem_header== 0)
        {
            strcpy (Error_Message, "invalid element type and/or wall value for Fieldview unstructured grid file");
            return (1);
        }

        for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
        {
	  Write_Flag = Write_Flag + ug_fwrite (&elem_header,
					       sizeof (elem_header), 1, Grid_File);
          for (j = 0; j < nvert; j++)
          {
 	    k = pen5_conn[j];
            ibuf[j] = Vol_Pent_5_Connectivity[Index][k];
          }

	  Write_Flag = Write_Flag + ug_fwrite (ibuf,
					       sizeof (INT_), nvert, Grid_File);
        }
      }

      /* 6-Pentahedra - Prism */
      if (Number_of_Vol_Pents_6 > 0)
      {
        nvert = 6;
        ibuf[0] = NOT_A_WALL;
        ibuf[1] = NOT_A_WALL;
        ibuf[2] = NOT_A_WALL;
        ibuf[3] = NOT_A_WALL;
        ibuf[4] = NOT_A_WALL;
        elem_header = fv_encode_elem_header(FV_PRISM_ELEM_ID, ibuf);
        if(elem_header== 0)
        {
            strcpy (Error_Message, "invalid element type and/or wall value for Fieldview unstructured grid file");
            return (1);
        }

        for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
        {
	  Write_Flag = Write_Flag + ug_fwrite (&elem_header,
					       sizeof (elem_header), 1, Grid_File);
          for (j = 0; j < nvert; j++)
          {
 	    k = pen6_conn[j];
            ibuf[j] = Vol_Pent_6_Connectivity[Index][k];
          }

	  Write_Flag = Write_Flag + ug_fwrite (ibuf,
					       sizeof (INT_), nvert, Grid_File);
        }
      }

      /* Dummy tet element for surface mesh only */
      if (Number_of_Vols == 0)
      {
        nvert = 4;
        ibuf[0] = NOT_A_WALL;
        ibuf[1] = NOT_A_WALL;
        ibuf[2] = NOT_A_WALL;
        ibuf[3] = NOT_A_WALL;
        elem_header = fv_encode_elem_header(FV_TET_ELEM_ID, ibuf);
        if(elem_header== 0)
        {
            strcpy (Error_Message, "invalid element type and/or wall value for Fieldview unstructured grid file");
            return (1);
        }
	Write_Flag = Write_Flag + ug_fwrite (&elem_header,
	 		                     sizeof (elem_header), 1, Grid_File);
        ibuf[0] = 1;
        ibuf[1] = 1;
        ibuf[2] = 1;
        ibuf[3] = 1;
	Write_Flag = Write_Flag + ug_fwrite (ibuf,
	                                     sizeof (INT_), nvert, Grid_File);
      }

    } /* for (grid = 0; grid < Number_of_Grids; grid++) */
  }

  else
  {
    strcpy (Error_Message, "File format not currently supported for Fieldview unstructured");
    //ug_free (Surf_ID_Flag_);
    return (1);
  }

  //ug_free (Surf_ID_Flag_);

  return (0);

}

/*
** Support functions for writing separate grid and results files
** in the binary FIELDVIEW unstructured format.
*/

/*
** fv_encode_elem_header:  return an encoded binary element header
**
** Input:
**    elem_type:  integer element type as shown in fv_reader_tags.h
**    wall_info:  array of integer "wall" flags, one for each face of
**                the element.  The wall flags are used during streamline
**                calculation.  Currently, the only meaningful values are
**                A_WALL and NOT_A_WALL as shown in fv_reader_tags.h.
**                Streamlines are forced away from faces marked as
**                "A_WALL", by limiting velocity and position very near
**                the wall.
** Output:
**    Function return value is the encoded binary element header.
*/

#ifdef __STDC__
unsigned int fv_encode_elem_header (int elem_type, int wall_info[])
#else
unsigned int fv_encode_elem_header (elem_type, wall_info)
int elem_type;
int wall_info[];
#endif
{
    unsigned int header;
    int i, nfaces;

    switch (elem_type)
    {
        case FV_TET_ELEM_ID:
            header = (1 << ELEM_TYPE_BIT_SHIFT);
            nfaces = 4;
            break;
        case FV_HEX_ELEM_ID:
            header = (4 << ELEM_TYPE_BIT_SHIFT);
            nfaces = 6;
            break;
        case FV_PRISM_ELEM_ID:
            header = (3 << ELEM_TYPE_BIT_SHIFT);
            nfaces = 5;
            break;
        case FV_PYRA_ELEM_ID:
            header = (2 << ELEM_TYPE_BIT_SHIFT);
            nfaces = 5;
            break;
        default:
            //fprintf(stderr, "ERROR:  Unknown element type\n");
            return 0;
    }

    for (i = 0; i < nfaces; i++)
    {
        unsigned int u = wall_info[i];
        if (u > A_WALL)
        {
            //fprintf(stderr, "ERROR:  Bad wall value\n");
            return 0;
        }
        header |= (u << (i*BITS_PER_WALL));
    }
    return header;
}

/*
** fwrite_str80:  write out a string padded to 80 characters.
**
** Like fwrite, this returns the number of items written, which
** should be 80 if successful, and less than 80 if it failed.
*/
#ifdef __STDC__
size_t fwrite_str80 (char *str, FILE *fp)
#else
int fwrite_str80 (str, fp)
char *str;
FILE *fp;
#endif
{
    char cbuf[80];
    size_t len;
    int i;

    /* Most of this just to avoid garbage after the name. */
    len = strlen(str);
    strncpy(cbuf, str, len < 80 ? len : 80);

    for (i = len; i < 80; i++)
        cbuf[i] = '\0';  /* pad with zeros */

    return fwrite(cbuf, sizeof(char), 80, fp);
}
