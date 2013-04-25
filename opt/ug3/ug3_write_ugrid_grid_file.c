#include "UG3_LIB.h"

INT_ ug3_write_ugrid_grid_file
 (char File_Name[],
  INT_ Message_Flag,
  INT_ File_Format_Flag,
  INT_ Write_Task_Flag,
  INT_ Number_of_BL_Vol_Tets,
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
 * Incrementally write grid data to a UGRID volume grid file.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_write_ugrid_grid_file.c,v 1.30 2013/03/19 04:35:53 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  static FILE *Grid_File = NULL;

  CHAR_UG_MAX drive, dname, fname, ext;

  CHAR_31 Name_Text;
  CHAR_133 Text;

  static INT_ Previous_Tasks = 0;

  INT_ Byte_Order, Index, Mode_Flag, Number_of_Surf_Faces, Number_of_Vol_Elems,
       Number_of_Vol_IDs;
  INT_ Write_Flag = 0;

  if (ug_file_status_monitor_flag())
  {
    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "ug3_write_ugrid_grid_file: writing UGRID file with File_Name=%s File_Format_Flag=%i Write_Task_Flag=%i", Name_Text, File_Format_Flag, Write_Task_Flag);
    ug_message (Text);
  }

  Byte_Order = (File_Format_Flag == 2) ? 1:
               (File_Format_Flag == 3) ? -1: ug_get_byte_order ();

  ug_set_byte_order (Byte_Order);

  if (Grid_File == NULL && Previous_Tasks == 9)
    Previous_Tasks = 0;

  if ((Write_Task_Flag == 1 && Previous_Tasks == 0) || Write_Task_Flag == -1)
  {
    Previous_Tasks = 1;

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
        sprintf (Text, "ug3_write_ugrid_grid_file: opening TMP file with name %s", Name_Text);
        ug_message (Text);
      }

      Grid_File = ug_fopen (File_Name, "tmp");
    }

    else
    {
      ug_backup_file (File_Name);

      if (ug_file_status_monitor_flag())
      {
        sprintf (Text, "ug3_write_ugrid_grid_file: opening regular file with name %s", Name_Text);
        ug_message (Text);
      }

      Grid_File = ug_fopen (File_Name, "w");
    }

    if (Grid_File == NULL)
      return (340);

    if (File_Format_Flag == 1)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i %i\n",
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
      Write_Flag = 0;
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                           sizeof (INT_), 1, Grid_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Trias,
                                           sizeof (INT_), 1, Grid_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Quads,
                                           sizeof (INT_), 1, Grid_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Tets,
                                           sizeof (INT_), 1, Grid_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Pents_5,
                                           sizeof (INT_), 1, Grid_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Pents_6,
                                           sizeof (INT_), 1, Grid_File);
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Hexs,
                                           sizeof (INT_), 1, Grid_File);
      Write_Flag = Write_Flag - 7;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }

    if (Message_Flag >= 1 && File_Format_Flag >= 1)
    { 
      Number_of_Vol_Elems = Number_of_Vol_Hexs + Number_of_Vol_Pents_5
                          + Number_of_Vol_Pents_6 + Number_of_Vol_Tets;

      Number_of_Vol_IDs = 0;

      ug_message (" ");
      ug_cpu_message ("UG3      :");
      ug_message (" ");
      ug_message ("UG3      : OUTPUT GRID");
      ug_message (" ");

      sprintf (Text, "UG3      : Grid File Name    = %s", Name_Text);
      ug_message (Text);
      sprintf (Text, "UG3      : Quad Surface Faces=%10i",
               Number_of_Surf_Quads);
      ug_message (Text);
      sprintf (Text, "UG3      : Tria Surface Faces=%10i",
               Number_of_Surf_Trias);
      ug_message (Text);
      sprintf (Text, "UG3      : Nodes             =%10i",
               Number_of_Nodes);
      ug_message (Text);
      sprintf (Text, "UG3      : Total Elements    =%10i",
               Number_of_Vol_Elems);
      ug_message (Text);
      sprintf (Text, "UG3      : Hex Elements      =%10i",
               Number_of_Vol_Hexs);
      ug_message (Text);
      sprintf (Text, "UG3      : Pent_5 Elements   =%10i",
               Number_of_Vol_Pents_5);
      ug_message (Text);
      sprintf (Text, "UG3      : Pent_6 Elements   =%10i",
               Number_of_Vol_Pents_6);
      ug_message (Text);
      sprintf (Text, "UG3      : Tet Elements      =%10i",
               Number_of_Vol_Tets);
      ug_message (Text);
      sprintf (Text, "UG3      : BL Tet Elements   =%10i",
               Number_of_BL_Vol_Tets);
      ug_message (Text);
      sprintf (Text, "UG3      : Vol./Material IDs =%10i",
               Number_of_Vol_IDs);
      ug_message (Text);
      ug_message (" ");
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 1) || Write_Task_Flag == -2)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 2;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%.15g %.15g %.15g\n",
                              Coordinates[Index][0],
                              Coordinates[Index][1],
                              Coordinates[Index][2]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Coordinates[1][0],
                              sizeof (double), Number_of_Nodes*3, Grid_File);
      Write_Flag = Write_Flag - Number_of_Nodes*3;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 2) || Write_Task_Flag == -3)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 3;

    Number_of_Surf_Faces = Number_of_Surf_Quads + Number_of_Surf_Trias;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i %i %i\n", 
                              Surf_Tria_Connectivity[Index][0],
                              Surf_Tria_Connectivity[Index][1],
                              Surf_Tria_Connectivity[Index][2]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Surf_Tria_Connectivity[1][0],
                              sizeof (INT_), Number_of_Surf_Trias*3, 
                              Grid_File);
      Write_Flag = Write_Flag - Number_of_Surf_Trias*3;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i %i %i %i\n", 
                              Surf_Quad_Connectivity[Index][0],
                              Surf_Quad_Connectivity[Index][1],
                              Surf_Quad_Connectivity[Index][2],
                              Surf_Quad_Connectivity[Index][3]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Surf_Quad_Connectivity[1][0],
                              sizeof (INT_), Number_of_Surf_Quads*4,
                              Grid_File);
      Write_Flag = Write_Flag - Number_of_Surf_Quads*4;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i\n", Surf_ID_Flag[Index]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Surf_ID_Flag[1],
                              sizeof (INT_), Number_of_Surf_Faces, Grid_File);
      Write_Flag = Write_Flag - Number_of_Surf_Faces;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 3) || Write_Task_Flag == -4)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 4;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i %i %i %i\n", 
                              Vol_Tet_Connectivity[Index][0],
                              Vol_Tet_Connectivity[Index][1],
                              Vol_Tet_Connectivity[Index][2],
                              Vol_Tet_Connectivity[Index][3]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Vol_Tet_Connectivity[1][0],
                              sizeof (INT_1D), Number_of_Vol_Tets*4, 
                              Grid_File);
      Write_Flag = Write_Flag - Number_of_Vol_Tets*4;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 4) || Write_Task_Flag == -5)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 5;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i %i %i %i %i\n", 
                              Vol_Pent_5_Connectivity[Index][0],
                              Vol_Pent_5_Connectivity[Index][1],
                              Vol_Pent_5_Connectivity[Index][2],
                              Vol_Pent_5_Connectivity[Index][3],
                              Vol_Pent_5_Connectivity[Index][4]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Vol_Pent_5_Connectivity[1][0],
                              sizeof (INT_), Number_of_Vol_Pents_5*5, 
                              Grid_File); 
      Write_Flag = Write_Flag - Number_of_Vol_Pents_5*5;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 5) || Write_Task_Flag == -6)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 6;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i\n", 
                              Vol_Pent_6_Connectivity[Index][0],
                              Vol_Pent_6_Connectivity[Index][1],
                              Vol_Pent_6_Connectivity[Index][2],
                              Vol_Pent_6_Connectivity[Index][3],
                              Vol_Pent_6_Connectivity[Index][4],
                              Vol_Pent_6_Connectivity[Index][5]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Vol_Pent_6_Connectivity[1][0],
                              sizeof (INT_), Number_of_Vol_Pents_6*6, 
                              Grid_File); 
      Write_Flag = Write_Flag - Number_of_Vol_Pents_6*6;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 6) || Write_Task_Flag == -7)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 7;

    if (File_Format_Flag == 1)
    {
      for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i %i %i\n", 
                              Vol_Hex_Connectivity[Index][0],
                              Vol_Hex_Connectivity[Index][1],
                              Vol_Hex_Connectivity[Index][2],
                              Vol_Hex_Connectivity[Index][3],
                              Vol_Hex_Connectivity[Index][4],
                              Vol_Hex_Connectivity[Index][5],
                              Vol_Hex_Connectivity[Index][6],
                              Vol_Hex_Connectivity[Index][7]);
      }
    }
    else
    {
      Write_Flag = ug_fwrite (&Vol_Hex_Connectivity[1][0],
                              sizeof (INT_), Number_of_Vol_Hexs*8, 
                              Grid_File); 
      Write_Flag = Write_Flag - Number_of_Vol_Hexs*8;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 7) || Write_Task_Flag == -8)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 8;

    if (File_Format_Flag == 1)
      Write_Flag = fprintf (Grid_File, "%i\n", Number_of_BL_Vol_Tets);
    else
    {
      Write_Flag = ug_fwrite (&Number_of_BL_Vol_Tets,
                              sizeof (INT_), 1, Grid_File); 
      Write_Flag = Write_Flag - 1;
    }

    if (Write_Flag < 0)
    {
      ug_fclose (Grid_File);
      return (340);
    }
  }

  if ((Write_Task_Flag == 1 && Previous_Tasks == 8) || Write_Task_Flag == -9)
  {
    if (Grid_File == NULL)
      return (340);

    Previous_Tasks = 9;

    Mode_Flag = ug_get_file_stream_mode_flag (Grid_File);

    if (Mode_Flag == UG_FIO_TMP_FILE_MODE)
      ug_rewind (Grid_File);
    else
      ug_fclose (Grid_File);

    Grid_File = NULL;
  }

  return (0);

}
