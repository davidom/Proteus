#include "UG_LIB.h"

INT_ ug_malloc_param
 (char Error_Message[],
  INT_ Max_Param_Labels,
  UG_Param_Struct ** UG_Param_Struct_Ptr)

{

/*
 * Malloc UG parameter structure arrays.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_malloc_param.c,v 1.27 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  UG_Param_Struct *Struct_Ptr_Ptr;

  INT_ Index, Label_Index;

  INT_ Error_Flag = 0;

  INT_ Max_Char_Params = 100;
  INT_ Max_Double_Params = 100;
  INT_ Max_Int_Params = 100;
  INT_ Def_Max_Param_Labels = 100;

  Max_Param_Labels = MAX (Def_Max_Param_Labels, Max_Param_Labels);

  *UG_Param_Struct_Ptr = (UG_Param_Struct *)
                          ug_malloc (&Error_Flag, sizeof (UG_Param_Struct));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc UG parameter structure");
    return (453);
  }

  Struct_Ptr_Ptr = *UG_Param_Struct_Ptr;

  Struct_Ptr_Ptr->Max_Param_Labels = Max_Param_Labels;

  Struct_Ptr_Ptr->Max_Char_Params = Max_Char_Params;
  Struct_Ptr_Ptr->Number_of_Char_Params = 0;

  Struct_Ptr_Ptr->Char_Param_Flag = (CHAR_11 *)
                                    ug_malloc (&Error_Flag,
                                               Max_Char_Params
                                             * sizeof (CHAR_11));
  Struct_Ptr_Ptr->Char_Param_Label = (CHAR_41 *)
                                     ug_malloc (&Error_Flag,
                                                Max_Param_Labels
                                              * Max_Char_Params
                                              * sizeof (CHAR_41));
  Struct_Ptr_Ptr->Char_Param_Name = (CHAR_25 *)
                                    ug_malloc (&Error_Flag,
                                               Max_Char_Params
                                             * sizeof (CHAR_25));
  Struct_Ptr_Ptr->Char_Param_Alt_Name = (CHAR_UG_MAX *)
                                    ug_malloc (&Error_Flag,
                                               Max_Char_Params
                                             * sizeof (CHAR_UG_MAX));
  Struct_Ptr_Ptr->Char_Param = (CHAR_UG_MAX *)
                               ug_malloc (&Error_Flag,
                                          Max_Char_Params
                                        * sizeof (CHAR_UG_MAX));
  Struct_Ptr_Ptr->Def_Char_Param = (CHAR_UG_MAX *)
                                   ug_malloc (&Error_Flag,
                                              Max_Char_Params
                                            * sizeof (CHAR_UG_MAX));
  Struct_Ptr_Ptr->Char_Param_Type = (INT_1D *)
                                   ug_malloc (&Error_Flag,
                                              Max_Char_Params
                                            * sizeof (INT_1D));

  Struct_Ptr_Ptr->Max_Double_Params = Max_Double_Params;
  Struct_Ptr_Ptr->Number_of_Double_Params = 0;
  Struct_Ptr_Ptr->Number_of_Double_Param_Vector_Entries = 0;

  Struct_Ptr_Ptr->Double_Param_Flag = (CHAR_11 *)
                                      ug_malloc (&Error_Flag,
                                                 Max_Double_Params
                                               * sizeof (CHAR_11));
  Struct_Ptr_Ptr->Double_Param_Label = (CHAR_41 *)
                                       ug_malloc (&Error_Flag,
                                                  Max_Param_Labels
                                                * Max_Double_Params
                                                * sizeof (CHAR_41));
  Struct_Ptr_Ptr->Double_Param_Name = (CHAR_25 *)
                                      ug_malloc (&Error_Flag,
                                                 Max_Double_Params
                                               * sizeof (CHAR_25));
  Struct_Ptr_Ptr->Double_Param_Alt_Name = (CHAR_UG_MAX *)
                                    ug_malloc (&Error_Flag,
                                               Max_Double_Params
                                             * sizeof (CHAR_UG_MAX));
  Struct_Ptr_Ptr->Double_Param_Vector_Entries = (INT_1D *)
                                                ug_malloc (&Error_Flag,
                                                           Max_Double_Params
                                                         * sizeof (INT_1D));
  Struct_Ptr_Ptr->Double_Param_Vector_Loc = (INT_1D *)
                                            ug_malloc (&Error_Flag,
                                                       Max_Double_Params
                                                     * sizeof (INT_1D));
  Struct_Ptr_Ptr->Double_Param = (DOUBLE_1D *)
                                 ug_malloc (&Error_Flag,
                                            Max_Double_Params
                                          * sizeof (DOUBLE_1D));
  Struct_Ptr_Ptr->Double_Param_Vector = NULL;
  Struct_Ptr_Ptr->Def_Double_Param = (DOUBLE_1D *)
                                     ug_malloc (&Error_Flag,
                                                 Max_Double_Params
                                              * sizeof (DOUBLE_1D));
  Struct_Ptr_Ptr->Max_Double_Param = (DOUBLE_1D *)
                                     ug_malloc (&Error_Flag,
                                                 Max_Double_Params
                                              * sizeof (DOUBLE_1D));
  Struct_Ptr_Ptr->Min_Double_Param = (DOUBLE_1D *)  
                                     ug_malloc (&Error_Flag,
                                                 Max_Double_Params
                                              * sizeof (DOUBLE_1D));

  Struct_Ptr_Ptr->Max_Int_Params = Max_Int_Params;
  Struct_Ptr_Ptr->Number_of_Int_Params = 0;
  Struct_Ptr_Ptr->Number_of_Int_Param_Vector_Entries = 0;

  Struct_Ptr_Ptr->Int_Param_Flag = (CHAR_11 *)
                                   ug_malloc (&Error_Flag,
                                              Max_Int_Params
                                            * sizeof (CHAR_11));
  Struct_Ptr_Ptr->Int_Param_Label = (CHAR_41 *)
                                    ug_malloc (&Error_Flag,
                                               Max_Param_Labels
                                             * Max_Int_Params
                                             * sizeof (CHAR_41));
  Struct_Ptr_Ptr->Int_Param_Name = (CHAR_25 *)
                                   ug_malloc (&Error_Flag,
                                              Max_Int_Params
                                            * sizeof (CHAR_25));
  Struct_Ptr_Ptr->Int_Param_Alt_Name = (CHAR_UG_MAX *)
                                    ug_malloc (&Error_Flag,
                                               Max_Int_Params
                                             * sizeof (CHAR_UG_MAX));
  Struct_Ptr_Ptr->Int_Param_Vector_Entries = (INT_1D *)
                                             ug_malloc (&Error_Flag,
                                                        Max_Int_Params 
                                                      * sizeof (INT_1D));
  Struct_Ptr_Ptr->Int_Param_Vector_Loc = (INT_1D *)
                                         ug_malloc (&Error_Flag,
                                                    Max_Int_Params 
                                                  * sizeof (INT_1D));
  Struct_Ptr_Ptr->Int_Param = (INT_1D *)
                              ug_malloc (&Error_Flag,
                                         Max_Int_Params
                                       * sizeof (INT_1D));
  Struct_Ptr_Ptr->Int_Param_Vector = NULL;
  Struct_Ptr_Ptr->Def_Int_Param = (INT_1D *)
                                  ug_malloc (&Error_Flag,
                                             Max_Int_Params
                                           * sizeof (INT_1D));
  Struct_Ptr_Ptr->Max_Int_Param = (INT_1D *)
                                  ug_malloc (&Error_Flag,
                                             Max_Int_Params
                                           * sizeof (INT_1D));
  Struct_Ptr_Ptr->Min_Int_Param = (INT_1D *)
                                  ug_malloc (&Error_Flag,
                                             Max_Int_Params 
                                           * sizeof (INT_1D));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc UG parameter arrays");
    return (454);
  }

  for (Index = 0; Index < Max_Char_Params; ++Index)
  {
    strcpy (Struct_Ptr_Ptr->Char_Param_Name[Index], "");
    strcpy (Struct_Ptr_Ptr->Char_Param_Alt_Name[Index], "");
    strcpy (Struct_Ptr_Ptr->Char_Param_Flag[Index], "yes");

    for (Label_Index = 0; Label_Index < Max_Param_Labels; ++Label_Index)
    {
      strcpy (Struct_Ptr_Ptr->Char_Param_Label[Label_Index+Index*Max_Param_Labels], "");
    }

    Struct_Ptr_Ptr->Char_Param_Type[Index] = 1;

    strcpy (Struct_Ptr_Ptr->Char_Param[Index], "");
    strcpy (Struct_Ptr_Ptr->Def_Char_Param[Index], "");
  }

  for (Index = 0; Index < Max_Double_Params; ++Index)
  {
    strcpy (Struct_Ptr_Ptr->Double_Param_Name[Index], "");
    strcpy (Struct_Ptr_Ptr->Double_Param_Alt_Name[Index], "");
    strcpy (Struct_Ptr_Ptr->Double_Param_Flag[Index], "yes");

    for (Label_Index = 0; Label_Index < Max_Param_Labels; ++Label_Index)
    {
      strcpy (Struct_Ptr_Ptr->Double_Param_Label[Label_Index+Index*Max_Param_Labels], "");
    }

    Struct_Ptr_Ptr->Double_Param[Index] = 0.0;
    Struct_Ptr_Ptr->Def_Double_Param[Index] = 0.0;
    Struct_Ptr_Ptr->Max_Double_Param[Index] = 0.0;
    Struct_Ptr_Ptr->Min_Double_Param[Index] = 0.0;

    Struct_Ptr_Ptr->Double_Param_Vector_Entries[Index] = -1;
    Struct_Ptr_Ptr->Double_Param_Vector_Loc[Index] = 0;
  }

  for (Index = 0; Index < Max_Int_Params; ++Index)
  {
    strcpy (Struct_Ptr_Ptr->Int_Param_Name[Index], "");
    strcpy (Struct_Ptr_Ptr->Int_Param_Alt_Name[Index], "");
    strcpy (Struct_Ptr_Ptr->Int_Param_Flag[Index], "yes");

    for (Label_Index = 0; Label_Index < Max_Param_Labels; ++Label_Index)
    {
      strcpy (Struct_Ptr_Ptr->Int_Param_Label[Label_Index+Index*Max_Param_Labels], "");
    }

    Struct_Ptr_Ptr->Int_Param[Index] = 0;
    Struct_Ptr_Ptr->Def_Int_Param[Index] = 0;
    Struct_Ptr_Ptr->Max_Int_Param[Index] = 0;
    Struct_Ptr_Ptr->Min_Int_Param[Index] = 0;

    Struct_Ptr_Ptr->Int_Param_Vector_Entries[Index] = -1;
    Struct_Ptr_Ptr->Int_Param_Vector_Loc[Index] = 0;
  }

  return (0);

}
