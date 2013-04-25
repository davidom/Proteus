
  if (Index >= Max_Char_Params)
  {
    UG_Param_Struct_Ptr->Number_of_Char_Params = Index + 1;

    Error_Flag = ug_realloc_param (Error_Message, UG_Param_Struct_Ptr);

    if (Error_Flag > 0)
      return (Error_Flag);

    Max_Char_Params = UG_Param_Struct_Ptr->Max_Char_Params;
    Char_Param_Flag = UG_Param_Struct_Ptr->Char_Param_Flag;
    Char_Param_Label = UG_Param_Struct_Ptr->Char_Param_Label;
    Char_Param_Name = UG_Param_Struct_Ptr->Char_Param_Name;
    Char_Param_Alt_Name = UG_Param_Struct_Ptr->Char_Param_Alt_Name;
    Def_Char_Param = UG_Param_Struct_Ptr->Def_Char_Param;
    Char_Param_Type = UG_Param_Struct_Ptr->Char_Param_Type;
  }
