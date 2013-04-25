#include "UG_LIB.h"

INT_ ug_realloc_param
 (char Error_Message[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Realloc UG parameter structure arrays.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_realloc_param.c,v 1.20 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index, Label_Index,
       Max_Char_Params, Number_of_Char_Params,
       Max_Double_Params, Number_of_Double_Params,
       Max_Int_Params, Number_of_Int_Params,
       Max_Param_Labels;

  INT_ Error_Flag = 0;

  double c;
  double crealloc = 1.25;

  Max_Param_Labels = UG_Param_Struct_Ptr->Max_Param_Labels;

  Max_Char_Params = UG_Param_Struct_Ptr->Max_Char_Params;

  Number_of_Char_Params = UG_Param_Struct_Ptr->Number_of_Char_Params;

  if (Number_of_Char_Params > Max_Char_Params)
  {
    c = crealloc * ((double) Max_Char_Params);

    Max_Char_Params = NINT (c);

    UG_Param_Struct_Ptr->Max_Char_Params = Max_Char_Params;

    UG_Param_Struct_Ptr->Char_Param_Flag = (CHAR_11 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Char_Param_Flag,
                                        Max_Char_Params * sizeof (CHAR_11));
    UG_Param_Struct_Ptr->Char_Param_Label = (CHAR_41 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Char_Param_Label,
                                        UG_Param_Struct_Ptr->Max_Param_Labels
                                      * Max_Char_Params * sizeof (CHAR_41));
    UG_Param_Struct_Ptr->Char_Param_Name = (CHAR_25 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Char_Param_Name,
                                        Max_Char_Params * sizeof (CHAR_25));
    UG_Param_Struct_Ptr->Char_Param_Alt_Name = (CHAR_UG_MAX *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Char_Param_Alt_Name,
                                        Max_Char_Params * sizeof (CHAR_UG_MAX));
    UG_Param_Struct_Ptr->Char_Param = (CHAR_UG_MAX *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Char_Param,
                                        Max_Char_Params * sizeof (CHAR_UG_MAX));
    UG_Param_Struct_Ptr->Def_Char_Param = (CHAR_UG_MAX *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Def_Char_Param,
                                        Max_Char_Params * sizeof (CHAR_UG_MAX));
    UG_Param_Struct_Ptr->Char_Param_Type = (INT_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Char_Param_Type,
                                        Max_Char_Params * sizeof (INT_1D));

    if (Error_Flag > 0)
    {
      strcpy (Error_Message, "unable to realloc UG parameter arrays");
      return (450);
    }

    for (Index = Number_of_Char_Params-1; Index < Max_Char_Params; ++Index)
    {
      strcpy (UG_Param_Struct_Ptr->Char_Param_Name[Index], "");
      strcpy (UG_Param_Struct_Ptr->Char_Param_Alt_Name[Index], "");
      strcpy (UG_Param_Struct_Ptr->Char_Param_Flag[Index], "yes");

      for (Label_Index = 0; Label_Index < Max_Param_Labels; ++Label_Index)
      {
        strcpy (UG_Param_Struct_Ptr->Char_Param_Label[Label_Index+Index*Max_Param_Labels], "");
      }

      UG_Param_Struct_Ptr->Char_Param_Type[Index] = 1;

      strcpy (UG_Param_Struct_Ptr->Char_Param[Index], "");
      strcpy (UG_Param_Struct_Ptr->Def_Char_Param[Index], "");
    }
  }

  Max_Double_Params = UG_Param_Struct_Ptr->Max_Double_Params;

  Number_of_Double_Params = UG_Param_Struct_Ptr->Number_of_Double_Params;

  if (Number_of_Double_Params > Max_Double_Params)
  {
    c = crealloc * ((double) Max_Double_Params);

    Max_Double_Params = NINT (c);

    UG_Param_Struct_Ptr->Max_Double_Params = Max_Double_Params;

    UG_Param_Struct_Ptr->Double_Param_Flag = (CHAR_11 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Double_Param_Flag,
                                        Max_Double_Params * sizeof (CHAR_11));
    UG_Param_Struct_Ptr->Double_Param_Label = (CHAR_41 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Double_Param_Label,
                                        UG_Param_Struct_Ptr->Max_Param_Labels
                                      * Max_Double_Params * sizeof (CHAR_41));
    UG_Param_Struct_Ptr->Double_Param_Name = (CHAR_25 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Double_Param_Name,
                                        Max_Double_Params * sizeof (CHAR_25));
    UG_Param_Struct_Ptr->Double_Param_Alt_Name = (CHAR_UG_MAX *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Double_Param_Alt_Name,
                                        Max_Double_Params * sizeof (CHAR_UG_MAX));
    UG_Param_Struct_Ptr->Double_Param_Vector_Entries = (INT_1D *)  
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Double_Param_Vector_Entries,
                                        Max_Double_Params * sizeof (INT_1D));
    UG_Param_Struct_Ptr->Double_Param_Vector_Loc = (INT_1D *)  
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Double_Param_Vector_Loc,
                                        Max_Double_Params * sizeof (INT_1D));
    UG_Param_Struct_Ptr->Double_Param = (DOUBLE_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Double_Param,
                                        Max_Double_Params * sizeof (DOUBLE_1D));
    UG_Param_Struct_Ptr->Def_Double_Param = (DOUBLE_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Def_Double_Param,
                                        Max_Double_Params * sizeof (DOUBLE_1D));
    UG_Param_Struct_Ptr->Max_Double_Param = (DOUBLE_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Max_Double_Param,
                                        Max_Double_Params * sizeof (DOUBLE_1D));
    UG_Param_Struct_Ptr->Min_Double_Param = (DOUBLE_1D *)  
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Min_Double_Param,
                                        Max_Double_Params * sizeof (DOUBLE_1D));

    if (Error_Flag > 0)
    {
      strcpy (Error_Message, "unable to realloc UG parameter arrays");
      return (451);
    }

    for (Index = Number_of_Double_Params-1; Index < Max_Double_Params; ++Index)
    {
      strcpy (UG_Param_Struct_Ptr->Double_Param_Name[Index], "");
      strcpy (UG_Param_Struct_Ptr->Double_Param_Alt_Name[Index], "");
      strcpy (UG_Param_Struct_Ptr->Double_Param_Flag[Index], "yes");

      for (Label_Index = 0; Label_Index < Max_Param_Labels; ++Label_Index)
      {
        strcpy (UG_Param_Struct_Ptr->Double_Param_Label[Label_Index+Index*Max_Param_Labels], "");
      }

      UG_Param_Struct_Ptr->Double_Param[Index] = 0.0;
      UG_Param_Struct_Ptr->Def_Double_Param[Index] = 0.0;
      UG_Param_Struct_Ptr->Max_Double_Param[Index] = 0.0;
      UG_Param_Struct_Ptr->Min_Double_Param[Index] = 0.0;

      UG_Param_Struct_Ptr->Double_Param_Vector_Entries[Index] = -1;
      UG_Param_Struct_Ptr->Double_Param_Vector_Loc[Index] = 0;
    }
  }

  Max_Int_Params = UG_Param_Struct_Ptr->Max_Int_Params;

  Number_of_Int_Params = UG_Param_Struct_Ptr->Number_of_Int_Params;

  if (Number_of_Int_Params > Max_Int_Params)
  {
    c = crealloc * ((double) Max_Int_Params);

    Max_Int_Params = NINT (c);

    UG_Param_Struct_Ptr->Max_Int_Params = Max_Int_Params;

    UG_Param_Struct_Ptr->Int_Param_Flag = (CHAR_11 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Int_Param_Flag,
                                        Max_Int_Params * sizeof (CHAR_11));
    UG_Param_Struct_Ptr->Int_Param_Label = (CHAR_41 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Int_Param_Label,
                                        UG_Param_Struct_Ptr->Max_Param_Labels
                                      * Max_Int_Params * sizeof (CHAR_41));
    UG_Param_Struct_Ptr->Int_Param_Name = (CHAR_25 *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Int_Param_Name,
                                        Max_Int_Params * sizeof (CHAR_25));
    UG_Param_Struct_Ptr->Int_Param_Alt_Name = (CHAR_UG_MAX *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Int_Param_Alt_Name,
                                        Max_Int_Params * sizeof (CHAR_UG_MAX));
    UG_Param_Struct_Ptr->Int_Param_Vector_Entries = (INT_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Int_Param_Vector_Entries,
                                        Max_Int_Params * sizeof (INT_1D));
    UG_Param_Struct_Ptr->Int_Param_Vector_Loc = (INT_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Int_Param_Vector_Loc,
                                        Max_Int_Params * sizeof (INT_1D));
    UG_Param_Struct_Ptr->Int_Param = (INT_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Int_Param,
                                        Max_Int_Params * sizeof (INT_1D));
    UG_Param_Struct_Ptr->Def_Int_Param = (INT_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Def_Int_Param,
                                        Max_Int_Params * sizeof (INT_1D));
    UG_Param_Struct_Ptr->Max_Int_Param = (INT_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Max_Int_Param,
                                        Max_Int_Params * sizeof (INT_1D));
    UG_Param_Struct_Ptr->Min_Int_Param = (INT_1D *)
                                        ug_realloc (&Error_Flag,
                                        UG_Param_Struct_Ptr->Min_Int_Param,
                                        Max_Int_Params * sizeof (INT_1D));

    if (Error_Flag > 0)
    {
      strcpy (Error_Message, "unable to realloc UG parameter arrays");
      return (452);
    }

    for (Index = Number_of_Int_Params-1; Index < Max_Int_Params; ++Index)
    {
      strcpy (UG_Param_Struct_Ptr->Int_Param_Name[Index], "");
      strcpy (UG_Param_Struct_Ptr->Int_Param_Alt_Name[Index], "");
      strcpy (UG_Param_Struct_Ptr->Int_Param_Flag[Index], "yes");

      for (Label_Index = 0; Label_Index < Max_Param_Labels; ++Label_Index)
      {
        strcpy (UG_Param_Struct_Ptr->Int_Param_Label[Label_Index+Index*Max_Param_Labels], "");
      }

      UG_Param_Struct_Ptr->Int_Param[Index] = 0;
      UG_Param_Struct_Ptr->Def_Int_Param[Index] = 0;
      UG_Param_Struct_Ptr->Max_Int_Param[Index] = 0;
      UG_Param_Struct_Ptr->Min_Int_Param[Index] = 0;

      UG_Param_Struct_Ptr->Int_Param_Vector_Entries[Index] = -1;
      UG_Param_Struct_Ptr->Int_Param_Vector_Loc[Index] = 0;
    }
  }

  return (0);

}
