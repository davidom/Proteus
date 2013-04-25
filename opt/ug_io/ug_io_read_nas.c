#include "UG_IO_LIB.h"

static void ug_io_char_read_double
 (char[],
  double *);

static void ug_io_char_read_double
 (char Text[],
  double *Value)

{

/*
 * Read a double from a NASTRAN 8 character string.
 * Required to support reading FORTRAN fields with an exponent and no "e" or "E"
 * such as 2.1234-1 instead of 2.1234E-1 or 0.21234.
 */

  CHAR_21 New_Text;

  int Exponent = 0;

  sscanf (Text, "%lg%i", Value, &Exponent);

  if (Exponent != 0)
  {
    sprintf (New_Text, "%fe%i", *Value, Exponent);

    sscanf (New_Text, "%lg", Value);
  }

  return;
}

INT_ ug_io_read_nas
 (FILE * Grid_File,
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_BL_Vol_Tets,
  INT_ *Number_of_Bnd_Nodes,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Hexs,
  INT_ *Number_of_Vol_Pents_5,
  INT_ *Number_of_Vol_Pents_6,
  INT_ *Number_of_Vol_Tets,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_1D * Surf_Reconnection_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D * Vol_Hex_Connectivity,
  INT_1D * Vol_ID_Flag,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Read grid data from a NASTRAN surface grid file.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_nas.c,v 1.49 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

#define MAX_FIELDS 20

  char *Text_ptr;
  char *Read_Label;

  CHAR_UG_MAX New_Text_Line;
  CHAR_UG_MAX *Text_Field;
  CHAR_UG_MAX Text_Item;
  CHAR_UG_MAX Text_Line;

  INT_1D *Node_Index_Map = NULL;

  static INT_ Max_File_Node_Index = 0;

  INT_ Data_Flag, Coordinate_System, Error_Flag, Field, File_Node_Index, 
       File_Node_Index_0, File_Node_Index_1, File_Node_Index_2,
       File_Node_Index_3, File_Node_Index_4, File_Node_Index_5,
       File_Node_Index_6, File_Node_Index_7, Index, Item_Length, Line_Length,
       Max_File_Bnd_Node_Index, New_Line_Length, New_Node_Index,
       New_Node_Index_0, New_Node_Index_1, New_Node_Index_2, New_Node_Index_3,
       New_Node_Index_4, New_Node_Index_5, New_Node_Index_6, New_Node_Index_7,
       Node_Index, Node_Index_0, Node_Index_1, Node_Index_2, Node_Index_3,
       Node_Index_4, Node_Index_5, Node_Index_6, Node_Index_7, Number_of_BNodes,
       Number_of_Fields, Number_of_Surf_Faces, Number_of_Text_Fields,
       Number_of_Text_Fields_C,  Number_of_Total_Nodes, ptr_Length, 
       Read_Flag, Surf_ID, Surf_Index, Surf_Quad_Index, Surf_Tria_Index,
       UG_IO_NAS_Flag, Vol_Hex_Index, Vol_ID, Vol_Index, Vol_Pent_5_Index,
       Vol_Pent_6_Index, Vol_Tet_Index;

  INT_ Field_Length = 8;
  INT_ TMP_INT_ = 0;

  double TMP;

  Error_Flag = 0;

  Text_Field = (CHAR_UG_MAX *) ug_malloc (&Error_Flag,
                                          MAX_FIELDS * sizeof (CHAR_UG_MAX));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc NASTRAN I/O text array");
    ug_free (Text_Field);
    return (1);
  }

  if (Read_Task_Flag == 1)
    Max_File_Node_Index = 0;

  Max_File_Bnd_Node_Index = 0;

  Node_Index = 0;

  Surf_Tria_Index = 0;
  Surf_Quad_Index = 0;

  Vol_Tet_Index = 0;
  Vol_Pent_5_Index = 0;
  Vol_Pent_6_Index = 0;
  Vol_Hex_Index = 0;

  UG_IO_NAS_Flag = 0;

  do
  {
    strcpy (Text_Line, "");

    Read_Label = fgets (Text_Line, UG_MAX_CHAR_STRING_LENGTH, Grid_File);

    if (Read_Label != NULL)
    {
      Text_ptr = strchr (Text_Line, '$');

      if (Text_ptr != NULL)
      {
        if (strcmp (Text_Line, Text_ptr) == 0)
        {
          if (strstr (Text_Line, "$UG_IO_") != NULL)
            UG_IO_NAS_Flag = 1;

          Data_Flag = 0;
        }
        else
        {
          Item_Length = (INT_) (strlen (Text_Line)
                              - strlen (Text_ptr));

          strcpy (Text_Item, "");
          strcat (Text_Item, Text_Line);

          strcpy (Text_Line, "");
          strncat (Text_Line, Text_Item, Item_Length);

          Data_Flag = 1;
        }
      }
      else
        Data_Flag = 1;
    }
    else
      Data_Flag = 0;

    if (Data_Flag == 1)
    {
      Text_ptr = strchr (Text_Line, ',');

      if (Text_ptr == NULL)
      {
        strcpy (New_Text_Line, "");
        strncat (New_Text_Line, Text_Line, Field_Length);

        strcpy (Text_Field[0], "");

        Read_Flag = sscanf (New_Text_Line, "%s", Text_Field[0]);
      }
      else
      {
        Data_Flag = -1;

        strcpy (New_Text_Line, "");
        strcat (New_Text_Line, Text_Line);

        New_Line_Length = (INT_) strlen (New_Text_Line)-1;

        Field = 0;

        Text_ptr = strchr (New_Text_Line, ',');

        ptr_Length = (INT_) strlen (Text_ptr)-1;

        strcpy (Text_Field[Field], "");

        if (New_Line_Length > ptr_Length)
        {
          Item_Length = New_Line_Length - ptr_Length;

          strcpy (Text_Item, "");

          strncat (Text_Item, New_Text_Line, Item_Length);

          Read_Flag = sscanf (Text_Item, "%s", Text_Field[Field]);
        }
      }

      if (strcmp (Text_Field[0], "GRID") == 0)
        Number_of_Fields = 6;

      else if (strcmp (Text_Field[0], "GRID*") == 0)
      {
        Data_Flag = 2*Data_Flag;

        if (Data_Flag > 0)
          Number_of_Fields = 9;
        else
          Number_of_Fields = 5;
      }

      else if (strcmp (Text_Field[0], "CTRIA3") == 0)
        Number_of_Fields = 6;

      else if (strcmp (Text_Field[0], "CQUAD4") == 0)
        Number_of_Fields = 7;

      else if (strcmp (Text_Field[0], "CTETRA") == 0)
        Number_of_Fields = 7;

      else if (strcmp (Text_Field[0], "CPENTA") == 0)
        Number_of_Fields = 9;

      else if (strcmp (Text_Field[0], "CHEXA") == 0)
      {
        Data_Flag = 2*Data_Flag;

        Number_of_Fields = 9;
      }

      else
        Data_Flag = 0;
    }

    if (Data_Flag > 0)
    {
      Line_Length = (INT_) strlen (Text_Line)-1;

      Number_of_Text_Fields = (Line_Length + Field_Length - 1) / Field_Length;

      if (Number_of_Text_Fields < Number_of_Fields)
      {
        sprintf (Error_Message, "NASTRAN grid file contains %s card that is missing data", Text_Field[0]);
        ug_free (Node_Index_Map);
        ug_free (Text_Field);
        return (1);
      }

      for (Field = 1; Field < Number_of_Fields; ++Field)
      {
        Index = Field * Field_Length;

        strcpy (New_Text_Line, "");
        strncat (New_Text_Line, &Text_Line[Index], Field_Length);

        strcpy (Text_Field[Field], "");

        Read_Flag = sscanf (New_Text_Line, "%s", Text_Field[Field]);
      }

      if (Data_Flag == 2)
      {
        if (Number_of_Text_Fields > Number_of_Fields)
        {
          Field = Number_of_Fields;

          Index = Field * Field_Length + 2;

          strcpy (New_Text_Line, "");
          strncat (New_Text_Line, &Text_Line[Index], Field_Length-2);

          strcpy (Text_Field[Field], "");

          Read_Flag = sscanf (New_Text_Line, "%s", Text_Field[Field]);
        }

        Read_Label = fgets (Text_Line, UG_MAX_CHAR_STRING_LENGTH, Grid_File);

        if (Read_Label != NULL)
        {
          Line_Length = (INT_) strlen (Text_Line)-1;

          Number_of_Text_Fields_C = (Line_Length + Field_Length - 1)
                                  / Field_Length;

          if (Number_of_Text_Fields_C < 3)
          {
            sprintf (Error_Message, "NASTRAN grid file contains %s continuation card that is missing data", Text_Field[0]);
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          if (Number_of_Text_Fields > Number_of_Fields)
          {
            Field = Number_of_Fields + 1;

            Index = 2;

            strcpy (New_Text_Line, "");
            strncat (New_Text_Line, &Text_Line[Index], Field_Length-2);

            strcpy (Text_Field[Field], "");

            Read_Flag = sscanf (New_Text_Line, "%s", Text_Field[Field]);

            if (strcmp (Text_Field[Field-1], Text_Field[Field]) != 0)
            {
              sprintf (Error_Message, "NASTRAN grid file contains %s card with wrong continuation line after it", Text_Field[0]);
              ug_free (Node_Index_Map);
              ug_free (Text_Field);
              return (1);
            }
          }

          for (Field = Number_of_Fields; Field <= Number_of_Fields+1; ++Field)
          {
            Index = (Field - Number_of_Fields + 1) * Field_Length;

            strcpy (New_Text_Line, "");
            strncat (New_Text_Line, &Text_Line[Index], Field_Length);

            strcpy (Text_Field[Field], "");

            Read_Flag = sscanf (New_Text_Line, "%s", Text_Field[Field]);
          }
        }
        else
        {
          sprintf (Error_Message, "NASTRAN grid file contains %s card without continuation line", Text_Field[0]);
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }
      }

      if (strcmp (Text_Field[0], "GRID*") == 0)
      {
        strcat (Text_Field[1], Text_Field[2]);

        Number_of_Text_Fields = 1 + (Number_of_Fields + 1) / 2;

        for (Field = 2; Field < Number_of_Text_Fields; ++Field)
        {
          strcpy (Text_Field[Field], Text_Field[2*Field-1]);
          strcat (Text_Field[Field], Text_Field[2*Field]);
        }
      }
    }

    else if (Data_Flag < 0)
    {
      strcpy (New_Text_Line, "");
      strcat (New_Text_Line, &Text_ptr[1]);

      New_Line_Length = ptr_Length-1;

      Field = 1;

      do
      {
        Text_ptr = strchr (New_Text_Line, ',');

        if (Text_ptr != NULL)
        {
          ptr_Length = (INT_) strlen (Text_ptr)-1;

          strcpy (Text_Field[Field], "");

          if (New_Line_Length > ptr_Length)
          {
            Item_Length = New_Line_Length - ptr_Length;

            strcpy (Text_Item, "");

            strncat (Text_Item, New_Text_Line, Item_Length);

            Read_Flag = sscanf (Text_Item, "%s", Text_Field[Field]);
          }

          strcpy (New_Text_Line, "");
          strcat (New_Text_Line, &Text_ptr[1]);

          New_Line_Length = ptr_Length-1;

          ++Field;
        }
      }
      while (Text_ptr != NULL);

      Number_of_Text_Fields = Field+1;

      if (Number_of_Text_Fields < Number_of_Fields)
      {
        sprintf (Error_Message, "NASTRAN grid file contains %s card that is missing data", Text_Field[0]);
        ug_free (Node_Index_Map);
        ug_free (Text_Field);
        return (1);
      }

      strcpy (Text_Field[Field], "");

      strcpy (Text_Item, "");

      strcat (Text_Item, New_Text_Line);

      Read_Flag = sscanf (Text_Item, "%s", Text_Field[Field]);

      if (Data_Flag == -2)
      {
        if (Number_of_Text_Fields > Number_of_Fields)
        {
          Field = Number_of_Fields;

          strcpy (New_Text_Line, "");
          strcat (New_Text_Line, Text_Field[Field]);

          Line_Length = (INT_) strlen (New_Text_Line)-1;

          strcpy (Text_Field[Field], "");

          if (Line_Length > 2)
            strcat (Text_Field[Field], &New_Text_Line[2]);
        }

        Read_Label = fgets (Text_Line, UG_MAX_CHAR_STRING_LENGTH, Grid_File);

        if (Read_Label != NULL)
        {
          if (Number_of_Text_Fields > Number_of_Fields)
          {
            strcpy (New_Text_Line, "");
            strcat (New_Text_Line, Text_Line);

            New_Line_Length = (INT_) strlen (New_Text_Line)-1;

            Text_ptr = strchr (New_Text_Line, ',');

            ptr_Length = (INT_) strlen (Text_ptr)-1;

            Field = Number_of_Fields + 1;

            strcpy (Text_Field[Field], "");

            if (New_Line_Length > ptr_Length)
            {
              Item_Length = New_Line_Length - ptr_Length;

              strcpy (Text_Item, "");

              strncat (Text_Item, New_Text_Line, Item_Length);

              Read_Flag = sscanf (Text_Item, "%s", Text_Field[Field]);

              strcpy (New_Text_Line, "");
              strcat (New_Text_Line, Text_Field[Field]);

              Line_Length = (INT_) strlen (New_Text_Line)-1;

              strcpy (Text_Field[Field], "");

              if (Line_Length > 2)
                strcat (Text_Field[Field], &New_Text_Line[2]);
            }

            if (strcmp (Text_Field[Field-1], Text_Field[Field]) != 0)
            {
              sprintf (Error_Message, "NASTRAN grid file contains %s card with wrong continuation line after it", Text_Field[0]);
              ug_free (Node_Index_Map);
              ug_free (Text_Field);
              return (1);
            }
          }
          else
          {
            strcpy (New_Text_Line, "");
            strcat (New_Text_Line, Text_Line);

            New_Line_Length = (INT_) strlen (New_Text_Line)-1;

            Text_ptr = strchr (New_Text_Line, ',');

            ptr_Length = (INT_) strlen (Text_ptr)-1;
          }

          strcpy (New_Text_Line, "");
          strcat (New_Text_Line, &Text_ptr[1]);

          New_Line_Length = ptr_Length-1;

          Field = Number_of_Fields;

          do
          {
            Text_ptr = strchr (New_Text_Line, ',');

            if (Text_ptr != NULL)
            {
              ptr_Length = (INT_) strlen (Text_ptr)-1;

              strcpy (Text_Field[Field], "");

              if (New_Line_Length > ptr_Length)
              {
                Item_Length = New_Line_Length - ptr_Length;

                strcpy (Text_Item, "");

                strncat (Text_Item, New_Text_Line, Item_Length);

                Read_Flag = sscanf (Text_Item, "%s", Text_Field[Field]);
              }

              strcpy (New_Text_Line, "");
              strcat (New_Text_Line, &Text_ptr[1]);

              New_Line_Length = ptr_Length-1;

              ++Field;
            }
          }
          while (Text_ptr != NULL);

          Number_of_Text_Fields = Field+1;

          if (Number_of_Text_Fields < Number_of_Fields)
          {
            sprintf (Error_Message, "NASTRAN grid file contains %s continueation card that is missing data", Text_Field[0]);
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          strcpy (Text_Field[Field], "");

          strcpy (Text_Item, "");

          strcat (Text_Item, New_Text_Line);

          Read_Flag = sscanf (Text_Item, "%s", Text_Field[Field]);
        }
        else
        {
          sprintf (Error_Message, "NASTRAN grid file contains %s card without continuation line", Text_Field[0]);
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }
      }
    }

    if (Data_Flag != 0)
    {
      if (Read_Task_Flag == 2 && Node_Index_Map == NULL)
      {
        Error_Flag = 0;

        Node_Index_Map = (INT_1D *) ug_malloc (&Error_Flag,
                                               (Max_File_Node_Index+1)
                                             * sizeof (INT_1D));

        if (Error_Flag > 0)
        {
          strcpy (Error_Message, "unable to malloc NASTRAN I/O work array");
          ug_free (Text_Field);
          return (1);
        }

        for (File_Node_Index = 0;
             File_Node_Index <= Max_File_Node_Index; ++File_Node_Index)
        {
          Node_Index_Map[File_Node_Index] = -1;
        }
      }

      if (strcmp (Text_Field[0], "GRID") == 0 ||
          strcmp (Text_Field[0], "GRID*") == 0)
      {
        Read_Flag = sscanf (Text_Field[1], "%i", &File_Node_Index);

        if (File_Node_Index < 0)
        {
          sprintf (Error_Message, "NASTRAN grid file contains %s node with negative index", Text_Field[0]);
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }

        ++Node_Index;

        if (Read_Task_Flag == 1)
          Max_File_Node_Index = MAX (File_Node_Index, Max_File_Node_Index);

        else if (Read_Task_Flag == 2 && Node_Index <= *Number_of_Nodes)
        {
          Read_Flag = sscanf (Text_Field[2], "%i", &Coordinate_System);

          Coordinate_System = (Read_Flag == 1) ? Coordinate_System: 0;

          if (Coordinate_System != 0)
          {
            sprintf (Error_Message, "NASTRAN grid file %s card with coordinate transformation is not supported", Text_Field[0]);
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          Node_Index_Map[File_Node_Index] = Node_Index;

          ug_io_char_read_double (Text_Field[3], &Coordinates[Node_Index][0]);
          ug_io_char_read_double (Text_Field[4], &Coordinates[Node_Index][1]);
          ug_io_char_read_double (Text_Field[5], &Coordinates[Node_Index][2]);
        }
      }

      else if (strcmp (Text_Field[0], "CTRIA3") == 0)
      {
        ++Surf_Tria_Index;

        if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
        {
          Read_Flag = sscanf (Text_Field[3], "%i", &File_Node_Index);

          Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index,
                                         File_Node_Index);

          Read_Flag = sscanf (Text_Field[4], "%i", &File_Node_Index);

          Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index,
                                         File_Node_Index);

          Read_Flag = sscanf (Text_Field[5], "%i", &File_Node_Index);

          Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index,
                                         File_Node_Index);
        }

        if (Read_Task_Flag == 2)
        {
          if (Surf_ID_Flag != NULL)
          {
            Read_Flag = sscanf (Text_Field[2], "%i", &Surf_ID);

            Surf_ID_Flag[Surf_Tria_Index] = Surf_ID;
          }

          Read_Flag = sscanf (Text_Field[3], "%i", &File_Node_Index_0);
          Read_Flag = sscanf (Text_Field[4], "%i", &File_Node_Index_1);
          Read_Flag = sscanf (Text_Field[5], "%i", &File_Node_Index_2);

          if (File_Node_Index_0 < 0 ||
              File_Node_Index_1 < 0 ||
              File_Node_Index_2 < 0)
          {
            strcpy (Error_Message, "NASTRAN grid file contains TRIA3 with negative node index");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          if (File_Node_Index_0 > Max_File_Node_Index ||
              File_Node_Index_1 > Max_File_Node_Index ||
              File_Node_Index_2 > Max_File_Node_Index)
          {
            strcpy (Error_Message, "NASTRAN grid file contains TRIA3 with node index greater than in GRID");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          Surf_Tria_Connectivity[Surf_Tria_Index][0] = File_Node_Index_0;
          Surf_Tria_Connectivity[Surf_Tria_Index][1] = File_Node_Index_1;
          Surf_Tria_Connectivity[Surf_Tria_Index][2] = File_Node_Index_2;
        }
      }

      else if (strcmp (Text_Field[0], "CQUAD4") == 0)
      {
        ++Surf_Quad_Index;

        if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
        {
          Read_Flag = sscanf (Text_Field[3], "%i", &File_Node_Index);

          Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index,
                                         File_Node_Index);

          Read_Flag = sscanf (Text_Field[4], "%i", &File_Node_Index);

          Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index,
                                         File_Node_Index);

          Read_Flag = sscanf (Text_Field[5], "%i", &File_Node_Index);

          Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index,
                                         File_Node_Index);

          Read_Flag = sscanf (Text_Field[6], "%i", &File_Node_Index);

          Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index,
                                         File_Node_Index);
        }

        if (Read_Task_Flag == 2)
        {
          if (Surf_ID_Flag != NULL)
          {
            Read_Flag = sscanf (Text_Field[2], "%i", &Surf_ID);

            Surf_Index = *Number_of_Surf_Trias + Surf_Quad_Index;

            Surf_ID_Flag[Surf_Index] = Surf_ID;
          }

          Read_Flag = sscanf (Text_Field[3], "%i", &File_Node_Index_0);
          Read_Flag = sscanf (Text_Field[4], "%i", &File_Node_Index_1);
          Read_Flag = sscanf (Text_Field[5], "%i", &File_Node_Index_2);
          Read_Flag = sscanf (Text_Field[6], "%i", &File_Node_Index_3);

          if (File_Node_Index_0 < 0 ||
              File_Node_Index_1 < 0 ||
              File_Node_Index_2 < 0 ||
              File_Node_Index_3 < 0)
          {
            strcpy (Error_Message, "NASTRAN grid file contains QUAD4 with negative node index");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          if (File_Node_Index_0 > Max_File_Node_Index ||
              File_Node_Index_1 > Max_File_Node_Index ||
              File_Node_Index_2 > Max_File_Node_Index ||
              File_Node_Index_3 > Max_File_Node_Index)
          {
            strcpy (Error_Message, "NASTRAN grid file contains QUAD4 with node index greater than in GRID");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          Surf_Quad_Connectivity[Surf_Quad_Index][0] = File_Node_Index_0;
          Surf_Quad_Connectivity[Surf_Quad_Index][1] = File_Node_Index_1;
          Surf_Quad_Connectivity[Surf_Quad_Index][2] = File_Node_Index_2;
          Surf_Quad_Connectivity[Surf_Quad_Index][3] = File_Node_Index_3;
        }
      }

      else if (strcmp (Text_Field[0], "CTETRA") == 0 &&
               *Number_of_Bnd_Nodes == 0)
      {
        ++Vol_Tet_Index;

        if (Read_Task_Flag == 2)
        {
          Read_Flag = sscanf (Text_Field[2], "%i", &Vol_ID);
          Read_Flag = sscanf (Text_Field[3], "%i", &File_Node_Index_0);
          Read_Flag = sscanf (Text_Field[4], "%i", &File_Node_Index_1);
          Read_Flag = sscanf (Text_Field[5], "%i", &File_Node_Index_2);
          Read_Flag = sscanf (Text_Field[6], "%i", &File_Node_Index_3);

          if (File_Node_Index_0 < 0 ||
              File_Node_Index_1 < 0 ||
              File_Node_Index_2 < 0 ||
              File_Node_Index_3 < 0)
          {
            strcpy (Error_Message, "NASTRAN grid file contains TETRA with negative node index");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          if (File_Node_Index_0 > Max_File_Node_Index ||
              File_Node_Index_1 > Max_File_Node_Index ||
              File_Node_Index_2 > Max_File_Node_Index ||
              File_Node_Index_3 > Max_File_Node_Index)
          {
            strcpy (Error_Message, "NASTRAN grid file contains TETRA with node index greater than in GRID");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          Vol_Tet_Connectivity[Vol_Tet_Index][0] = File_Node_Index_0;
          Vol_Tet_Connectivity[Vol_Tet_Index][1] = File_Node_Index_1;
          Vol_Tet_Connectivity[Vol_Tet_Index][2] = File_Node_Index_2;
          Vol_Tet_Connectivity[Vol_Tet_Index][3] = File_Node_Index_3;

          if (Vol_ID_Flag != NULL)
            Vol_ID_Flag[Vol_Tet_Index] = Vol_ID;
        }
      }

      else if (strcmp (Text_Field[0], "CPENTA") == 0 &&
               *Number_of_Bnd_Nodes == 0)
      {
        Read_Flag = sscanf (Text_Field[7], "%i", &File_Node_Index_4);
        Read_Flag = sscanf (Text_Field[8], "%i", &File_Node_Index_5);

        if (File_Node_Index_5 == 0 ||
            File_Node_Index_5 == File_Node_Index_4)
          ++Vol_Pent_5_Index;
        else
          ++Vol_Pent_6_Index;

        if (Read_Task_Flag == 2)
        {
          Read_Flag = sscanf (Text_Field[2], "%i", &Vol_ID);
          Read_Flag = sscanf (Text_Field[3], "%i", &File_Node_Index_0);
          Read_Flag = sscanf (Text_Field[4], "%i", &File_Node_Index_1);
          Read_Flag = sscanf (Text_Field[5], "%i", &File_Node_Index_2);
          Read_Flag = sscanf (Text_Field[6], "%i", &File_Node_Index_3);

          if (File_Node_Index_0 < 0 ||
              File_Node_Index_1 < 0 ||
              File_Node_Index_2 < 0 ||
              File_Node_Index_3 < 0 ||
              File_Node_Index_4 < 0 ||
              File_Node_Index_5 < 0)
          {
            strcpy (Error_Message, "NASTRAN grid file contains PENTA with negative node index");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          if (File_Node_Index_0 > Max_File_Node_Index ||
              File_Node_Index_1 > Max_File_Node_Index ||
              File_Node_Index_2 > Max_File_Node_Index ||
              File_Node_Index_3 > Max_File_Node_Index ||
              File_Node_Index_4 > Max_File_Node_Index ||
              File_Node_Index_5 > Max_File_Node_Index)
          {
            strcpy (Error_Message, "NASTRAN grid file contains PENTA with node index greater than in GRID");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          if (File_Node_Index_5 == 0 ||
              File_Node_Index_5 == File_Node_Index_4)
          {
            Vol_Pent_5_Connectivity[Vol_Pent_5_Index][0] = File_Node_Index_0;
            Vol_Pent_5_Connectivity[Vol_Pent_5_Index][1] = File_Node_Index_1;
            Vol_Pent_5_Connectivity[Vol_Pent_5_Index][2] = File_Node_Index_2;
            Vol_Pent_5_Connectivity[Vol_Pent_5_Index][3] = File_Node_Index_3;
            Vol_Pent_5_Connectivity[Vol_Pent_5_Index][4] = File_Node_Index_4;

            if (Vol_ID_Flag != NULL)
            {
              Vol_Index = (*Number_of_Vol_Tets) + Vol_Pent_5_Index;

              Vol_ID_Flag[Vol_Index] = Vol_ID;
            }
          }
          else
          {
            Vol_Pent_6_Connectivity[Vol_Pent_6_Index][0] = File_Node_Index_0;
            Vol_Pent_6_Connectivity[Vol_Pent_6_Index][1] = File_Node_Index_1;
            Vol_Pent_6_Connectivity[Vol_Pent_6_Index][2] = File_Node_Index_2;
            Vol_Pent_6_Connectivity[Vol_Pent_6_Index][3] = File_Node_Index_3;
            Vol_Pent_6_Connectivity[Vol_Pent_6_Index][4] = File_Node_Index_4;
            Vol_Pent_6_Connectivity[Vol_Pent_6_Index][5] = File_Node_Index_5;

            if (Vol_ID_Flag != NULL)
            {
              Vol_Index = (*Number_of_Vol_Tets) + (*Number_of_Vol_Pents_5)
                        + Vol_Pent_6_Index;

              Vol_ID_Flag[Vol_Index] = Vol_ID;
            }
          }
        }
      }

      else if (strcmp (Text_Field[0], "CHEXA") == 0 &&
               *Number_of_Bnd_Nodes == 0)
      {
        ++Vol_Hex_Index;

        if (Read_Task_Flag == 2)
        {
          Read_Flag = sscanf (Text_Field[2], "%i", &Vol_ID);
          Read_Flag = sscanf (Text_Field[3], "%i", &File_Node_Index_0);
          Read_Flag = sscanf (Text_Field[4], "%i", &File_Node_Index_1);
          Read_Flag = sscanf (Text_Field[5], "%i", &File_Node_Index_2);
          Read_Flag = sscanf (Text_Field[6], "%i", &File_Node_Index_3);
          Read_Flag = sscanf (Text_Field[7], "%i", &File_Node_Index_4);
          Read_Flag = sscanf (Text_Field[8], "%i", &File_Node_Index_5);
          Read_Flag = sscanf (Text_Field[9], "%i", &File_Node_Index_6);
          Read_Flag = sscanf (Text_Field[10], "%i", &File_Node_Index_7);

          if (File_Node_Index_0 < 0 ||
              File_Node_Index_1 < 0 ||
              File_Node_Index_2 < 0 ||
              File_Node_Index_3 < 0 ||
              File_Node_Index_4 < 0 ||
              File_Node_Index_5 < 0 ||
              File_Node_Index_6 < 0 ||
              File_Node_Index_7 < 0)
          {
            strcpy (Error_Message, "NASTRAN grid file contains HEXA with negative node index");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          if (File_Node_Index_0 > Max_File_Node_Index ||
              File_Node_Index_1 > Max_File_Node_Index ||
              File_Node_Index_2 > Max_File_Node_Index ||
              File_Node_Index_3 > Max_File_Node_Index ||
              File_Node_Index_4 > Max_File_Node_Index ||
              File_Node_Index_5 > Max_File_Node_Index ||
              File_Node_Index_6 > Max_File_Node_Index ||
              File_Node_Index_7 > Max_File_Node_Index)
          {
            strcpy (Error_Message, "NASTRAN grid file contains HEXA with node index greater than in GRID");
            ug_free (Node_Index_Map);
            ug_free (Text_Field);
            return (1);
          }

          Vol_Hex_Connectivity[Vol_Hex_Index][0] = File_Node_Index_0;
          Vol_Hex_Connectivity[Vol_Hex_Index][1] = File_Node_Index_1;
          Vol_Hex_Connectivity[Vol_Hex_Index][2] = File_Node_Index_2;
          Vol_Hex_Connectivity[Vol_Hex_Index][3] = File_Node_Index_3;
          Vol_Hex_Connectivity[Vol_Hex_Index][4] = File_Node_Index_4;
          Vol_Hex_Connectivity[Vol_Hex_Index][5] = File_Node_Index_5;
          Vol_Hex_Connectivity[Vol_Hex_Index][6] = File_Node_Index_6;
          Vol_Hex_Connectivity[Vol_Hex_Index][7] = File_Node_Index_7;

          if (Vol_ID_Flag != NULL)
          {
            Vol_Index = (*Number_of_Vol_Tets) + (*Number_of_Vol_Pents_5)
                      + (*Number_of_Vol_Pents_6) + Vol_Hex_Index;

            Vol_ID_Flag[Vol_Index] = Vol_ID;
          }
        }
      }
    }
  }
  while (Read_Label != NULL && UG_IO_NAS_Flag == 0);

  ug_free (Text_Field);

  if (UG_IO_NAS_Flag == 0)
  {
    *Number_of_Nodes = Node_Index;
    *Number_of_Surf_Trias = Surf_Tria_Index;
    *Number_of_Surf_Quads = Surf_Quad_Index;
    *Number_of_Vol_Tets = Vol_Tet_Index;
    *Number_of_Vol_Pents_5 = Vol_Pent_5_Index;
    *Number_of_Vol_Pents_6 = Vol_Pent_6_Index;
    *Number_of_Vol_Hexs = Vol_Hex_Index;

    *Number_of_BL_Vol_Tets = 0;

    if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
      *Number_of_Bnd_Nodes = Max_File_Bnd_Node_Index;

    Number_of_Total_Nodes = *Number_of_Nodes;

    if (*Number_of_Bnd_Nodes > 0)
    {
      *Number_of_Nodes = *Number_of_Bnd_Nodes;
      *Number_of_Vol_Hexs = 0;
      *Number_of_Vol_Pents_5 = 0;
      *Number_of_Vol_Pents_6 = 0;
      *Number_of_Vol_Tets = 0;
    }

    if (Read_Task_Flag == 2)
    {
      Number_of_Surf_Faces = (*Number_of_Surf_Trias) + (*Number_of_Surf_Quads);

      for (Surf_Tria_Index = 1;
           Surf_Tria_Index <= *Number_of_Surf_Trias; ++Surf_Tria_Index)
      {
        File_Node_Index_0 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
        File_Node_Index_1 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
        File_Node_Index_2 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

        Node_Index_0 = Node_Index_Map[File_Node_Index_0];
        Node_Index_1 = Node_Index_Map[File_Node_Index_1];
        Node_Index_2 = Node_Index_Map[File_Node_Index_2];

        if (Node_Index_0 == -1 ||
            Node_Index_1 == -1 ||
            Node_Index_2 == -1)
        {
          strcpy (Error_Message, "NASTRAN grid file contains TRIA3 with node index that does not exist");
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }

        Surf_Tria_Connectivity[Surf_Tria_Index][0] = Node_Index_0;
        Surf_Tria_Connectivity[Surf_Tria_Index][1] = Node_Index_1;
        Surf_Tria_Connectivity[Surf_Tria_Index][2] = Node_Index_2;
      }

      for (Surf_Quad_Index = 1;
           Surf_Quad_Index <= *Number_of_Surf_Quads; ++Surf_Quad_Index)
      {
        File_Node_Index_0 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
        File_Node_Index_1 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
        File_Node_Index_2 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
        File_Node_Index_3 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

        Node_Index_0 = Node_Index_Map[File_Node_Index_0];
        Node_Index_1 = Node_Index_Map[File_Node_Index_1];
        Node_Index_2 = Node_Index_Map[File_Node_Index_2];
        Node_Index_3 = Node_Index_Map[File_Node_Index_3];

        if (Node_Index_0 == -1 ||
            Node_Index_1 == -1 ||
            Node_Index_2 == -1 ||
            Node_Index_3 == -1)
        {
          strcpy (Error_Message, "NASTRAN grid file contains QUAD4 with node index that does not exist");
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }

        Surf_Quad_Connectivity[Surf_Quad_Index][0] = Node_Index_0;
        Surf_Quad_Connectivity[Surf_Quad_Index][1] = Node_Index_1;
        Surf_Quad_Connectivity[Surf_Quad_Index][2] = Node_Index_2;
        Surf_Quad_Connectivity[Surf_Quad_Index][3] = Node_Index_3;
      }

      for (Vol_Tet_Index = 1;
           Vol_Tet_Index <= *Number_of_Vol_Tets; ++Vol_Tet_Index)
      {
        File_Node_Index_0 = Vol_Tet_Connectivity[Vol_Tet_Index][0];
        File_Node_Index_1 = Vol_Tet_Connectivity[Vol_Tet_Index][1];
        File_Node_Index_2 = Vol_Tet_Connectivity[Vol_Tet_Index][2];
        File_Node_Index_3 = Vol_Tet_Connectivity[Vol_Tet_Index][3];

        Node_Index_0 = Node_Index_Map[File_Node_Index_0];
        Node_Index_1 = Node_Index_Map[File_Node_Index_1];
        Node_Index_2 = Node_Index_Map[File_Node_Index_2];
        Node_Index_3 = Node_Index_Map[File_Node_Index_3];

        if (Node_Index_0 == -1 ||
            Node_Index_1 == -1 ||
            Node_Index_2 == -1 ||
            Node_Index_3 == -1)
        {
          strcpy (Error_Message, "NASTRAN grid file contains TETRA with node index that does not exist");
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }

        Vol_Tet_Connectivity[Vol_Tet_Index][0] = Node_Index_0;
        Vol_Tet_Connectivity[Vol_Tet_Index][1] = Node_Index_1;
        Vol_Tet_Connectivity[Vol_Tet_Index][2] = Node_Index_2;
        Vol_Tet_Connectivity[Vol_Tet_Index][3] = Node_Index_3;
      }

      for (Vol_Pent_5_Index = 1;
           Vol_Pent_5_Index <= *Number_of_Vol_Pents_5; ++Vol_Pent_5_Index)
      {
        File_Node_Index_0 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][0];
        File_Node_Index_1 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][1];
        File_Node_Index_2 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][2];
        File_Node_Index_3 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][3];
        File_Node_Index_4 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][4];

        Node_Index_0 = Node_Index_Map[File_Node_Index_0];
        Node_Index_1 = Node_Index_Map[File_Node_Index_1];
        Node_Index_2 = Node_Index_Map[File_Node_Index_2];
        Node_Index_3 = Node_Index_Map[File_Node_Index_3];
        Node_Index_4 = Node_Index_Map[File_Node_Index_4];

        if (Node_Index_0 == -1 ||
            Node_Index_1 == -1 ||
            Node_Index_2 == -1 ||
            Node_Index_3 == -1 ||
            Node_Index_4 == -1)
        {
          strcpy (Error_Message, "NASTRAN grid file contains PENTA with node index that does not exist");
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }

        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][0] = Node_Index_0;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][1] = Node_Index_1;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][2] = Node_Index_2;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][3] = Node_Index_3;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][4] = Node_Index_4;
      }

      for (Vol_Pent_6_Index = 1;
           Vol_Pent_6_Index <= *Number_of_Vol_Pents_6; ++Vol_Pent_6_Index)
      {
        File_Node_Index_0 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][0];
        File_Node_Index_1 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][1];
        File_Node_Index_2 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][2];
        File_Node_Index_3 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][3];
        File_Node_Index_4 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][4];
        File_Node_Index_5 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][5];

        Node_Index_0 = Node_Index_Map[File_Node_Index_0];
        Node_Index_1 = Node_Index_Map[File_Node_Index_1];
        Node_Index_2 = Node_Index_Map[File_Node_Index_2];
        Node_Index_3 = Node_Index_Map[File_Node_Index_3];
        Node_Index_4 = Node_Index_Map[File_Node_Index_4];
        Node_Index_5 = Node_Index_Map[File_Node_Index_5];

        if (Node_Index_0 == -1 ||
            Node_Index_1 == -1 ||
            Node_Index_2 == -1 ||
            Node_Index_3 == -1 ||
            Node_Index_4 == -1 ||
            Node_Index_5 == -1)
        {
          strcpy (Error_Message, "NASTRAN grid file contains PENTA with node index that does not exist");
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }

        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][0] = Node_Index_0;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][1] = Node_Index_1;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][2] = Node_Index_2;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][3] = Node_Index_3;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][4] = Node_Index_4;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][5] = Node_Index_5;
      }

      for (Vol_Hex_Index = 1;
           Vol_Hex_Index <= *Number_of_Vol_Hexs; ++Vol_Hex_Index)
      {
        File_Node_Index_0 = Vol_Hex_Connectivity[Vol_Hex_Index][0];
        File_Node_Index_1 = Vol_Hex_Connectivity[Vol_Hex_Index][1];
        File_Node_Index_2 = Vol_Hex_Connectivity[Vol_Hex_Index][2];
        File_Node_Index_3 = Vol_Hex_Connectivity[Vol_Hex_Index][3];
        File_Node_Index_4 = Vol_Hex_Connectivity[Vol_Hex_Index][4];
        File_Node_Index_5 = Vol_Hex_Connectivity[Vol_Hex_Index][5];
        File_Node_Index_6 = Vol_Hex_Connectivity[Vol_Hex_Index][6];
        File_Node_Index_7 = Vol_Hex_Connectivity[Vol_Hex_Index][7];

        Node_Index_0 = Node_Index_Map[File_Node_Index_0];
        Node_Index_1 = Node_Index_Map[File_Node_Index_1];
        Node_Index_2 = Node_Index_Map[File_Node_Index_2];
        Node_Index_3 = Node_Index_Map[File_Node_Index_3];
        Node_Index_4 = Node_Index_Map[File_Node_Index_4];
        Node_Index_5 = Node_Index_Map[File_Node_Index_5];
        Node_Index_6 = Node_Index_Map[File_Node_Index_6];
        Node_Index_7 = Node_Index_Map[File_Node_Index_7];

        if (Node_Index_0 == -1 ||
            Node_Index_1 == -1 ||
            Node_Index_2 == -1 ||
            Node_Index_3 == -1 ||
            Node_Index_4 == -1 ||
            Node_Index_5 == -1 ||
            Node_Index_6 == -1 ||
            Node_Index_7 == -1)
        {
          strcpy (Error_Message, "NASTRAN grid file contains HEXA with node index that does not exist");
          ug_free (Node_Index_Map);
          ug_free (Text_Field);
          return (1);
        }

        Vol_Hex_Connectivity[Vol_Hex_Index][0] = Node_Index_0;
        Vol_Hex_Connectivity[Vol_Hex_Index][1] = Node_Index_1;
        Vol_Hex_Connectivity[Vol_Hex_Index][2] = Node_Index_2;
        Vol_Hex_Connectivity[Vol_Hex_Index][3] = Node_Index_3;
        Vol_Hex_Connectivity[Vol_Hex_Index][4] = Node_Index_4;
        Vol_Hex_Connectivity[Vol_Hex_Index][5] = Node_Index_5;
        Vol_Hex_Connectivity[Vol_Hex_Index][6] = Node_Index_6;
        Vol_Hex_Connectivity[Vol_Hex_Index][7] = Node_Index_7;
      }

      for (Node_Index = 1; Node_Index <= *Number_of_Nodes; ++Node_Index)
      {
        Node_Index_Map[Node_Index] = 0;
      }

      for (Surf_Tria_Index = 1;
           Surf_Tria_Index <= *Number_of_Surf_Trias; ++Surf_Tria_Index)
      {
        Node_Index_0 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
        Node_Index_1 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
        Node_Index_2 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

        Node_Index_Map[Node_Index_0] = 1;
        Node_Index_Map[Node_Index_1] = 1;
        Node_Index_Map[Node_Index_2] = 1;
      }

      for (Surf_Quad_Index = 1;
           Surf_Quad_Index <= *Number_of_Surf_Quads; ++Surf_Quad_Index)
      {
        Node_Index_0 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
        Node_Index_1 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
        Node_Index_2 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
        Node_Index_3 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

        Node_Index_Map[Node_Index_0] = 1;
        Node_Index_Map[Node_Index_1] = 1;
        Node_Index_Map[Node_Index_2] = 1;
        Node_Index_Map[Node_Index_3] = 1;
      }

      for (Vol_Tet_Index = 1;
           Vol_Tet_Index <= *Number_of_Vol_Tets; ++Vol_Tet_Index)
      {
        Node_Index_0 = Vol_Tet_Connectivity[Vol_Tet_Index][0];
        Node_Index_1 = Vol_Tet_Connectivity[Vol_Tet_Index][1];
        Node_Index_2 = Vol_Tet_Connectivity[Vol_Tet_Index][2];
        Node_Index_3 = Vol_Tet_Connectivity[Vol_Tet_Index][3];

        Node_Index_Map[Node_Index_0] = 1;
        Node_Index_Map[Node_Index_1] = 1;
        Node_Index_Map[Node_Index_2] = 1;
        Node_Index_Map[Node_Index_3] = 1;
      }

      for (Vol_Pent_5_Index = 1;
           Vol_Pent_5_Index <= *Number_of_Vol_Pents_5; ++Vol_Pent_5_Index)
      {
        Node_Index_0 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][0];
        Node_Index_1 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][1];
        Node_Index_2 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][2];
        Node_Index_3 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][3];
        Node_Index_4 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][4];

        Node_Index_Map[Node_Index_0] = 1;
        Node_Index_Map[Node_Index_1] = 1;
        Node_Index_Map[Node_Index_2] = 1;
        Node_Index_Map[Node_Index_3] = 1;
        Node_Index_Map[Node_Index_4] = 1;
      }

      for (Vol_Pent_6_Index = 1;
           Vol_Pent_6_Index <= *Number_of_Vol_Pents_6; ++Vol_Pent_6_Index)
      {
        Node_Index_0 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][0];
        Node_Index_1 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][1];
        Node_Index_2 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][2];
        Node_Index_3 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][3];
        Node_Index_4 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][4];
        Node_Index_5 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][5];

        Node_Index_Map[Node_Index_0] = 1;
        Node_Index_Map[Node_Index_1] = 1;
        Node_Index_Map[Node_Index_2] = 1;
        Node_Index_Map[Node_Index_3] = 1;
        Node_Index_Map[Node_Index_4] = 1;
        Node_Index_Map[Node_Index_5] = 1;
      }

      for (Vol_Hex_Index = 1;
           Vol_Hex_Index <= *Number_of_Vol_Hexs; ++Vol_Hex_Index)
      {
        Node_Index_0 = Vol_Hex_Connectivity[Vol_Hex_Index][0];
        Node_Index_1 = Vol_Hex_Connectivity[Vol_Hex_Index][1];
        Node_Index_2 = Vol_Hex_Connectivity[Vol_Hex_Index][2];
        Node_Index_3 = Vol_Hex_Connectivity[Vol_Hex_Index][3];
        Node_Index_4 = Vol_Hex_Connectivity[Vol_Hex_Index][4];
        Node_Index_5 = Vol_Hex_Connectivity[Vol_Hex_Index][5];
        Node_Index_6 = Vol_Hex_Connectivity[Vol_Hex_Index][6];
        Node_Index_7 = Vol_Hex_Connectivity[Vol_Hex_Index][7];

        Node_Index_Map[Node_Index_0] = 1;
        Node_Index_Map[Node_Index_1] = 1;
        Node_Index_Map[Node_Index_2] = 1;
        Node_Index_Map[Node_Index_3] = 1;
        Node_Index_Map[Node_Index_4] = 1;
        Node_Index_Map[Node_Index_5] = 1;
        Node_Index_Map[Node_Index_6] = 1;
        Node_Index_Map[Node_Index_7] = 1;
      }

      New_Node_Index = 0;

      for (Node_Index = 1; Node_Index <= *Number_of_Nodes; ++Node_Index)
      {
        if (Node_Index_Map[Node_Index] == 1)
        {
          ++New_Node_Index;

          Node_Index_Map[Node_Index] = New_Node_Index;

          Coordinates[New_Node_Index][0] = Coordinates[Node_Index][0];
          Coordinates[New_Node_Index][1] = Coordinates[Node_Index][1];
          Coordinates[New_Node_Index][2] = Coordinates[Node_Index][2];
        }
      }

      *Number_of_Nodes = New_Node_Index;

      for (Surf_Tria_Index = 1;
           Surf_Tria_Index <= *Number_of_Surf_Trias; ++Surf_Tria_Index)
      {
        Node_Index_0 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
        Node_Index_1 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
        Node_Index_2 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

        New_Node_Index_0 = Node_Index_Map[Node_Index_0];
        New_Node_Index_1 = Node_Index_Map[Node_Index_1];
        New_Node_Index_2 = Node_Index_Map[Node_Index_2];

        Surf_Tria_Connectivity[Surf_Tria_Index][0] = New_Node_Index_0;
        Surf_Tria_Connectivity[Surf_Tria_Index][1] = New_Node_Index_1;
        Surf_Tria_Connectivity[Surf_Tria_Index][2] = New_Node_Index_2;
      }

      for (Surf_Quad_Index = 1;
           Surf_Quad_Index <= *Number_of_Surf_Quads; ++Surf_Quad_Index)
      {
        Node_Index_0 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
        Node_Index_1 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
        Node_Index_2 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
        Node_Index_3 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

        New_Node_Index_0 = Node_Index_Map[Node_Index_0];
        New_Node_Index_1 = Node_Index_Map[Node_Index_1];
        New_Node_Index_2 = Node_Index_Map[Node_Index_2];
        New_Node_Index_3 = Node_Index_Map[Node_Index_3];

        Surf_Quad_Connectivity[Surf_Quad_Index][0] = New_Node_Index_0;
        Surf_Quad_Connectivity[Surf_Quad_Index][1] = New_Node_Index_1;
        Surf_Quad_Connectivity[Surf_Quad_Index][2] = New_Node_Index_2;
        Surf_Quad_Connectivity[Surf_Quad_Index][3] = New_Node_Index_3;
      }

      for (Vol_Tet_Index = 1;
           Vol_Tet_Index <= *Number_of_Vol_Tets; ++Vol_Tet_Index)
      {
        Node_Index_0 = Vol_Tet_Connectivity[Vol_Tet_Index][0];
        Node_Index_1 = Vol_Tet_Connectivity[Vol_Tet_Index][1];
        Node_Index_2 = Vol_Tet_Connectivity[Vol_Tet_Index][2];
        Node_Index_3 = Vol_Tet_Connectivity[Vol_Tet_Index][3];

        New_Node_Index_0 = Node_Index_Map[Node_Index_0];
        New_Node_Index_1 = Node_Index_Map[Node_Index_1];
        New_Node_Index_2 = Node_Index_Map[Node_Index_2];
        New_Node_Index_3 = Node_Index_Map[Node_Index_3];

        Vol_Tet_Connectivity[Vol_Tet_Index][0] = New_Node_Index_0;
        Vol_Tet_Connectivity[Vol_Tet_Index][1] = New_Node_Index_1;
        Vol_Tet_Connectivity[Vol_Tet_Index][2] = New_Node_Index_2;
        Vol_Tet_Connectivity[Vol_Tet_Index][3] = New_Node_Index_3;
      }

      for (Vol_Pent_5_Index = 1;
           Vol_Pent_5_Index <= *Number_of_Vol_Pents_5; ++Vol_Pent_5_Index)
      {
        Node_Index_0 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][0];
        Node_Index_1 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][1];
        Node_Index_2 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][2];
        Node_Index_3 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][3];
        Node_Index_4 = Vol_Pent_5_Connectivity[Vol_Pent_5_Index][4];

        New_Node_Index_0 = Node_Index_Map[Node_Index_0];
        New_Node_Index_1 = Node_Index_Map[Node_Index_1];
        New_Node_Index_2 = Node_Index_Map[Node_Index_2];
        New_Node_Index_3 = Node_Index_Map[Node_Index_3];
        New_Node_Index_4 = Node_Index_Map[Node_Index_4];

        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][0] = New_Node_Index_0;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][1] = New_Node_Index_1;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][2] = New_Node_Index_2;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][3] = New_Node_Index_3;
        Vol_Pent_5_Connectivity[Vol_Pent_5_Index][4] = New_Node_Index_4;
      }

      for (Vol_Pent_6_Index = 1;
           Vol_Pent_6_Index <= *Number_of_Vol_Pents_6; ++Vol_Pent_6_Index)
      {
        Node_Index_0 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][0];
        Node_Index_1 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][1];
        Node_Index_2 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][2];
        Node_Index_3 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][3];
        Node_Index_4 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][4];
        Node_Index_5 = Vol_Pent_6_Connectivity[Vol_Pent_6_Index][5];

        New_Node_Index_0 = Node_Index_Map[Node_Index_0];
        New_Node_Index_1 = Node_Index_Map[Node_Index_1];
        New_Node_Index_2 = Node_Index_Map[Node_Index_2];
        New_Node_Index_3 = Node_Index_Map[Node_Index_3];
        New_Node_Index_4 = Node_Index_Map[Node_Index_4];
        New_Node_Index_5 = Node_Index_Map[Node_Index_5];

        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][0] = New_Node_Index_0;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][1] = New_Node_Index_1;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][2] = New_Node_Index_2;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][3] = New_Node_Index_3;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][4] = New_Node_Index_4;
        Vol_Pent_6_Connectivity[Vol_Pent_6_Index][5] = New_Node_Index_5;
      }

      for (Vol_Hex_Index = 1;
           Vol_Hex_Index <= *Number_of_Vol_Hexs; ++Vol_Hex_Index)
      {
        Node_Index_0 = Vol_Hex_Connectivity[Vol_Hex_Index][0];
        Node_Index_1 = Vol_Hex_Connectivity[Vol_Hex_Index][1];
        Node_Index_2 = Vol_Hex_Connectivity[Vol_Hex_Index][2];
        Node_Index_3 = Vol_Hex_Connectivity[Vol_Hex_Index][3];
        Node_Index_4 = Vol_Hex_Connectivity[Vol_Hex_Index][4];
        Node_Index_5 = Vol_Hex_Connectivity[Vol_Hex_Index][5];
        Node_Index_6 = Vol_Hex_Connectivity[Vol_Hex_Index][6];
        Node_Index_7 = Vol_Hex_Connectivity[Vol_Hex_Index][7];

        New_Node_Index_0 = Node_Index_Map[Node_Index_0];
        New_Node_Index_1 = Node_Index_Map[Node_Index_1];
        New_Node_Index_2 = Node_Index_Map[Node_Index_2];
        New_Node_Index_3 = Node_Index_Map[Node_Index_3];
        New_Node_Index_4 = Node_Index_Map[Node_Index_4];
        New_Node_Index_5 = Node_Index_Map[Node_Index_5];
        New_Node_Index_6 = Node_Index_Map[Node_Index_6];
        New_Node_Index_7 = Node_Index_Map[Node_Index_7];

        Vol_Hex_Connectivity[Vol_Hex_Index][0] = New_Node_Index_0;
        Vol_Hex_Connectivity[Vol_Hex_Index][1] = New_Node_Index_1;
        Vol_Hex_Connectivity[Vol_Hex_Index][2] = New_Node_Index_2;
        Vol_Hex_Connectivity[Vol_Hex_Index][3] = New_Node_Index_3;
        Vol_Hex_Connectivity[Vol_Hex_Index][4] = New_Node_Index_4;
        Vol_Hex_Connectivity[Vol_Hex_Index][5] = New_Node_Index_5;
        Vol_Hex_Connectivity[Vol_Hex_Index][6] = New_Node_Index_6;
        Vol_Hex_Connectivity[Vol_Hex_Index][7] = New_Node_Index_7;
      }

      ug_free (Node_Index_Map);
    }
  }

  else
  {
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_BL_Vol_Tets);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", &Number_of_BNodes);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Nodes);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Surf_Quads);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Surf_Trias);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Hexs);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Pents_5);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Pents_6);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%s", Text_Line);

    Read_Flag = fscanf (Grid_File, "%s", Text_Line);
    Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Tets);

    if (Read_Flag <= 0)
    {
      strcpy (Error_Message, "error reading UG_IO NASTRAN grid file");
      return (1);
    }

    Number_of_Surf_Faces = (*Number_of_Surf_Trias) + (*Number_of_Surf_Quads);

    Number_of_Total_Nodes = *Number_of_Nodes;

    if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
     *Number_of_Bnd_Nodes = Number_of_BNodes;

    if (*Number_of_Bnd_Nodes > 0)
      *Number_of_Nodes = *Number_of_Bnd_Nodes;

    if (Read_Task_Flag == 2)
    {
      Read_Flag = fscanf (Grid_File, "%s", Text_Line);

      Read_Flag = fscanf (Grid_File, "%s", Text_Line);
      Read_Flag = fscanf (Grid_File, "%s", Text_Line);

      Read_Flag = fscanf (Grid_File, "%s", Text_Line);
      Read_Flag = fscanf (Grid_File, "%s", Text_Line);

      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
        Read_Flag = fscanf (Grid_File, "%lg", &Coordinates[Index][0]);
        Read_Flag = fscanf (Grid_File, "%lg", &Coordinates[Index][1]);
        Read_Flag = fscanf (Grid_File, "%8s", Text_Line);

        Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%lg", &Coordinates[Index][2]);
      }

      for (Index = *Number_of_Nodes+1; Index <= Number_of_Total_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
        Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
        Read_Flag = fscanf (Grid_File, "%lg", &TMP);
        Read_Flag = fscanf (Grid_File, "%lg", &TMP);
        Read_Flag = fscanf (Grid_File, "%8s", Text_Line);

        Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%lg", &TMP);
      }

      if (Read_Flag <= 0)
      {
        strcpy (Error_Message, "error reading UG_IO NASTRAN grid file");
        return (1);
      }

      Surf_Index = 0;

      if (*Number_of_Surf_Trias > 0)
      {
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
          Read_Flag = fscanf (Grid_File, "%i", &Surf_ID);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_0);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_1);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_2);

          if (Surf_ID_Flag != NULL)
          {
            ++Surf_Index;

            Surf_ID_Flag[Surf_Index] = Surf_ID;
          }

          Surf_Tria_Connectivity[Index][0] = Node_Index_0;
          Surf_Tria_Connectivity[Index][1] = Node_Index_1;
          Surf_Tria_Connectivity[Index][2] = Node_Index_2;
        }

        if (Read_Flag <= 0)
        {
          strcpy (Error_Message, "error reading of UG_IO NASTRAN grid file");
          return (1);
        }
      }

      if (*Number_of_Surf_Quads > 0)
      {
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
          Read_Flag = fscanf (Grid_File, "%i", &Surf_ID);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_0);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_1);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_2);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_3);

          if (Surf_ID_Flag != NULL)
          {
            ++Surf_Index;

            Surf_ID_Flag[Surf_Index] = Surf_ID;
          }

          Surf_Quad_Connectivity[Index][0] = Node_Index_0;
          Surf_Quad_Connectivity[Index][1] = Node_Index_1;
          Surf_Quad_Connectivity[Index][2] = Node_Index_2;
          Surf_Quad_Connectivity[Index][3] = Node_Index_3;
        }

        if (Read_Flag <= 0)
        {
          strcpy (Error_Message, "error reading UG_IO NASTRAN grid file");
          return (1);
        }
      }

      Vol_Index = 0;

      if (*Number_of_Vol_Tets > 0)
      {
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= *Number_of_Vol_Tets; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
          Read_Flag = fscanf (Grid_File, "%i", &Vol_ID);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_0);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_1);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_2);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_3);

          if (*Number_of_Bnd_Nodes == 0)
          {
            if (Vol_ID_Flag != NULL)
            {
              ++Vol_Index;

              Vol_ID_Flag[Vol_Index] = Vol_ID;
            }

            Vol_Tet_Connectivity[Index][0] = Node_Index_0;
            Vol_Tet_Connectivity[Index][1] = Node_Index_1;
            Vol_Tet_Connectivity[Index][2] = Node_Index_2;
            Vol_Tet_Connectivity[Index][3] = Node_Index_3;
          }
        }

        if (Read_Flag <= 0)
        {
          strcpy (Error_Message, "error reading UG_IO NASTRAN grid file");
          return (1);
        }
      }

      if (*Number_of_Vol_Pents_5 > 0)
      {
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= *Number_of_Vol_Pents_5; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
          Read_Flag = fscanf (Grid_File, "%i", &Vol_ID);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_0);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_1);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_2);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_3);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_4);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);

          if (*Number_of_Bnd_Nodes == 0)
          {
            if (Vol_ID_Flag != NULL)
            {
              ++Vol_Index;

              Vol_ID_Flag[Vol_Index] = Vol_ID;
            }

            Vol_Pent_5_Connectivity[Index][0] = Node_Index_0;
            Vol_Pent_5_Connectivity[Index][1] = Node_Index_1;
            Vol_Pent_5_Connectivity[Index][2] = Node_Index_2;
            Vol_Pent_5_Connectivity[Index][3] = Node_Index_3;
            Vol_Pent_5_Connectivity[Index][4] = Node_Index_4;
          }
        }

        if (Read_Flag <= 0)
        {
          strcpy (Error_Message, "error reading UG_IO NASTRAN grid file");
          return (1);
        }
      }

      if (*Number_of_Vol_Pents_6 > 0)
      {
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= *Number_of_Vol_Pents_6; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
          Read_Flag = fscanf (Grid_File, "%i", &Vol_ID);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_0);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_1);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_2);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_3);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_4);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_5);

          if (*Number_of_Bnd_Nodes == 0)
          {
            if (Vol_ID_Flag != NULL)
            {
              ++Vol_Index;

              Vol_ID_Flag[Vol_Index] = Vol_ID;
            }

            Vol_Pent_6_Connectivity[Index][0] = Node_Index_0;
            Vol_Pent_6_Connectivity[Index][1] = Node_Index_1;
            Vol_Pent_6_Connectivity[Index][2] = Node_Index_2;
            Vol_Pent_6_Connectivity[Index][3] = Node_Index_3;
            Vol_Pent_6_Connectivity[Index][4] = Node_Index_4;
            Vol_Pent_6_Connectivity[Index][5] = Node_Index_5;
          }
        }

        if (Read_Flag <= 0)
        {
          strcpy (Error_Message, "error reading UG_IO NASTRAN grid file");
          return (1);
        }
      }

      if (*Number_of_Vol_Hexs > 0)
      {
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= *Number_of_Vol_Hexs; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);
          Read_Flag = fscanf (Grid_File, "%i", &Vol_ID);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_0);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_1);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_2);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_3);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_4);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_5);
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%8s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_6);
          Read_Flag = fscanf (Grid_File, "%i", &Node_Index_7);

          if (*Number_of_Bnd_Nodes == 0)
          {
            if (Vol_ID_Flag != NULL)
            {
              ++Vol_Index;

              Vol_ID_Flag[Vol_Index] = Vol_ID;
            }

            Vol_Hex_Connectivity[Index][0] = Node_Index_0;
            Vol_Hex_Connectivity[Index][1] = Node_Index_1;
            Vol_Hex_Connectivity[Index][2] = Node_Index_2;
            Vol_Hex_Connectivity[Index][3] = Node_Index_3;
            Vol_Hex_Connectivity[Index][4] = Node_Index_4;
            Vol_Hex_Connectivity[Index][5] = Node_Index_5;
            Vol_Hex_Connectivity[Index][6] = Node_Index_6;
            Vol_Hex_Connectivity[Index][7] = Node_Index_7;
          }
        }

        if (Read_Flag <= 0)
        {
          strcpy (Error_Message, "error reading UG_IO NASTRAN grid file");
          return (1);
        }
      }

      if (Number_of_Surf_Faces > 0)
      {
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= Number_of_BNodes; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%lg", &TMP);

          if (Initial_Normal_Spacing != NULL)
            Initial_Normal_Spacing[Index] = TMP;
        }

        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= Number_of_BNodes; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%lg", &TMP);

          if (BL_Thickness != NULL)
            BL_Thickness[Index] = TMP;
        }

        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);

          if (Surf_Grid_BC_Flag != NULL)
            Surf_Grid_BC_Flag[Index] = TMP_INT_;
        }

        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        Read_Flag = fscanf (Grid_File, "%s", Text_Line);
        Read_Flag = fscanf (Grid_File, "%s", Text_Line);

        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%s", Text_Line);
          Read_Flag = fscanf (Grid_File, "%i", &TMP_INT_);

          if (Surf_Reconnection_Flag != NULL)
            Surf_Reconnection_Flag[Index] = TMP_INT_;
        }
      }
    }
  }

  return (0);

}
