#ifndef __UG_PARAM_STRUCT_H__

#define __UG_PARAM_STRUCT_H__

typedef struct _UG_Param_Struct UG_Param_Struct;

struct _UG_Param_Struct
{
  INT_ Max_Param_Labels;

  INT_ Max_Char_Params;
  INT_ Number_of_Char_Params;
  CHAR_11 *Char_Param_Flag;
  CHAR_41 *Char_Param_Label;
  CHAR_25 *Char_Param_Name;
  CHAR_UG_MAX *Char_Param_Alt_Name;
  CHAR_UG_MAX *Char_Param;
  CHAR_UG_MAX *Def_Char_Param;
  INT_1D *Char_Param_Type;

  INT_ Max_Double_Params;
  INT_ Number_of_Double_Params;
  INT_ Number_of_Double_Param_Vector_Entries;
  CHAR_11 *Double_Param_Flag;
  CHAR_41 *Double_Param_Label;
  CHAR_25 *Double_Param_Name;
  CHAR_UG_MAX *Double_Param_Alt_Name;
  INT_1D *Double_Param_Vector_Entries;
  INT_1D *Double_Param_Vector_Loc;
  DOUBLE_1D *Double_Param;
  DOUBLE_1D *Double_Param_Vector;
  DOUBLE_1D *Def_Double_Param;
  DOUBLE_1D *Max_Double_Param;
  DOUBLE_1D *Min_Double_Param;

  INT_ Max_Int_Params;
  INT_ Number_of_Int_Params;
  INT_ Number_of_Int_Param_Vector_Entries;
  CHAR_11 *Int_Param_Flag;
  CHAR_41 *Int_Param_Label;
  CHAR_25 *Int_Param_Name;
  CHAR_UG_MAX *Int_Param_Alt_Name;
  INT_1D *Int_Param_Vector_Entries;
  INT_1D *Int_Param_Vector_Loc;
  INT_1D *Int_Param;
  INT_1D *Int_Param_Vector;
  INT_1D *Def_Int_Param;
  INT_1D *Max_Int_Param;
  INT_1D *Min_Int_Param;
};

#endif
