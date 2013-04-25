#include "UG_LIB.h"

void ug_write_param_info
(INT_ UG_Param_Output_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Write information on all UG parameters to standard output.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_write_param_info.c,v 1.30 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX *Char_Param_Alt_Name, *Def_Char_Param, *Double_Param_Alt_Name,
              *Int_Param_Alt_Name;
  CHAR_133 Text;
  CHAR_41 *Char_Param_Label, *Double_Param_Label, *Int_Param_Label, Text_String;
  CHAR_25 *Char_Param_Name, *Double_Param_Name, *Int_Param_Name;
  CHAR_11 *Char_Param_Flag, *Double_Param_Flag, *Int_Param_Flag, Yes_No_Label;

  INT_1D *Char_Param_Type, *Def_Int_Param, *Double_Param_Vector_Entries,
         *Max_Int_Param, *Min_Int_Param, *Int_Param_Vector_Entries;

  DOUBLE_1D *Def_Double_Param, *Max_Double_Param, *Min_Double_Param;

  INT_ Heading_Flag, Index, Label_Index, Max_Param_Labels,
       Max_Param_Labels_Output, Number_of_Char_Params, Number_of_Double_Params,
       Number_of_Int_Params, Yes_No_Flag, Yes_No_Index;

  if (UG_Param_Output_Flag == 0)
    return;

  Max_Param_Labels = UG_Param_Struct_Ptr->Max_Param_Labels;

  Max_Param_Labels_Output = (UG_Param_Output_Flag == 2) ? Max_Param_Labels : 1;

  Number_of_Char_Params = UG_Param_Struct_Ptr->Number_of_Char_Params;
  Char_Param_Flag = UG_Param_Struct_Ptr->Char_Param_Flag;
  Char_Param_Label = UG_Param_Struct_Ptr->Char_Param_Label;
  Char_Param_Name = UG_Param_Struct_Ptr->Char_Param_Name;
  Char_Param_Alt_Name = UG_Param_Struct_Ptr->Char_Param_Alt_Name;
  Def_Char_Param = UG_Param_Struct_Ptr->Def_Char_Param;
  Char_Param_Type = UG_Param_Struct_Ptr->Char_Param_Type;

  Number_of_Double_Params = UG_Param_Struct_Ptr->Number_of_Double_Params;
  Double_Param_Name = UG_Param_Struct_Ptr->Double_Param_Name;
  Double_Param_Alt_Name = UG_Param_Struct_Ptr->Double_Param_Alt_Name;
  Double_Param_Flag = UG_Param_Struct_Ptr->Double_Param_Flag;
  Double_Param_Label = UG_Param_Struct_Ptr->Double_Param_Label;
  Double_Param_Vector_Entries = UG_Param_Struct_Ptr->Double_Param_Vector_Entries;
  Def_Double_Param = UG_Param_Struct_Ptr->Def_Double_Param;
  Max_Double_Param = UG_Param_Struct_Ptr->Max_Double_Param;
  Min_Double_Param = UG_Param_Struct_Ptr->Min_Double_Param;

  Number_of_Int_Params = UG_Param_Struct_Ptr->Number_of_Int_Params;
  Int_Param_Name = UG_Param_Struct_Ptr->Int_Param_Name;
  Int_Param_Alt_Name = UG_Param_Struct_Ptr->Int_Param_Alt_Name;
  Int_Param_Flag = UG_Param_Struct_Ptr->Int_Param_Flag;
  Int_Param_Label = UG_Param_Struct_Ptr->Int_Param_Label;
  Int_Param_Vector_Entries = UG_Param_Struct_Ptr->Int_Param_Vector_Entries;
  Def_Int_Param = UG_Param_Struct_Ptr->Def_Int_Param;
  Max_Int_Param = UG_Param_Struct_Ptr->Max_Int_Param;
  Min_Int_Param = UG_Param_Struct_Ptr->Min_Int_Param;

  Heading_Flag = 1;

  for (Index = 0; Index < Number_of_Char_Params; ++Index)
  {
    if (Char_Param_Type[Index] >= 2)
    {
      if (Heading_Flag)
      {
        ug_message (" ");
        ug_message ("Equivalent Parameters         Parameter Name and Description");

        ug_message ("---------------------         ------------------------------");

        Heading_Flag = 0;
      }

      if (strcmp (Def_Char_Param[Index], ""))
      {
        strcpy (Text, "");
        strncpy (Text, Def_Char_Param[Index], 40);
        ug_message (Text);
      }

      sprintf (Text, "                              %-24s", Char_Param_Name[Index]);
      ug_message (Text);

      if (strcmp (Char_Param_Alt_Name[Index], ""))
      {
        strcpy (Text_String, "");
        strncpy (Text_String, Char_Param_Alt_Name[Index], 30);

        sprintf (Text, "                              or any of %-30s", Text_String);
        ug_message (Text);
      }

      Label_Index = 0;

      while (Label_Index < Max_Param_Labels_Output && strcmp (Char_Param_Label[Label_Index+Index*Max_Param_Labels], "") != 0)
      {
        sprintf (Text, "                              %-40s", Char_Param_Label[Label_Index+Index*Max_Param_Labels]);
        ug_message (Text);

        ++Label_Index;
      }

      if (Max_Param_Labels_Output > 1)
      {
        if (Char_Param_Type[Index] == 2)
          ug_message ("                              No value should be specified.");

        else
          ug_message ("                              Last parameter value must be specified.");

        ug_message (" ");
      }
    }
  }

  Yes_No_Flag = 0;

  for (Yes_No_Index = 0; Yes_No_Index <= 1; ++Yes_No_Index)
  {
    if (Yes_No_Index == Yes_No_Flag)
    {
      if (Yes_No_Index == 0)
        strcpy (Yes_No_Label, "yes");
      else
      {
        strcpy (Yes_No_Label, "no");

        ug_message (" ");
        ug_message ("**********************************************************************");
        ug_message ("            THE FOLLOWING PARAMETERS SHOULD NOT BE CHANGED!");
        ug_message ("**********************************************************************");
      }

      ug_message (" ");
      ug_message ("Default   Minimum   Maximum   Parameter");
      ug_message ("Value     Value     Value     Name and Description");
      ug_message ("-------   -------   -------   --------------------");

      for (Index = 0; Index < Number_of_Int_Params; ++Index)
      {
        if (strcmp (Int_Param_Flag[Index], Yes_No_Label) == 0)
        {
          if (Int_Param_Vector_Entries[Index] == -1)
          {
            if (Max_Int_Param[Index] > Min_Int_Param[Index])
              sprintf (Text, "%-10i%-10i%-10i%-24s",
                       (int) Def_Int_Param[Index],
                       (int) Min_Int_Param[Index],
                       (int) Max_Int_Param[Index],
                       Int_Param_Name[Index]);
            else
              sprintf (Text, "%-10i*         *         %-24s",
                       (int) Def_Int_Param[Index],
                       Int_Param_Name[Index]);
          }
          else
            sprintf (Text, "*         *         *         %-24s",
                     Int_Param_Name[Index]);

          ug_message (Text);

          if (strcmp (Int_Param_Alt_Name[Index], ""))
          {
            strcpy (Text_String, "");
            strncpy (Text_String, Int_Param_Alt_Name[Index], 30);
            sprintf (Text, "                              or any of %-30s", Text_String);
            ug_message (Text);
          }

          Label_Index = 0;

          while (Label_Index < Max_Param_Labels_Output &&
                 strcmp (Int_Param_Label[Label_Index
                                        +Index*Max_Param_Labels], "") != 0)
          {
            sprintf (Text, "                              %-40s",
                     Int_Param_Label[Label_Index+Index*Max_Param_Labels]);
            ug_message (Text);

            ++Label_Index;
          }

          if (Max_Param_Labels_Output > 1)
            ug_message (" ");
        }
        else
          Yes_No_Flag = 1;
      }

      for (Index = 0; Index < Number_of_Double_Params; ++Index)
      {
        if (strcmp (Double_Param_Flag[Index], Yes_No_Label) == 0)
        {
          if (Double_Param_Vector_Entries[Index] == -1)
          {
            if (Max_Double_Param[Index] > Min_Double_Param[Index])
              sprintf (Text, "%-10g%-10g%-10g%-24s",
                       Def_Double_Param[Index],
                       Min_Double_Param[Index],
                       Max_Double_Param[Index],
                       Double_Param_Name[Index]);
            else
              sprintf (Text, "%-10g*         *         %-24s",
                       Def_Double_Param[Index],
                       Double_Param_Name[Index]);
          }
          else
            sprintf (Text, "*         *         *         %-24s",
                     Double_Param_Name[Index]);

          ug_message (Text);

          if (strcmp (Double_Param_Alt_Name[Index], ""))
          {
            strcpy (Text_String, "");
            strncpy (Text_String, Double_Param_Alt_Name[Index], 30);

            sprintf (Text, "                              or any of %-30s", Text_String);
            ug_message (Text);
          }

          Label_Index = 0;

          while (Label_Index < Max_Param_Labels_Output &&
                 strcmp (Double_Param_Label[Label_Index
                                        +Index*Max_Param_Labels], "") != 0)
          {
            sprintf (Text, "                              %-40s",
                     Double_Param_Label[Label_Index+Index*Max_Param_Labels]);
            ug_message (Text);

            ++Label_Index;
          }

          if (Max_Param_Labels_Output > 1)
            ug_message (" ");
        }
        else
          Yes_No_Flag = 1;
      }

      for (Index = 0; Index < Number_of_Char_Params; ++Index)
      {
        if (Char_Param_Type[Index] == 1 && strcmp (Char_Param_Flag[Index], Yes_No_Label) == 0)
        {
          strcpy (Text_String, "");
          strncpy (Text_String, Def_Char_Param[Index], 30);

          sprintf (Text, "%-30s%-24s", Text_String, Char_Param_Name[Index]);
          ug_message (Text);

          if (strcmp (Char_Param_Alt_Name[Index], ""))
          {
            strcpy (Text_String, "");
            strncpy (Text_String, Char_Param_Alt_Name[Index], 30);

            sprintf (Text, "                              or any of %-30s", Text_String);
            ug_message (Text);
          }

          Label_Index = 0;

          while (Label_Index < Max_Param_Labels_Output &&
                 strcmp (Char_Param_Label[Label_Index
                                         +Index*Max_Param_Labels], "") != 0)
          {
            sprintf (Text, "                              %-40s",
                     Char_Param_Label[Label_Index+Index*Max_Param_Labels]);
            ug_message (Text);

            ++Label_Index;
          }

          if (Max_Param_Labels_Output > 1)
            ug_message (" ");
        }
        else
          Yes_No_Flag = 1;
      }

      if (Max_Param_Labels_Output > 1)
        ug_message (" ");
    }
  }

  return;

}
