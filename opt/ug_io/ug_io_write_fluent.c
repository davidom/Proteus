#include "UG_IO_LIB.h"

INT_ ug_io_write_fluent
 (FILE * Grid_File,
  char Case_Name[],
  char Error_Message[],
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6,
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_Grid_BC_Flag,
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
 * Write grid data to a binary FLUENT volume grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_fluent.c,v 1.24 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text, zone_name, zone_type;
  CHAR_UG_MAX arch_mode32_suffix, arch_mode64_suffix, 
              base_arch_type, base_sys_arch_type,
              def_arch_mode_suffix, def_arch_type,
              Line,
              Tags_File_Name;

  INT_ bqf, btf, cellnum, dim, domain_id, etype, ieleml, ielemr, index1, index2,
       ioffset, i2, nitems, nquad, ntria, nv, qf, surfid, tf, type,
       zoneid;
  INT_ Error_Flag, Fluid_Zone, Index, Interior_Zone, Max_SurfID,
       Number_of_Cells, Number_of_Quad_Faces, Number_of_Tria_Faces,
       Number_of_Trans_Surf_Trias, Number_of_Trans_Surf_Quads, Write_Flag,
       Number_of_Surfaces;
  INT_ tmp_array[7];

  INT_1D *Groups_ = NULL;
  INT_3D *Groups = NULL;
  INT_5D *Tria_Faces = NULL;
  INT_6D *Quad_Faces = NULL;
  INT_6D *Bnd_Tria = NULL;
  INT_7D *Bnd_Quad = NULL;

  FILE *Grid_Tags_File = NULL;

  Error_Flag = ug_get_arch_info (arch_mode32_suffix, arch_mode64_suffix,
                                 base_arch_type, base_sys_arch_type,
                                 def_arch_mode_suffix, def_arch_type); 

  if(Error_Flag){
    strcpy (Error_Message, "unable to determine system type for writing FLUENT grid file");
    return (1);
  }
  else if(strstr (base_sys_arch_type, "WIN")){
    strcpy (Error_Message, "unable to write FLUENT grid file for Windows systems");
    return (1);
  }

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
		 &Groups_,
		 &Tria_Faces,
		 &Quad_Faces);

  if(Error_Flag){
    strcpy (Error_Message, "unable to extract faces for FLUENT grid file");
    return (1);
  }

  Number_of_Cells = Number_of_Vol_Tets
                  + Number_of_Vol_Pents_5 + Number_of_Vol_Pents_6
                  + Number_of_Vol_Hexs;

  /* Write header info */
  strcpy (Text, "(0 \"UG_IO to FLUENT File\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  strcpy (Text, "(1 \"File generated using MSU SimCenter UG_IO\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  strcpy (Text, "(0 \"Machine Config:\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  if (strstr (base_sys_arch_type, "IRIX"))
    strcpy (Text, "(4 (23 1 0 1 2 4 4 4 8 4 4))\n");
  else if (strstr (base_sys_arch_type, "SunOS"))
    strcpy (Text, "(4 (10 1 0 1 2 4 4 4 8 4 4))\n");
  else if (strstr (base_sys_arch_type, "Linux"))
    strcpy (Text, "(4 (60 0 0 1 2 4 4 4 8 4 4))\n");
  else if (ug_get_byte_order() == 1)
    strcpy (Text, "(4 (23 1 0 1 2 4 4 4 8 4 4))\n");
  else
    strcpy (Text, "(4 (60 0 0 1 2 4 4 4 8 4 4))\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  strcpy (Text, "(0 \"Dimension:\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  strcpy (Text, "(2 3)\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  strcpy (Text, "(0 \"Grid:\")\n\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);

  tf = 0;
  qf = 0;
  btf = 0;
  bqf = 0;
  cellnum = 1;
  Max_SurfID = Number_of_Surfaces;
  Error_Flag = 0;
  Number_of_Trans_Surf_Trias = 0;
  Number_of_Trans_Surf_Quads = 0;

  /*
  for (Index = 1; Index <= Number_of_Surf_Trias; Index++)
  {
    if (CHK_TRANSP_UG3_GBC (Surf_Grid_BC_Flag[Index])) Number_of_Trans_Surf_Trias++;

    surfid = Surf_ID_Flag[Index];
    if (surfid > Max_SurfID) Max_SurfID = surfid;
  }

  for (Index = 1; Index <= Number_of_Surf_Quads; Index++)
  {
    if (CHK_TRANSP_UG3_GBC (Surf_Grid_BC_Flag[Number_of_Surf_Trias + Index])) Number_of_Trans_Surf_Quads++;

    surfid = Surf_ID_Flag[Number_of_Surf_Trias + Index];
    if (surfid > Max_SurfID) Max_SurfID = surfid;
  }
  */
  /* Write the cell declaration */
  strcpy (Text, "(0 \"Cells:\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  zoneid = 0;
  index1 = 1;
  index2 = Number_of_Cells;
  type   = 0;
  sprintf(Text, "(12 (%x %x %x %x))\n",zoneid,index1,index2,type);
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);

  /* Write the face declaration */
  strcpy (Text, "(0 \"Faces:\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  zoneid = 0;
  index1 = 1;
  index2 = Number_of_Tria_Faces + Number_of_Quad_Faces;
  type   = 0;
  sprintf(Text, "(13 (%x %x %x %x))\n",zoneid,index1,index2,type);
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);

  /* Write the coordinate declaration */
  strcpy (Text, "(0 \"Nodes:\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  zoneid = 0;
  index1 = 1;
  index2 = Number_of_Nodes;
  type   = 0;
  dim    = 3;
  sprintf (Text, "(10 (%x %x %x %x %x))\n\n",  zoneid, index1, index2, type, dim);
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);

  /* Allocate all memory we will need */
  Groups     = (INT_3D *) ug_malloc (&Error_Flag, (Max_SurfID+1) * sizeof (INT_3D));
  Bnd_Tria   = (INT_6D *) ug_malloc (&Error_Flag, Number_of_Surf_Trias * sizeof (INT_6D));
  Bnd_Quad   = (INT_7D *) ug_malloc (&Error_Flag, Number_of_Surf_Quads * sizeof (INT_7D));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc FLUENT I/O work arrays");
    ug_free (Groups);
    ug_free (Tria_Faces);
    ug_free (Quad_Faces);
    ug_free (Bnd_Tria);
    ug_free (Bnd_Quad);
    return (1);
  }

  for (Index = 1; Index <= Max_SurfID; ++Index)
  {
    Groups[Index][0] = 0;
    Groups[Index][1] = 0;
    Groups[Index][2] = 3;   /* default bc: Wall */
  }

  for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
  {
    surfid = Surf_ID_Flag[Index];

    tf++;

    /* Count the number of trias in each group */
    Groups[surfid][0]++;

    /* Set the bc to interior */
    if (CHK_TRANSP_UG3_GBC (Surf_Grid_BC_Flag[Index])) Groups[surfid][2] = 2;
  }

  for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
  {
    surfid = Surf_ID_Flag[Number_of_Surf_Trias + Index];

    qf++;

    /* Count the number of quads in each group */
    Groups[surfid][1]++;

    /* Set the bc to interior */
    if (CHK_TRANSP_UG3_GBC (Surf_Grid_BC_Flag[Number_of_Surf_Trias + Index])) Groups[surfid][2] = 2;
  }

  Interior_Zone = Max_SurfID + 1;
  Fluid_Zone = Max_SurfID + 2;

  /* Write the cells */
  if (Number_of_Cells > 0)
  {
    zoneid = Fluid_Zone;
    index1 = 1;
    index2 = Number_of_Cells;
    type   = 1;
    if (Number_of_Cells == Number_of_Vol_Tets)
    {
      etype = 2;       /* all tet4 */
      Write_Flag = 0;
    }
    else if (Number_of_Cells == Number_of_Vol_Pents_5)
    {
      etype = 5;       /* all pent5 */
      Write_Flag = 0;
    }
    else if (Number_of_Cells == Number_of_Vol_Pents_6)
    {
      etype = 6;       /* all pent6 */
      Write_Flag = 0;
    }
    else if (Number_of_Cells == Number_of_Vol_Hexs)
    {
      etype = 4;      /* all hex8 */
      Write_Flag = 0;
    }
    else
    {
      etype  = 0;      /* mixed */
      Write_Flag = 1;
    }
    sprintf(Text, "(3012 (%x %x %x %x %x)\n(",zoneid,index1,index2,type,etype);
    fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  }

  nitems = 1;

  /* Write number tets */
  etype = 2;
  for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
  {
    if (Write_Flag) fwrite (&etype, sizeof(INT_), nitems, Grid_File);
  }

  /* Write number of pent5s */
  etype = 5;
  for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
  {
    if (Write_Flag) fwrite (&etype, sizeof(INT_), nitems, Grid_File);
  }

  /* Write number of pent6s */
  etype = 6;
  for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
  {
    if (Write_Flag) fwrite (&etype, sizeof(INT_), nitems, Grid_File);
  }

  /* Write number of hexs */
  etype = 4;
  for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
  {
    if (Write_Flag) fwrite (&etype, sizeof(INT_), nitems, Grid_File);
  }

  if (Number_of_Cells > 0)
  {
    strcpy (Text, ")\nEnd of Binary Section 3012)\n\n");
    fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  }

  /* Write the zone information */
  strcpy (Text, "(0 \"Zones:\")\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  sprintf(Text, "(45 (%d fluid fluid)())\n",Fluid_Zone);
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  sprintf(Text, "(45 (%d interior interior-faces)())\n",Interior_Zone);
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);

  /* Read the boundary condition file, if present */
  sprintf(Tags_File_Name, "%s.fluent.bc",Case_Name);

  Grid_Tags_File = fopen(Tags_File_Name, "r");

  if (Grid_Tags_File != NULL)
  {
    while (!feof(Grid_Tags_File))
    {
      nv = fscanf(Grid_Tags_File, "(%d (%d %s %[^()])())\n",&Index,&surfid,zone_type,zone_name);
      if (nv != 4 || surfid > Max_SurfID)
      {
        strcpy (Error_Message, "error reading FLUENT boundary condition file");
        ug_free (Groups);
        ug_free (Tria_Faces);
        ug_free (Quad_Faces);
        ug_free (Bnd_Tria);
        ug_free (Bnd_Quad);
        fclose (Grid_Tags_File);
        return (1);
      }

      if (!strcmp(zone_type, "axis"))
        Groups[surfid][2] = 37;
      else if (!strcmp(zone_type, "exhaust fan"))
        Groups[surfid][2] = 5;
      else if (!strcmp(zone_type, "fan"))
        Groups[surfid][2] = 14;
      else if (!strcmp(zone_type, "inlet vent"))
        Groups[surfid][2] = 4;
      else if (!strcmp(zone_type, "intake fan"))
        Groups[surfid][2] = 4;
      else if (!strcmp(zone_type, "interface"))
        Groups[surfid][2] = 24;
      else if (!strcmp(zone_type, "interior"))
        Groups[surfid][2] = 2;
      else if (!strcmp(zone_type, "mass-flow-inlet"))
        Groups[surfid][2] = 20;
      else if (!strcmp(zone_type, "outlet vent"))
        Groups[surfid][2] = 5;
      else if (!strcmp(zone_type, "outflow"))
        Groups[surfid][2] = 36;
      else if (!strcmp(zone_type, "periodic"))
        Groups[surfid][2] = 12;
      else if (!strcmp(zone_type, "porous-jump"))
        Groups[surfid][2] = 14;
      else if (!strcmp(zone_type, "pressure-far-field"))
        Groups[surfid][2] = 9;
      else if (!strcmp(zone_type, "pressure-inlet"))
        Groups[surfid][2] = 4;
      else if (!strcmp(zone_type, "pressure-outlet"))
        Groups[surfid][2] = 5;
      else if (!strcmp(zone_type, "radiator"))
        Groups[surfid][2] = 14;
      else if (!strcmp(zone_type, "shadow"))
        Groups[surfid][2] = 8;
      else if (!strcmp(zone_type, "symmetry"))
        Groups[surfid][2] = 7;
      else if (!strcmp(zone_type, "velocity-inlet"))
        Groups[surfid][2] = 10;
      else if (!strcmp(zone_type, "wall"))
        Groups[surfid][2] = 3;
      else
      {
        strcpy (Error_Message, "invalid FLUENT boundary conditions");
        ug_free (Groups);
        ug_free (Tria_Faces);
        ug_free (Quad_Faces);
        ug_free (Bnd_Tria);
        ug_free (Bnd_Quad);
        fclose (Grid_Tags_File);
        return (1);
      }

      sprintf(Text, "(%d (%d %s %s %d)())\n",Index,surfid,zone_type,zone_name,Groups[surfid][2]);
      fwrite (Text, sizeof(char), strlen(Text), Grid_File);
    }

    fclose (Grid_Tags_File);
  }
  else
  {
    sprintf(Tags_File_Name,"%s.tags",Case_Name);

    Grid_Tags_File = fopen(Tags_File_Name, "r");

    if (Grid_Tags_File != NULL)
    {
      Index = 45;
      strcpy (zone_type, "wall");
      domain_id = 3;

      while (!feof(Grid_Tags_File))
      {
        if (fgets(Line, UG_MAX_CHAR_STRING_LENGTH, Grid_Tags_File) != NULL)
        {
          if (strncmp(Line, "#", 1))
          {
            nv = sscanf(Line, "%d %s %s", &surfid, zone_name, Text);
            if (nv != 3)
            {
              strcpy (Error_Message, "error reading SolidMesh tags file for FLUENT grid file");
              ug_free (Groups);
              ug_free (Tria_Faces);
              ug_free (Quad_Faces);
              ug_free (Bnd_Tria);
              ug_free (Bnd_Quad);
              fclose (Grid_Tags_File);
              return (1);
            }

            sprintf(Text, "(%d (%d %s %s %d)())\n",Index,surfid,zone_type,zone_name,domain_id);
            fwrite (Text, sizeof(char), strlen(Text), Grid_File);
          }
          else if (isdigit(Line[1]))
          {
            nv = sscanf(Line, "#%d: %s %s", &surfid, zone_name, Text);
            if (nv != 3)
            {
              strcpy (Error_Message, "error reading SolidMesh tags file for FLUENT grid file");
              ug_free (Groups);
              ug_free (Tria_Faces);
              ug_free (Quad_Faces);
              ug_free (Bnd_Tria);
              ug_free (Bnd_Quad);
              fclose (Grid_Tags_File);
              return (1);
            }

            sprintf(Text, "(%d (%d %s %s %d)())\n",Index,surfid,zone_type,zone_name,domain_id);
            fwrite (Text, sizeof(char), strlen(Text), Grid_File);
          }
        }
      }

      fclose (Grid_Tags_File);
    }
  }
  strcpy (Text, "\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);


  /* Write the interior faces */
  if (Number_of_Cells > 0)
  {
    zoneid = Interior_Zone;
    index1 = 1;
    index2 = Number_of_Tria_Faces + Number_of_Quad_Faces - Number_of_Surf_Trias - Number_of_Surf_Quads;
    type   = 2;     /* interior bc */
    if (Number_of_Quad_Faces - Number_of_Surf_Quads == 0)
    {
      etype   = 3;  /* all trias */
      ioffset = 0;
    }
    else if (Number_of_Tria_Faces - Number_of_Surf_Trias == 0)
    {
      etype   = 4;  /* all quads */
      ioffset = 0;
    }
    else
    {
      etype   = 0;  /* mixed */
      ioffset = 1;
    }
    sprintf(Text, "(3013 (%x %x %x %x %d)\n(",zoneid,index1,index2,type,etype);
    fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  }

  tf = 0;
  qf = 0;
  i2 = 0;

  nitems = 5;
  tmp_array[0] = 3;
  for (Index = 0; Index < Number_of_Tria_Faces; ++Index)
  {
    if (Tria_Faces[i2][3] < 0)  /* store the boundary face info */
    {
      surfid = -Tria_Faces[i2][3];
      /* Check for double sided boundary face */
      if ((Groups[surfid][2] == 2 || Groups[surfid][2] == 14) &&
	  Number_of_Trans_Surf_Trias > 0)
      {
	i2++;
	ieleml = Tria_Faces[i2+1][3];
	ielemr = Tria_Faces[i2][3];
      }
      else
      {
	ieleml = Tria_Faces[i2+1][3];
	ielemr = 0;
      }

      if (Tria_Faces[i2][4])
        ug3_swap_int(&Tria_Faces[i2][0],&Tria_Faces[i2][2]);

      Bnd_Tria[btf][0] = Tria_Faces[i2][0];
      Bnd_Tria[btf][1] = Tria_Faces[i2][1];
      Bnd_Tria[btf][2] = Tria_Faces[i2][2];
      Bnd_Tria[btf][3] = ieleml;
      Bnd_Tria[btf][4] = ielemr;
      Bnd_Tria[btf][5] = surfid;

      btf++;
    }
    else /* write interior tria faces */
    {
      tmp_array[ioffset + 0] = Tria_Faces[i2][0];
      tmp_array[ioffset + 1] = Tria_Faces[i2][1];
      tmp_array[ioffset + 2] = Tria_Faces[i2][2];
      if (Tria_Faces[i2][4])
      {
        tmp_array[ioffset + 3] = Tria_Faces[i2][3];
        tmp_array[ioffset + 4] = Tria_Faces[i2+1][3];
      }
      else
      {
        tmp_array[ioffset + 3] = Tria_Faces[i2+1][3];
        tmp_array[ioffset + 4] = Tria_Faces[i2][3];
      }

      fwrite (tmp_array, sizeof(INT_), (size_t)(nitems+ioffset), Grid_File);

      tf++;
    }

    i2 += 2;
  }

  i2 = 0;

  nitems = 6;
  tmp_array[0] = 4;
  for (Index = 0; Index < Number_of_Quad_Faces; ++Index)
  {
    if (Quad_Faces[i2][4] < 0)  /* store the boundary face info */
    {
      surfid = -Quad_Faces[i2][4];
      /* Check for double sided boundary face */
      if ((Groups[surfid][2] == 2 || Groups[surfid][2] == 14) &&
	  Number_of_Trans_Surf_Quads > 0)
      {
	i2++;
	ieleml = Quad_Faces[i2+1][4];
	ielemr = Quad_Faces[i2][4];
      }
      else
      {
	ieleml = Quad_Faces[i2+1][4];
	ielemr = 0;
      }

      if (Quad_Faces[i2][5])
      {
        Bnd_Quad[bqf][0] = Quad_Faces[i2][3];
        Bnd_Quad[bqf][1] = Quad_Faces[i2][2];
        Bnd_Quad[bqf][2] = Quad_Faces[i2][1];
        Bnd_Quad[bqf][3] = Quad_Faces[i2][0];
      }
      else
      {
        Bnd_Quad[bqf][0] = Quad_Faces[i2][0];
        Bnd_Quad[bqf][1] = Quad_Faces[i2][1];
        Bnd_Quad[bqf][2] = Quad_Faces[i2][2];
        Bnd_Quad[bqf][3] = Quad_Faces[i2][3];
      }
      Bnd_Quad[bqf][4] = ieleml;
      Bnd_Quad[bqf][5] = ielemr;
      Bnd_Quad[bqf][6] = surfid;

      bqf++;
    }
    else  /* write interior quad faces */
    {
      tmp_array[ioffset + 0] = Quad_Faces[i2][0];
      tmp_array[ioffset + 1] = Quad_Faces[i2][1];
      tmp_array[ioffset + 2] = Quad_Faces[i2][2];
      tmp_array[ioffset + 3] = Quad_Faces[i2][3];
      if (Quad_Faces[i2][5])
      {
        tmp_array[ioffset + 4] = Quad_Faces[i2][4];
        tmp_array[ioffset + 5] = Quad_Faces[i2+1][4];
      }
      else
      {
        tmp_array[ioffset + 4] = Quad_Faces[i2+1][4];
        tmp_array[ioffset + 5] = Quad_Faces[i2][4];
      }

      fwrite (tmp_array, sizeof(INT_), (size_t)(nitems+ioffset), Grid_File);

      qf++;
    }

    i2 += 2;
  }

  /* check size of btf bqf */
  if (btf != Number_of_Surf_Trias || bqf != Number_of_Surf_Quads)
  {
    strcpy (Error_Message, "unable to match boundary surface faces in FLUENT grid data");
    ug_free (Groups);
    ug_free (Tria_Faces);
    ug_free (Quad_Faces);
    ug_free (Bnd_Tria);
    ug_free (Bnd_Quad);
    return (1);
  }

  if (Number_of_Cells > 0)
  {
    strcpy (Text, ")\nEnd of Binary Section 3013)\n\n");
    fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  }

  /* Sort the boundary face information based on surface id */
  qsort(Bnd_Tria, (size_t) btf, sizeof(INT_6D), ug3_bnd_tria_sort);
  qsort(Bnd_Quad, (size_t) bqf, sizeof(INT_7D), ug3_bnd_quad_sort);

  btf = 0;
  bqf = 0;

  for (surfid = 1; surfid <= Max_SurfID; ++surfid)
  {
    ntria = Groups[surfid][0];
    nquad = Groups[surfid][1];

    if (ntria + nquad > 0)
    {
      zoneid = surfid;
      index1 = tf + qf + 1;
      index2 = index1 + ntria + nquad - 1;
      type   = Groups[surfid][2];
      if (nquad == 0)
      {
        etype   = 3;  /* trias */
        ioffset = 0;
      }
      else if (ntria == 0)
      {
        etype   = 4;  /* quads */
        ioffset = 0;
      }
      else
      {
        etype   = 0;  /* mixed */
        ioffset = 1;
      }
      sprintf(Text, "(3013 (%x %x %x %x %x)\n(",zoneid,index1,index2,type,etype);
      fwrite (Text, sizeof(char), strlen(Text), Grid_File);

      nitems = 5;
      tmp_array[0] = 3;
      for (Index = 0; Index < ntria; Index++)
      {
        tmp_array[ioffset + 0] = Bnd_Tria[btf][0];
        tmp_array[ioffset + 1] = Bnd_Tria[btf][1];
        tmp_array[ioffset + 2] = Bnd_Tria[btf][2];
        tmp_array[ioffset + 3] = Bnd_Tria[btf][3];
        tmp_array[ioffset + 4] = Bnd_Tria[btf][4];

        fwrite (tmp_array, sizeof(INT_), (size_t)(nitems+ioffset), Grid_File);

        tf++;
        btf++;
      }

      nitems = 6;
      tmp_array[0] = 4;
      for (Index = 0; Index < nquad; Index++)
      {
        tmp_array[ioffset + 0] = Bnd_Quad[bqf][0];
        tmp_array[ioffset + 1] = Bnd_Quad[bqf][1];
        tmp_array[ioffset + 2] = Bnd_Quad[bqf][2];
        tmp_array[ioffset + 3] = Bnd_Quad[bqf][3];
        tmp_array[ioffset + 4] = Bnd_Quad[bqf][4];
        tmp_array[ioffset + 5] = Bnd_Quad[bqf][5];

        fwrite (tmp_array, sizeof(INT_), (size_t)(nitems+ioffset), Grid_File);

        qf++;
        bqf++;
      }
      strcpy (Text, ")\nEnd of Binary Section 3013)\n\n");
      fwrite (Text, sizeof(char), strlen(Text), Grid_File);
    }
  }

  /* Write the coordinates */
  zoneid = 1;
  index1 = 1;
  index2 = Number_of_Nodes;
  type   = 1;
  nitems = 3*Number_of_Nodes;
  sprintf (Text, "(3010 (%x %x %x %x %x)\n(",zoneid,index1,index2,type,dim);
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);
  Write_Flag = (INT_) fwrite (&Coordinates[1][0],
                              sizeof (double),
                              nitems,
                              Grid_File);
  strcpy (Text, ")\nEnd of Binary Section 3010)\n\n");
  fwrite (Text, sizeof(char), strlen(Text), Grid_File);

  if (Write_Flag < Number_of_Nodes)
  {
    strcpy (Error_Message, "error writing FLUENT grid file");
    ug_free (Groups);
    ug_free (Tria_Faces);
    ug_free (Quad_Faces);
    ug_free (Bnd_Tria);
    ug_free (Bnd_Quad);
    return (1);
  }

  /* Free working memory */
  ug_free (Groups);
  ug_free (Tria_Faces);
  ug_free (Quad_Faces);
  ug_free (Bnd_Tria);
  ug_free (Bnd_Quad);

  return (0);
}
