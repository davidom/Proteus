#include "UG_IO_LIB.h"

INT_ ug_io_read_tags
 (FILE * Tags_Data_File,
  UG_Param_Struct * UG_Param_Struct_Ptr,
  INT_ Message_Flag,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_1D * Surf_ID_Flag)

{

/*
 * Read data from a TAGS data file and set grid BC and reconnection flag at
 * surface faces along with initial normal spacing, BL thickness, and maximum
 * number of BLs for each surface ID.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_tags.c,v 1.18 2012/11/08 22:16:18 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Label;
  CHAR_UG_MAX Text;

  char *Text_;

  INT_1D *BC_IDs;
  INT_1D *Number_of_BLs;
  INT_1D *Grid_BC_Flag;
  INT_1D *Rec_Flag;
  INT_1D *Surf_ID_Flag_Map;
  DOUBLE_1D *BL_Initial_Normal_Spacing;
  DOUBLE_1D *BL_Thickness;

  INT_ BC_BL_Flag, BC_Fixed_Flag, BC_ID, BC_Rebuild_Flag, BC_Rec_Flag,
       BC_Source_Flag, BC_Transp_Flag, BC_Transp_Delete_Flag, Error_Flag,
       Found_Flag, Grid_BC_Flag_, IDm, Length, Max_ID, Number_of_BC_IDs,
       Number_of_BLs_, Number_of_Faces, Number_of_IDs, Read_Flag, Set_BC_Data,
       Set_Flag, Surf_ID_Flag_;

  double dc0, BL_Initial_Normal_Spacing_, BL_Thickness_;

  dc0 = 0.0;

  Number_of_Faces =  Number_of_Surf_Quads + Number_of_Surf_Trias;

  Error_Flag = ug_ivivm (Number_of_Faces, &Number_of_IDs, Surf_ID_Flag, &Surf_ID_Flag_Map);

  if (Error_Flag)
  {
    ug_free (Surf_ID_Flag_Map);
    return (1);
  }

  BC_IDs = (INT_1D *) ug_malloc (&Error_Flag, (Number_of_IDs+1) * sizeof (INT_1D));
  BL_Initial_Normal_Spacing = (DOUBLE_1D *) ug_malloc (&Error_Flag, (Number_of_IDs+1) * sizeof (DOUBLE_1D));
  BL_Thickness = (DOUBLE_1D *) ug_malloc (&Error_Flag, (Number_of_IDs+1) * sizeof (DOUBLE_1D));
  Grid_BC_Flag = (INT_1D *) ug_malloc (&Error_Flag, (Number_of_IDs+1) * sizeof (INT_1D));
  Number_of_BLs = (INT_1D *) ug_malloc (&Error_Flag, (Number_of_IDs+1) * sizeof (INT_1D));
  Rec_Flag = (INT_1D *) ug_malloc (&Error_Flag, (Number_of_IDs+1) * sizeof (INT_1D));

  if (Error_Flag)
  {
    ug_free (BC_IDs);
    ug_free (BL_Initial_Normal_Spacing);
    ug_free (BL_Thickness);
    ug_free (Grid_BC_Flag);
    ug_free (Number_of_BLs);
    ug_free (Rec_Flag);
    ug_free (Surf_ID_Flag_Map);
    return (1);
  }

  IDm = 1;

  Max_ID = Surf_ID_Flag_Map[IDm];

  for (IDm = 2; IDm <= Number_of_IDs; ++ IDm)
  {
    Max_ID = MAX (Max_ID, Surf_ID_Flag_Map[IDm]);
  }

  for (IDm = 1; IDm <= Number_of_IDs; ++ IDm)
  {
    BC_IDs[IDm] = Max_ID+1;
  }

  Set_BC_Data = 0;

  do
  {
    Text_ = &Text[0];

    Text_ = fgets (Text_, (int) UG_MAX_CHAR_STRING_LENGTH, Tags_Data_File);

    if (Text_ != NULL)
    {
      Length = (INT_) strlen (Text_);

      if (Length > 1)
      {
        if (strncmp (Text_, "#", 1) != 0)
          Read_Flag = 1;
        else
          Read_Flag = 0;
      }
      else
        Read_Flag = 0;
    }
    else
      Read_Flag = 0;

    if (Read_Flag == 1)
    {
      Read_Flag = sscanf (Text_, "%s %s %s %s %s %s %s %s %s %s %s %s",
                          Label, Label, Label, Label, Label, Label,
                          Label, Label, Label, Label, Label, Label);

      if (Read_Flag == 12)
        Read_Flag = sscanf (Text_, "%i %s %i %i %i %i %i %i %i %lg %lg %i",
                            &Surf_ID_Flag_, Label,
                            &BC_BL_Flag, &BC_Rec_Flag, &BC_Rebuild_Flag,
                            &BC_Fixed_Flag, &BC_Source_Flag, &BC_Transp_Flag,
                            &BC_Transp_Delete_Flag,
                            &BL_Initial_Normal_Spacing_, &BL_Thickness_,
                            &Number_of_BLs_);

      else if (Read_Flag == 11)
      {
        Read_Flag = sscanf (Text_, "%i %s %i %i %i %i %i %i %i %lg %lg",
                            &Surf_ID_Flag_, Label,
                            &BC_BL_Flag, &BC_Rec_Flag, &BC_Rebuild_Flag,
                            &BC_Fixed_Flag, &BC_Source_Flag, &BC_Transp_Flag,
                            &BC_Transp_Delete_Flag,
                            &BL_Initial_Normal_Spacing_, &BL_Thickness_);

        Number_of_BLs_ = 0;
      }

      else if (Read_Flag == 10)
      {
        Read_Flag = sscanf (Text_, "%i %s %i %i %i %i %i %i %lg %lg",
                            &Surf_ID_Flag_, Label,
                            &BC_BL_Flag, &BC_Rec_Flag, &BC_Rebuild_Flag,
                            &BC_Source_Flag, &BC_Transp_Flag,
                            &BC_Transp_Delete_Flag,
                            &BL_Initial_Normal_Spacing_, &BL_Thickness_);

        BC_Fixed_Flag = (BC_Rec_Flag == 0 && BC_Rebuild_Flag == 1) ? 1: 0;

        Number_of_BLs_ = 0;
      }

      else
        Read_Flag = 0;
    }

    if (Read_Flag)
    {
      Grid_BC_Flag_ = (BC_Transp_Delete_Flag) ? TRANSP_INTRNL_UG3_GBC:
                      (BC_Transp_Flag && ! BC_Rebuild_Flag) ? TRANSP_UG3_GBC:
                      (BC_Transp_Flag && BC_Rebuild_Flag) ? TRANSP_BL_INT_UG3_GBC:
                      (BC_Source_Flag) ? TRANSP_SRC_UG3_GBC:
                      (BC_Fixed_Flag) ? FIXED_BL_INT_UG3_GBC:
                      (BC_Rebuild_Flag) ? BL_INT_UG3_GBC: STD_UG3_GBC;

      if (BC_BL_Flag)
        Grid_BC_Flag_ = -Grid_BC_Flag_;

      IDm = 1;

      do
      {
        Found_Flag = (Surf_ID_Flag_ == Surf_ID_Flag_Map[IDm]) ? 1: 0;

        ++IDm;
      }
      while (IDm <= Number_of_IDs && Found_Flag == 0);

      --IDm;

      if (Found_Flag == 0)
        Read_Flag = 0;
    }

    if (Read_Flag)
    {
      Set_BC_Data = 1;

      BC_IDs[IDm] = Surf_ID_Flag_;
      BL_Initial_Normal_Spacing[IDm] = BL_Initial_Normal_Spacing_;
      BL_Thickness[IDm] = BL_Thickness_;
      Grid_BC_Flag[IDm] = Grid_BC_Flag_;
      Number_of_BLs[IDm] = Number_of_BLs_;
      Rec_Flag[IDm] = (BC_Rec_Flag == 0) ? 0: 7;

      if (Message_Flag >= 1)
      {
        sprintf (Text, "UG_IO    : Data Set For ID =%10i", Surf_ID_Flag_);
        ug_message (Text);
      }
    }
  }
  while (Text_ != NULL);

  if (Set_BC_Data)
  {
    BC_ID = 0;

    for (IDm = 1; IDm <= Number_of_IDs; ++ IDm)
    {
      if (BC_IDs[IDm] <= Max_ID)
      {
        if (BC_ID < IDm)
        {
          BC_IDs[BC_ID] = BC_IDs[IDm];
          BL_Initial_Normal_Spacing[BC_ID] = BL_Initial_Normal_Spacing[IDm];
          BL_Thickness[BC_ID] = BL_Thickness[IDm];
          Grid_BC_Flag[BC_ID] = Grid_BC_Flag[IDm];
          Number_of_BLs[BC_ID] = Number_of_BLs[IDm];
          Rec_Flag[BC_ID] = Rec_Flag[IDm];
        }

        ++BC_ID;
      }
    }

    Number_of_BC_IDs = BC_ID;

    Set_Flag = 1;

    if (Set_Flag == 1) Set_Flag = ug_set_int_param_vector ("BC_IDs", Number_of_BC_IDs, BC_IDs, UG_Param_Struct_Ptr);
    if (Set_Flag == 1) Set_Flag = ug_set_double_param_vector ("BL_DS", Number_of_BC_IDs, BL_Initial_Normal_Spacing, UG_Param_Struct_Ptr);
    if (Set_Flag == 1) Set_Flag = ug_set_double_param_vector ("BL_DEL", Number_of_BC_IDs, BL_Thickness, UG_Param_Struct_Ptr);
    if (Set_Flag == 1) Set_Flag = ug_set_int_param_vector ("Number_of_BLs", Number_of_BC_IDs, Number_of_BLs, UG_Param_Struct_Ptr);
    if (Set_Flag == 1) Set_Flag = ug_set_int_param_vector ("Grid_BC_Flag", Number_of_BC_IDs, Grid_BC_Flag, UG_Param_Struct_Ptr);
    if (Set_Flag == 1) Set_Flag = ug_set_int_param_vector ("Rec_Flag", Number_of_BC_IDs, Rec_Flag, UG_Param_Struct_Ptr);

    Error_Flag = 1-Set_Flag;
  }

  ug_free (BC_IDs);
  ug_free (BL_Initial_Normal_Spacing);
  ug_free (BL_Thickness);
  ug_free (Grid_BC_Flag);
  ug_free (Number_of_BLs);
  ug_free (Rec_Flag);
  ug_free (Surf_ID_Flag_Map);

  if (Error_Flag)
    return (1);

  if (Set_BC_Data == 0 && Message_Flag >= 1)
    ug_message ("UG_IO    : No Data Set");

  return (0);

}
