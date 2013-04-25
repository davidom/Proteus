
  if (Index >= Max_Int_Params)
  {
    UG_Param_Struct_Ptr->Number_of_Int_Params = Index + 1;

    Error_Flag = ug_realloc_param (Error_Message, UG_Param_Struct_Ptr);

    if (Error_Flag > 0)
      return (Error_Flag);

    Max_Int_Params = UG_Param_Struct_Ptr->Max_Int_Params;
    Int_Param_Flag = UG_Param_Struct_Ptr->Int_Param_Flag;
    Int_Param_Label = UG_Param_Struct_Ptr->Int_Param_Label;
    Int_Param_Name = UG_Param_Struct_Ptr->Int_Param_Name;
    Int_Param_Alt_Name = UG_Param_Struct_Ptr->Int_Param_Alt_Name;
    Int_Param_Vector_Entries = UG_Param_Struct_Ptr->Int_Param_Vector_Entries;
    Def_Int_Param = UG_Param_Struct_Ptr->Def_Int_Param;
    Max_Int_Param = UG_Param_Struct_Ptr->Max_Int_Param;
    Min_Int_Param = UG_Param_Struct_Ptr->Min_Int_Param;
  }
