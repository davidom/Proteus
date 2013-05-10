#include "UG_LIB.h"

INT_ ug_set_double_param
 (char _Double_Param_Name[],
  double _Double_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set the value of a UG type double parameter.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_set_double_param.c,v 1.16 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *Param_Name_Suffix = NULL;

  INT_ Index;
  INT_ Message_Flag=0;

  double Current_Double_Param_Value;

  if (UG_Param_Struct_Ptr == NULL)
    return (-2);

  Index = ug_get_double_param_ (_Double_Param_Name, &Current_Double_Param_Value, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (-1);

  Param_Name_Suffix = strstr (_Double_Param_Name, "@");

  if (Param_Name_Suffix)
  {
    if (strcmp (Param_Name_Suffix, "@def") == 0)
      _Double_Param_Value = UG_Param_Struct_Ptr->Def_Double_Param[Index];
    else if (strcmp (Param_Name_Suffix, "@max") == 0)
      _Double_Param_Value = UG_Param_Struct_Ptr->Max_Double_Param[Index];
    else if (strcmp (Param_Name_Suffix, "@min") == 0)
      _Double_Param_Value = UG_Param_Struct_Ptr->Min_Double_Param[Index];
  }

  ug_set_double_param_ (Index, Message_Flag, _Double_Param_Value, UG_Param_Struct_Ptr);

  return (1);

}

void ug_set_double_param_
 (INT_ Index,
  INT_ Message_Flag,
  double _Double_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{
  CHAR_133 Text;

  if (UG_Param_Struct_Ptr == NULL)
    return;

  if (_Double_Param_Value != UG_Param_Struct_Ptr->Double_Param[Index])
  {
    if (UG_Param_Struct_Ptr->Max_Double_Param[Index] > UG_Param_Struct_Ptr->Min_Double_Param[Index])
      UG_Param_Struct_Ptr->Double_Param[Index] = MAX (MIN (_Double_Param_Value, UG_Param_Struct_Ptr->Max_Double_Param[Index]),
                                                                                UG_Param_Struct_Ptr->Min_Double_Param[Index]);
    else
      UG_Param_Struct_Ptr->Double_Param[Index] = _Double_Param_Value;

    if (UG_Param_Struct_Ptr->Max_Double_Param[Index] > UG_Param_Struct_Ptr->Min_Double_Param[Index])
      UG_Param_Struct_Ptr->Double_Param[Index] = MAX (MIN (UG_Param_Struct_Ptr->Double_Param[Index],
                                                           UG_Param_Struct_Ptr->Max_Double_Param[Index]),
                                                           UG_Param_Struct_Ptr->Min_Double_Param[Index]);

    if (Message_Flag)
    {
      sprintf (Text, "UG PARAM : %-24s    = %-10g", UG_Param_Struct_Ptr->Double_Param_Name[Index], UG_Param_Struct_Ptr->Double_Param[Index]);
      ug_message (Text);
    }
  }

  return;
}
