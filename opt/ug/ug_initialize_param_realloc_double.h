
  if (Index >= Max_Double_Params)
  {
    UG_Param_Struct_Ptr->Number_of_Double_Params = Index + 1;

    Error_Flag = ug_realloc_param (Error_Message, UG_Param_Struct_Ptr);

    if (Error_Flag > 0)
      return (Error_Flag);

    Max_Double_Params = UG_Param_Struct_Ptr->Max_Double_Params;

    Double_Param_Flag = UG_Param_Struct_Ptr->Double_Param_Flag;
    Double_Param_Label = UG_Param_Struct_Ptr->Double_Param_Label;
    Double_Param_Name = UG_Param_Struct_Ptr->Double_Param_Name;
    Double_Param_Alt_Name = UG_Param_Struct_Ptr->Double_Param_Alt_Name;
    Double_Param_Vector_Entries = UG_Param_Struct_Ptr->Double_Param_Vector_Entries;
    Def_Double_Param = UG_Param_Struct_Ptr->Def_Double_Param;
    Max_Double_Param = UG_Param_Struct_Ptr->Max_Double_Param;
    Min_Double_Param = UG_Param_Struct_Ptr->Min_Double_Param;
  }
