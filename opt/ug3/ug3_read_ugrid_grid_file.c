#include "UG3_LIB.h"

INT_ ug3_read_ugrid_grid_file
 (char File_Name[],
  INT_ File_Format_Flag,
  INT_ Read_Task_Flag,
  INT_ *Number_of_BL_Vol_Tets,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Hexs,
  INT_ *Number_of_Vol_Pents_5,
  INT_ *Number_of_Vol_Pents_6,
  INT_ *Number_of_Vol_Tets,
  INT_ Number_of_Read_Nodes,
  INT_ Number_of_Read_Vol_Hexs,
  INT_ Number_of_Read_Vol_Pents_5,
  INT_ Number_of_Read_Vol_Pents_6,
  INT_ Number_of_Read_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D ** Vol_Hex_Connectivity_Ptr,
  INT_5D ** Vol_Pent_5_Connectivity_Ptr,
  INT_6D ** Vol_Pent_6_Connectivity_Ptr,
  INT_4D ** Vol_Tet_Connectivity_Ptr,
  DOUBLE_3D ** Coordinates_Ptr)

{

/*
 * Incrementally read grid data from a UGRID volume grid file.
 *
 * If Read_Task_Flag = 1 then open the grid file and read number of nodes,
 * boundary faces, and elements.
 *
 * If Read_Task_Flag = 2 then read everything after number of nodes, boundary
 * faces, and elements (see Read_Task_Flag = -2,-3,...,-8) and then close or
 * rewind the grid file.
 *
 * If Read_Task_Flag = 3 then open the grid file, reallocate arrays, read
 * everything (see Read_Task_Flag = -1,-2,-3,...,-8) and then close or rewind
 * the grid file.
 *
 * If Read_Task_Flag = -1 then open the grid file and read number of nodes,
 * boundary faces, and elements.
 *
 * If Read_Task_Flag = -2 then incrementally read node coordinates
 * (Number_of_Read_Nodes at a time).
 *
 * If Read_Task_Flag = -3 then read boundary face connectivity and flags
 *
 * If Read_Task_Flag = -4 then incrementally read tet element connectivity
 * (Number_of_Read_Read_Vol_Tets at a time).
 *
 * If Read_Task_Flag = -5 then incrementally read pyramid (pent-5) element
 * connectivity (Number_of_Read_Vol_Pents_5 at a time).
 *
 * If Read_Task_Flag = -6 then incrementally read prism (pent-6) element
 * connectivity (Number_of_Read_Vol_Pents_6 at a time).
 *
 * If Read_Task_Flag = -7 then incrementally read hex element connectivity
 * (Number_of_Read_Vol_Hexs at a time).
 *
 * If Read_Task_Flag = -8 then read number of BL tet elements
 *
 * If Read_Task_Flag = -9 then close or rewind the grid file
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug3_read_ugrid_grid_file.c,v 1.33 2013/03/19 04:35:44 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  static FILE *Grid_File = NULL;

  CHAR_UG_MAX drive, dname, fname, ext;

  CHAR_133 Text;
  CHAR_31 Name_Text;

  INT_8D *Vol_Hex_Connectivity = NULL;
  INT_5D *Vol_Pent_5_Connectivity = NULL;
  INT_6D *Vol_Pent_6_Connectivity = NULL;
  INT_4D *Vol_Tet_Connectivity = NULL;
  DOUBLE_3D *Coordinates = NULL;

  INT_ Byte_Order, Error_Flag, Index, Mode_Flag, Number_of_Surf_Faces,
       Read_Flag, TMP_int, TMP_int1, TMP_int2, TMP_int3, TMP_int4, TMP_int5,
       TMP_int6, TMP_int7, TMP_int8;

  if (ug_file_status_monitor_flag())
  {
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "ug3_read_ugrid_grid_file : reading UGRID file with File_Name=%s File_Format_Flag=%i Read_Task_Flag=%i", Name_Text, File_Format_Flag, Read_Task_Flag);
    ug_message (Text);
  }

  Byte_Order = ug_get_byte_order ();
  
  Byte_Order = (File_Format_Flag == 2) ? 1:
               (File_Format_Flag == 3) ? -1: ug_get_byte_order ();

  ug_set_byte_order (Byte_Order);

  if (Read_Task_Flag == 1 || Read_Task_Flag == -1 || Read_Task_Flag == 3)
  {
    if (File_Format_Flag == 0 && Byte_Order > 0)
      strcat (File_Name, ".tmp.b8.ugrid");
    else if (File_Format_Flag == 0 && Byte_Order < 0)
      strcat (File_Name, ".tmp.lb8.ugrid");
    else if (File_Format_Flag == 1)
      strcat (File_Name, ".ugrid");
    else if (File_Format_Flag == 2)
      strcat (File_Name, ".b8.ugrid");
    else
      strcat (File_Name, ".lb8.ugrid");

    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);

    if (File_Format_Flag == 0)
    {
      strcat (fname, "+");
      strcpy (File_Name, fname);
    }

    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    if (File_Format_Flag == 0)
    {
      if (ug_file_status_monitor_flag())
      {
        sprintf (Text, "ug3_read_ugrid_grid_file : opening TMP file with name %s", Name_Text);
        ug_message (Text);
      }

      Grid_File = ug_fopen (File_Name, "r_tmp");
    }

    else
    {
      if (ug_file_status_monitor_flag())
      {
        sprintf (Text, "ug3_read_ugrid_grid_file : opening regular file with name %s", Name_Text);
        ug_message (Text);
      }

      Grid_File = ug_fopen (File_Name, "r");
    }

    if (Grid_File == NULL)
      return (338);

    if (File_Format_Flag == 1)
    {
      Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i %i",
                          Number_of_Nodes,
                          Number_of_Surf_Trias,
                          Number_of_Surf_Quads,
                          Number_of_Vol_Tets,
                          Number_of_Vol_Pents_5,
                          Number_of_Vol_Pents_6,
                          Number_of_Vol_Hexs);
    }
    else
    {
      Read_Flag = 0;
      Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Surf_Trias,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Surf_Quads,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Tets,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Pents_5,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Pents_6,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag + ug_fread (Number_of_Vol_Hexs,
                                        sizeof (INT_), 1, Grid_File);
      Read_Flag = Read_Flag - 7;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }
  }

  if (Read_Task_Flag == 3)
  {
    Error_Flag = 0;

    if (*Number_of_Vol_Hexs > 0)
      *Vol_Hex_Connectivity_Ptr = (INT_8D *) ug_realloc (&Error_Flag,
                                 *Vol_Hex_Connectivity_Ptr,
                                 (*Number_of_Vol_Hexs+1) * sizeof (INT_8D));

    if (*Number_of_Vol_Pents_5 > 0)
      *Vol_Pent_5_Connectivity_Ptr = (INT_5D *) ug_realloc (&Error_Flag,
                                 *Vol_Pent_5_Connectivity_Ptr,
                                 (*Number_of_Vol_Pents_5+1) * sizeof (INT_5D));

    if (*Number_of_Vol_Pents_6 > 0)
      *Vol_Pent_6_Connectivity_Ptr = (INT_6D *) ug_realloc (&Error_Flag,
                                 *Vol_Pent_6_Connectivity_Ptr,
                                 (*Number_of_Vol_Pents_6+1) * sizeof (INT_6D));

    if (*Number_of_Vol_Tets > 0)
      *Vol_Tet_Connectivity_Ptr = (INT_4D *) ug_realloc (&Error_Flag,
                                 *Vol_Tet_Connectivity_Ptr,
                                 (*Number_of_Vol_Tets+1) * sizeof (INT_4D));

    if (*Number_of_Nodes > 0)
      *Coordinates_Ptr = (DOUBLE_3D *) ug_realloc (&Error_Flag,
                                 *Coordinates_Ptr,
                                 (*Number_of_Nodes+1) * sizeof (DOUBLE_3D));

    if (Error_Flag > 0)
    {
      ug_fclose (Grid_File);
      return (100305);
    }
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -2 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    Coordinates = *Coordinates_Ptr;

    if (Read_Task_Flag >= 2)
      Number_of_Read_Nodes = *Number_of_Nodes;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Read_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%lg", &Coordinates[Index][0]);
        Read_Flag = fscanf (Grid_File, "%lg", &Coordinates[Index][1]);
        Read_Flag = fscanf (Grid_File, "%lg", &Coordinates[Index][2]);
                           
      }
    }
    else
    {
      Read_Flag = ug_fread (&Coordinates[1][0],
                            sizeof (double), Number_of_Read_Nodes*3,
                            Grid_File);
      Read_Flag = Read_Flag - Number_of_Read_Nodes*3;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -3 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int1);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int2);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int3);

        if (Surf_Tria_Connectivity != NULL)
        {
          Surf_Tria_Connectivity[Index][0] = TMP_int1;
          Surf_Tria_Connectivity[Index][1] = TMP_int2;
          Surf_Tria_Connectivity[Index][2] = TMP_int3;
        }
      }
    }
    else
    {
      if (*Number_of_Surf_Trias > 0 && Surf_Tria_Connectivity != NULL)
        Read_Flag = ug_fread (&Surf_Tria_Connectivity[1][0],
                              sizeof (INT_), *Number_of_Surf_Trias*3,
                              Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= *Number_of_Surf_Trias*3; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }

      Read_Flag = Read_Flag - *Number_of_Surf_Trias*3;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int1);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int2);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int3);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int4);

        if (Surf_Quad_Connectivity != NULL)
        {
          Surf_Quad_Connectivity[Index][0] = TMP_int1;
          Surf_Quad_Connectivity[Index][1] = TMP_int2;
          Surf_Quad_Connectivity[Index][2] = TMP_int3;
          Surf_Quad_Connectivity[Index][3] = TMP_int4;
        }
      }
    }
    else
    {
      if (*Number_of_Surf_Quads > 0 && Surf_Quad_Connectivity != NULL)
        Read_Flag = ug_fread (&Surf_Quad_Connectivity[1][0],
                              sizeof (INT_), *Number_of_Surf_Quads*4,
                              Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= *Number_of_Surf_Quads*4; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }

      Read_Flag = Read_Flag - *Number_of_Surf_Quads*4;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }

    Number_of_Surf_Faces = *Number_of_Surf_Trias + *Number_of_Surf_Quads;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int);

        if (Surf_ID_Flag != NULL)
        {
          Surf_ID_Flag[Index] = TMP_int;
        }
      }
    }
    else
    {
      if (Number_of_Surf_Faces > 0 && Surf_ID_Flag != NULL)
        Read_Flag = ug_fread (&Surf_ID_Flag[1],
                              sizeof (INT_1D), Number_of_Surf_Faces,
                              Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }

      Read_Flag = Read_Flag - Number_of_Surf_Faces;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -4 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    Vol_Tet_Connectivity = *Vol_Tet_Connectivity_Ptr;

    if (Read_Task_Flag >= 2)
      Number_of_Read_Vol_Tets = *Number_of_Vol_Tets;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Read_Vol_Tets; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int1);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int2);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int3);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int4);

        if (Vol_Tet_Connectivity != NULL)
        {
          Vol_Tet_Connectivity[Index][0] = TMP_int1;
          Vol_Tet_Connectivity[Index][1] = TMP_int2;
          Vol_Tet_Connectivity[Index][2] = TMP_int3;
          Vol_Tet_Connectivity[Index][3] = TMP_int4;
        }
      }
    }
    else
    {
      if (Number_of_Read_Vol_Tets > 0 && Vol_Tet_Connectivity != NULL)
        Read_Flag = ug_fread (&Vol_Tet_Connectivity[1][0],
                              sizeof (INT_), Number_of_Read_Vol_Tets*4,
                              Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= Number_of_Read_Vol_Tets*4; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }

      Read_Flag = Read_Flag - Number_of_Read_Vol_Tets*4;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -5 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    Vol_Pent_5_Connectivity = *Vol_Pent_5_Connectivity_Ptr;

    if (Read_Task_Flag >= 2)
      Number_of_Read_Vol_Pents_5 = *Number_of_Vol_Pents_5;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Read_Vol_Pents_5; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int1);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int2);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int3);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int4);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int5);

        if (Vol_Pent_5_Connectivity != NULL)
        {
          Vol_Pent_5_Connectivity[Index][0] = TMP_int1;
          Vol_Pent_5_Connectivity[Index][1] = TMP_int2;
          Vol_Pent_5_Connectivity[Index][2] = TMP_int3;
          Vol_Pent_5_Connectivity[Index][3] = TMP_int4;
          Vol_Pent_5_Connectivity[Index][4] = TMP_int5;
        }
      }
    }
    else
    {
      if (Number_of_Read_Vol_Pents_5 > 0 && Vol_Pent_5_Connectivity != NULL)
        Read_Flag = ug_fread (&Vol_Pent_5_Connectivity[1][0],
                              sizeof (INT_), Number_of_Read_Vol_Pents_5*5,
                              Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= Number_of_Read_Vol_Pents_5*5; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }

      Read_Flag = Read_Flag - Number_of_Read_Vol_Pents_5*5;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -6 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    Vol_Pent_6_Connectivity = *Vol_Pent_6_Connectivity_Ptr;

    if (Read_Task_Flag >= 2)
      Number_of_Read_Vol_Pents_6 = *Number_of_Vol_Pents_6;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Read_Vol_Pents_6; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int1);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int2);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int3);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int4);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int5);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int6);

        if (Vol_Pent_6_Connectivity != NULL)
        {
          Vol_Pent_6_Connectivity[Index][0] = TMP_int1;
          Vol_Pent_6_Connectivity[Index][1] = TMP_int2;
          Vol_Pent_6_Connectivity[Index][2] = TMP_int3;
          Vol_Pent_6_Connectivity[Index][3] = TMP_int4;
          Vol_Pent_6_Connectivity[Index][4] = TMP_int5;
          Vol_Pent_6_Connectivity[Index][5] = TMP_int6;
        }
      }
    }
    else
    {
      if (Number_of_Read_Vol_Pents_6 > 0 && Vol_Pent_6_Connectivity != NULL)
        Read_Flag = ug_fread (&Vol_Pent_6_Connectivity[1][0],
                              sizeof (INT_), Number_of_Read_Vol_Pents_6*6,
                              Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= Number_of_Read_Vol_Pents_6*6; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }

      Read_Flag = Read_Flag - Number_of_Read_Vol_Pents_6*6;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -7 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    Vol_Hex_Connectivity = *Vol_Hex_Connectivity_Ptr;

    if (Read_Task_Flag >= 2)
      Number_of_Read_Vol_Hexs = *Number_of_Vol_Hexs;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Read_Vol_Hexs; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int1);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int2);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int3);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int4);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int5);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int6);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int7);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_int8);

        if (Vol_Hex_Connectivity != NULL)
        {
          Vol_Hex_Connectivity[Index][0] = TMP_int1;
          Vol_Hex_Connectivity[Index][1] = TMP_int2;
          Vol_Hex_Connectivity[Index][2] = TMP_int3;
          Vol_Hex_Connectivity[Index][3] = TMP_int4;
          Vol_Hex_Connectivity[Index][4] = TMP_int5;
          Vol_Hex_Connectivity[Index][5] = TMP_int6;
          Vol_Hex_Connectivity[Index][6] = TMP_int7;
          Vol_Hex_Connectivity[Index][7] = TMP_int8;
        }
      }
    }
    else
    {
      if (Number_of_Read_Vol_Hexs > 0 && Vol_Hex_Connectivity != NULL)
        Read_Flag = ug_fread (&Vol_Hex_Connectivity[1][0],
                              sizeof (INT_), Number_of_Read_Vol_Hexs*8,
                              Grid_File);
      else
      {
        Read_Flag = 0;

        for (Index = 1; Index <= Number_of_Read_Vol_Hexs*8; ++Index)
        {
          Read_Flag = Read_Flag + ug_fread (&TMP_int,
                                            sizeof (INT_), 1, Grid_File);
        }
      }

      Read_Flag = Read_Flag - Number_of_Read_Vol_Hexs*8;
    }

    if (Read_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (338);
    }
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -8 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    *Number_of_BL_Vol_Tets = 0;

    if (File_Format_Flag == 1)
      Read_Flag = fscanf (Grid_File, "%i", Number_of_BL_Vol_Tets);
    else
      Read_Flag = ug_fread (Number_of_BL_Vol_Tets,
                            sizeof (INT_), 1, Grid_File);
  }

  if (Read_Task_Flag == 2 || Read_Task_Flag == -9 || Read_Task_Flag == 3)
  {
    if (Grid_File == NULL)
      return (338);

    Mode_Flag = ug_get_file_stream_mode_flag (Grid_File);

    if (Mode_Flag == UG_FIO_TMP_FILE_MODE)
      ug_rewind (Grid_File);
    else
      ug_fclose (Grid_File);
  }

  return (0);

}
