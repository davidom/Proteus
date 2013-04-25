#ifndef __UG_IO_PARAM_STRUCT_H__

#define __UG_IO_PARAM_STRUCT_H__

typedef struct _UG_IO_Param_Struct UG_IO_Param_Struct;

struct _UG_IO_Param_Struct
{
  INT_ Max_IO_Param_Labels;

  INT_ Max_File_Types;
  INT_ Number_of_File_Types;
  INT_1D *File_Data_Type;
  INT_1D *File_Type_Format_Flag;
  CHAR_41 *File_Type_Label;
  CHAR_21 *File_Type_Suffix;

  INT_ Max_File_Formats;
  INT_ Number_of_File_Formats;
  INT_1D *File_Format;
  CHAR_41 *File_Format_Label;
  CHAR_11 *File_Format_Suffix;

  INT_ Max_File_Modes;
  INT_ Number_of_File_Modes;
  INT_1D *File_Mode;
  CHAR_41 *File_Mode_Label;
  CHAR_11 *File_Mode_Suffix;

  INT_ Max_File_Compressions;
  INT_ Number_of_File_Compressions;
  CHAR_41 *File_Compression_Label;
  CHAR_11 *File_Compression_Suffix;

  INT_ Version_Flag;
};

#endif
