#include "UG_LIB.h"

void dummy_ext_signal_cleanup_routine (void);
void dummy_ext_ug_message_routine (FILE *UG_Output_File, INT_ UG_Standard_Output_Flag, char *text);
void dummy_ext_ug_error_message_routine (FILE *UG_Output_File, char *text);
INT_ ug_check_renumber (void);
INT_ ug_check_sort (void);
INT_ ug_initialize_test_param (char [], UG_Param_Struct *);

int main (int argc, char *argv[])
{

/*
 * -----------------------------------------------------------------------------
 * TEST PROGRAM : UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug.c,v 1.93 2013/03/16 19:11:06 marcum Exp $
 * -----------------------------------------------------------------------------
 */

  UG_Param_Struct *UG_Param_Struct_Ptr;

  DOUBLE_1D *Double_Array1, *Double_Array2, *Double_Param_Vector;

  INT_1D *Int_Array1, *Int_Array2, *Int_Param_Vector, *Test_Array;

  FILE *File1, *File2, *Param_File;

  CHAR_UG_MAX Char_Param, TMP_File_Dir,
              machine_type, os_release, os_type,
              arch_mode32_suffix, arch_mode64_suffix, base_arch_type,
              base_sys_arch_type, def_arch_mode_suffix, def_arch_type,
              drive, dname, fname, ext;
  CHAR_133 Case_Name, cat_Command, Date, Error_Message, 
           File_Name1, File_Name2, New_File_Name, 
           Suffix_Name, System_Command, Text, Time,
           UG_Param_File_Name, Version_Date, Version_Number;
  CHAR_11 File_Compression_Suffix;

  INT_ arg, Background_Flag, Byte_Order, Double_Param_Vector_Entries,
       Error_Flag, Get_Flag, Index, Int_Param, Int_Param_Vector_Entries,
       Known_Error, Max_Int_Value, Min_Int_Value, New_Number_of_Array_Elements,
       Number_of_Array_Elements, PID, Set_Flag, Start_Index, End_Index, Value;

  double Double_Param, double_Value, Max_double_Value, Min_double_Value,
         Round_Off_Error, v1, v2, v3;

  INT_ File_Status_Monitor = 0;
  INT_ Memory_Monitor = 0;
  INT_ Message_Flag = 1;

  Known_Error = 0;

  ug_message (" ");
  ug_message ("Set signal handler.");
  ug_message ("Register a dummy external cleanup routine.");

    ug_signal ();
    ug_register_signal_cleanup (dummy_ext_signal_cleanup_routine);

  ug_message (" ");
  ug_message ("Run executable with the following options to test argument vector handling.");
  ug_message ("ug.exe -Int_Param_3 32 -Double_Param_1=3.72 -Int_Param_Vector_2 7,12,2,9,8,43,377,-21 Double_Param_3=0.002 -Double_Param_2 97.8 --version -Int_Param_Vector_2 5 111 222 333 444 555");

  if (File_Status_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn off file status monitor");

      ug_file_status_monitor ("on");
  }

  if (Memory_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn on memory monitor");

      ug_memory_monitor ("on");
  }

  ug_message (" ");
  ug_message ("Get current date and time.");

    ug_date_time (Date, Time);

  ug_message (Date);
  ug_message (Time);

  ug_message (" ");
  ug_message ("Get process ID");

    PID = ug_getpid ();

  sprintf (Text, "Process ID is %i", (int) PID);
  ug_message (Text);

  ug_message (" ");
  ug_message ("List program version information.");

strncpy (Version_Date, "08/15/12 @ 01:07PM", 40);
strncpy (Version_Number, "3.32.15", 40);

  strcpy (&(Version_Date[40]), "\0");
  strcpy (&(Version_Number[40]), "\0");

  ug_message (" ");
  sprintf (Text, "UG Test Prog : Version Number %-50s", Version_Number);
  ug_message (Text);
  sprintf (Text, "UG Test Prog : Version Date   %-50s", Version_Date);
  ug_message (Text);

  ug_message (" ");
  ug_message ("List library compile date and OS information.");

    ug_version_info ("ug", ug_version);

  ug_message (" ");
  ug_message ("Check for argument options requesting rerun with another version of the exectuable.");

    Error_Flag = ug_rerun (Message_Flag, argc, argv);

    if (Error_Flag != 0)
    {
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Get OS information.");

    ug_get_sys_info (machine_type, os_release, os_type);

  sprintf (Text, "machine type is %s", machine_type);
  ug_message (Text);
  sprintf (Text, "system OS type is %s", os_type);
  ug_message (Text);
  sprintf (Text, "system OS release is %s", os_release);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get architecture naming information.");

    Error_Flag = ug_get_arch_info (arch_mode32_suffix, arch_mode64_suffix,
                                   base_arch_type, base_sys_arch_type,
                                   def_arch_mode_suffix, def_arch_type);

    if (Error_Flag != 0)
    {
      ug_error_message ("*** unknown architecture type ***");
      exit (Error_Flag);
    }

  sprintf (Text, "32-bit architecture suffix is %s", arch_mode32_suffix);
  ug_message (Text);
  sprintf (Text, "64-bit architecture suffix is %s", arch_mode64_suffix);
  ug_message (Text);
  sprintf (Text, "base architecture type is %s", base_arch_type);
  ug_message (Text);
  sprintf (Text, "base system architecture type is %s", base_sys_arch_type);
  ug_message (Text);
  sprintf (Text, "default architecture suffix is %s", def_arch_mode_suffix);
  ug_message (Text);
  sprintf (Text, "default architecture type is %s", def_arch_type);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get host endian.");

    Byte_Order = ug_get_byte_order ();

  if (Byte_Order == 1)
    ug_message ("Host byte ordering is big endian");
  else
    ug_message ("Host byte ordering is little endian");

  strcpy (cat_Command, UG_SHELL_COMMAND_CAT);
  strcat (cat_Command, " ");

  ug_message (" ");
  sprintf (Text, "Using %i-byte integers", (int) sizeof (INT_));
  ug_message (Text);

  ug_message (" ");
  ug_message ("Initialize CPU time routine.");

    ug_cpu_message ("");

  strcpy (Case_Name, "ug");
  ug_message (" ");
  sprintf (Text, "Set Case Name to %s", Case_Name);
  ug_message (Text);

    ug_case_name (Case_Name);

  ug_message (" ");
  ug_message ("Get Case Name");
  strcpy (Case_Name, "");

    ug_case_name (Case_Name);

  ug_message (" ");
  sprintf (Text, "Case Name = %s", Case_Name);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Malloc UG parameter structure arrays.");

    Error_Flag = ug_malloc_param (Error_Message, 0, &UG_Param_Struct_Ptr);

    if (Error_Flag > 0)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (Error_Flag);
    }
 
  ug_message (" ");
  ug_message ("Set default and initial values for UG parameters.");

    Error_Flag = ug_initialize_test_param (Error_Message, UG_Param_Struct_Ptr);

    if (Error_Flag > 0)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Write summary UG parameter information.");

    ug_write_param_info (1, UG_Param_Struct_Ptr);

  ug_message (" ");
  ug_message ("Write all UG parameter information.");

    ug_write_param_info (2, UG_Param_Struct_Ptr);

  ug_message (" ");
  ug_message ("Check parameters");

    ug_check_param (UG_Param_Struct_Ptr);

  ug_message (" ");
  ug_message ("Get value of Int_Param_1.");

    Get_Flag = ug_get_int_param ("Int_Param_1", &Int_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_1 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Int_Param_2.");

    Get_Flag = ug_get_int_param ("Int_Param_2", &Int_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get default value of Int_Param_2.");

    Get_Flag = ug_get_int_param ("Int_Param_2@def", &Int_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get defaut value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 (def) = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get minimum value of Int_Param_2.");

    Get_Flag = ug_get_int_param ("Int_Param_2@min", &Int_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get minimum value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 (min) = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get maximum value of Int_Param_2.");

    Get_Flag = ug_get_int_param ("Int_Param_2@max", &Int_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get maximum value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 (max) = %i", (int) Int_Param);
  ug_message (Text);
  ug_message (" ");
  ug_message ("Get value of Int_Param_3.");

    Get_Flag = ug_get_int_param ("Int_Param_3", &Int_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_3 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Int_Param_4.");

    Get_Flag = ug_get_int_param ("Int_Param_4", &Int_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_4 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_1.");

    Get_Flag = ug_get_double_param ("Double_Param_1", &Double_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_1 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_2.");

    Get_Flag = ug_get_double_param ("Double_Param_2", &Double_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_2 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_3.");

    Get_Flag = ug_get_double_param ("Double_Param_3", &Double_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_3 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_4.");

    Get_Flag = ug_get_double_param ("Double_Param_4", &Double_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_4 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of character string Char_Param_1");

    Get_Flag = ug_get_char_param ("Char_Param_1", Char_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Char_Param_1    = %s", Char_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of int parameter vector Int_Param_Vector_1");

    Get_Flag = ug_get_int_param_vector ("Int_Param_Vector_1",
                                        &Int_Param_Vector_Entries,
                                        &Int_Param_Vector,
                                        UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
    ug_message (Text);
  }

  ug_message (" ");
  ug_message ("Get value of int parameter vector Int_Param_Vector_2");

    Get_Flag = ug_get_int_param_vector ("Int_Param_Vector_2",
                                        &Int_Param_Vector_Entries,
                                        &Int_Param_Vector,
                                        UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
    ug_message (Text);
  }

  ug_message (" ");
  ug_message ("Get value of double parameter vector Double_Param_Vector_1");

    Get_Flag = ug_get_double_param_vector ("Double_Param_Vector_1",
                                           &Double_Param_Vector_Entries,
                                           &Double_Param_Vector,
                                           UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get value of parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Double_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Double Vector Value [%i] = %g", (int) Index, Double_Param_Vector[Index]);
    ug_message (Text);
  }

  ug_message (" ");
  ug_message ("Set value of Int_Param_2 to 17.");

    Set_Flag = ug_set_int_param ("Int_Param_2", 17, UG_Param_Struct_Ptr);

    if (Set_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to set value of parameter.");
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of Int_Param_2.");
 
    Get_Flag = ug_get_int_param ("Int_Param_2", &Int_Param,  UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Set value of Double_Param_2 to 1.7.");

    Set_Flag = ug_set_double_param ("Double_Param_2", 1.7, UG_Param_Struct_Ptr);

    if (Set_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to set value of parameter.");
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of Double_Param_2.");
 
    Get_Flag = ug_get_double_param ("Double_Param_2", &Double_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_2 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Set value of Char_Param_1 to _other_string_.");

    Set_Flag = ug_set_char_param ("Char_Param_1", "_other_string_", UG_Param_Struct_Ptr);

    if (Set_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to set value of parameter.");
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of character string Char_Param_1");

    Get_Flag = ug_get_char_param ("Char_Param_1", Char_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Char_Param_1    = %s", Char_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Set value of Int_Param_Vector_1 to 0,1,2,3,4");

    Int_Param_Vector_Entries = 5;

    Error_Flag = 0;

    Int_Param_Vector= (INT_1D *) ug_malloc (&Error_Flag,
                                            Int_Param_Vector_Entries
                                          * sizeof (INT_1D));
 
    if (Error_Flag > 0)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_free (Int_Param_Vector);
      ug_error_message ("Unable to malloc test array.");
      exit (Error_Flag);
    }

    for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
    {
      Int_Param_Vector[Index] = Index;
    }

    Set_Flag = ug_set_int_param_vector ("Int_Param_Vector_1",
                                        Int_Param_Vector_Entries,
                                        Int_Param_Vector,
                                        UG_Param_Struct_Ptr);

    ug_free (Int_Param_Vector);

    Int_Param_Vector = NULL;

    if (Set_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to set value of parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of int parameter vector Int_Param_Vector_1");

    Get_Flag = ug_get_int_param_vector ("Int_Param_Vector_1",
                                        &Int_Param_Vector_Entries,
                                        &Int_Param_Vector,
                                        UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
    ug_message (Text);
  }

  ug_message (" ");
  ug_message ("Set value of Double_Param_Vector_1 to 0.03,1.03,2.03,3.03,4.03,5.03,6.03,7.03");

    Double_Param_Vector_Entries = 8;

    Error_Flag = 0;

    Double_Param_Vector= (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                                  Double_Param_Vector_Entries
                                                * sizeof (DOUBLE_1D));
 
    if (Error_Flag > 0)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_free (Double_Param_Vector);
      ug_error_message ("Unable to malloc test array.");
      exit (Error_Flag);
    }

    for (Index = 0; Index < Double_Param_Vector_Entries; ++Index)
    {
      Double_Param_Vector[Index] = 0.03 + (double) Index;
    }

    Set_Flag = ug_set_double_param_vector ("Double_Param_Vector_1",
                                           Double_Param_Vector_Entries,
                                           Double_Param_Vector,
                                           UG_Param_Struct_Ptr);

    ug_free (Double_Param_Vector);

    Double_Param_Vector = NULL;

    if (Set_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to set value of parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of double parameter vector Double_Param_Vector_1");

    Get_Flag = ug_get_double_param_vector ("Double_Param_Vector_1",
                                           &Double_Param_Vector_Entries,
                                           &Double_Param_Vector,
                                           UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get value of parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Double_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Double Vector Value [%i] = %g", (int) Index,
             Double_Param_Vector[Index]);
    ug_message (Text);
  }

  strcpy (UG_Param_File_Name, "ug.par");

  ug_message (" ");
  sprintf (Text, "Open parameter file %s", UG_Param_File_Name);
  ug_message (Text);

    Param_File = ug_fopen (UG_Param_File_Name, "w");

  ug_message (" ");
  sprintf (Text, "Create parameter file %s with the entries.",
           UG_Param_File_Name);
  ug_message (Text);

    fprintf (Param_File, "#Sample Parameter File\n");
    fprintf (Param_File, "	Double_Param_2 21.8\n");
    fprintf (Param_File, "#this is an int param vector using single line format\n");
    fprintf (Param_File, "#it also has an embedded range string (int param vector single line format only)\n");
    fprintf (Param_File, "	Int_Param_Vector_1 72,3,5-12,17\n");
    fprintf (Param_File, "#this is a double param vector using multi-line format\n");
    fprintf (Param_File, "	Double_Param_Vector_1 2\n");
    fprintf (Param_File, "	-2.97783\n");
    fprintf (Param_File, "	0.007894\n");
    fprintf (Param_File, "#the dash prefix will be ignored\n");
    fprintf (Param_File, "	-Char_Param_1 _new_string_\n");
    fprintf (Param_File, "	Int_Param_2 8\n");
    fprintf (Param_File, "	Int_Param_2 999# <- note that the # sign anywhere turns the line into a comment\n");
    fprintf (Param_File, "#this is an int param vector using multi-line format\n");
    fprintf (Param_File, "#the dash prefix will be ignored\n");
    fprintf (Param_File, "	-Int_Param_Vector_2 4\n");
    fprintf (Param_File, "	32\n");
    fprintf (Param_File, "	4\n");
    fprintf (Param_File, "	12\n");
    fprintf (Param_File, "	90028\n");
    ug_fclose (Param_File);

  strcpy (System_Command, cat_Command);
  strcat (System_Command, UG_Param_File_Name);

    Error_Flag = ug_system (System_Command);
 
    if (Error_Flag != 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("Unable to execute system command");
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Get value of Int_Param_1 from the structure.");
 
    Get_Flag = ug_get_read_int_param ("Int_Param_1", UG_Param_File_Name,
                                      &Int_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_1 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Int_Param_2 from the file.");
 
    Get_Flag = ug_get_read_int_param ("Int_Param_2", UG_Param_File_Name,
                                      &Int_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_1 from the structure.");
 
    Get_Flag = ug_get_read_double_param ("Double_Param_1", UG_Param_File_Name,
                                         &Double_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_1 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_2 from the file.");
 
    Get_Flag = ug_get_read_double_param ("Double_Param_2", UG_Param_File_Name,
                                         &Double_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_2 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of character string Char_Param_1 from the file.");

    Get_Flag = ug_get_read_char_param ("Char_Param_1", UG_Param_File_Name,
                                       Char_Param, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Char_Param_1    = %s", Char_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of int parameter vector Int_Param_Vector_1 from the file.");

    Int_Param_Vector = NULL;

    Get_Flag = ug_get_read_int_param_vector ("Int_Param_Vector_1",
                                             UG_Param_File_Name,
                                             &Int_Param_Vector_Entries,
                                             &Int_Param_Vector,
                                             UG_Param_Struct_Ptr);

    if (Get_Flag >= 1)
    {
      for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
      {
        sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
        ug_message (Text);
      }

      if (Get_Flag == 2)
        ug_free (Int_Param_Vector);
    }

    else
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of int parameter vector Int_Param_Vector_2 from the file.");

    Int_Param_Vector = NULL;

    Get_Flag = ug_get_read_int_param_vector ("Int_Param_Vector_2",
                                             UG_Param_File_Name,
                                             &Int_Param_Vector_Entries,
                                             &Int_Param_Vector,
                                             UG_Param_Struct_Ptr);

    if (Get_Flag >= 1)
    {
      for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
      {
        sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
        ug_message (Text);
      }

      if (Get_Flag == 2)
        ug_free (Int_Param_Vector);
    }

    else
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of double parameter vector Double_Param_Vector_1 from the file");

    Double_Param_Vector = NULL;

    Get_Flag = ug_get_read_double_param_vector ("Double_Param_Vector_1",
                                                UG_Param_File_Name,
                                                &Double_Param_Vector_Entries,
                                                &Double_Param_Vector,
                                                UG_Param_Struct_Ptr);

    if (Get_Flag >= 1)
    {
      for (Index = 0; Index < Double_Param_Vector_Entries; ++Index)
      {
        sprintf (Text, "Double Vector Value [%i] = %g", (int) Index, Double_Param_Vector[Index]);
        ug_message (Text);
      }

      if (Get_Flag == 2)
        ug_free (Double_Param_Vector);
    }

    else
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get value of parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  ug_message ("Output argument vector.");
  ug_message (" ");

    for (arg = 0; arg < argc; ++arg)
    {
      sprintf (Text, "argv[%i]=%s", (int) arg, argv[arg]);
      ug_message(Text);
    }

  ug_message (" ");
  ug_message ("Get value of Int_Param_1 from the argument vector, file or structure.");
 
    Get_Flag = ug_get_arg_int_param (argv, "Int_Param_1", UG_Param_File_Name,
                                     argc, &Int_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_1 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Int_Param_2 from the argument vector, file or structure.");
 
    Get_Flag = ug_get_arg_int_param (argv, "Int_Param_2", UG_Param_File_Name,
                                     argc, &Int_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Int_Param_3 from the argument vector, file or structure.");
 
    Get_Flag = ug_get_arg_int_param (argv, "Int_Param_3", UG_Param_File_Name,
                                     argc, &Int_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_3 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_1 from the argument vector, file or structure.");
 
    Get_Flag = ug_get_arg_double_param (argv, "Double_Param_1", UG_Param_File_Name,
                                        argc, &Double_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_1 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_2 from the argument vector, file or structure.");
 
    Get_Flag = ug_get_arg_double_param (argv, "Double_Param_2", UG_Param_File_Name,
                                        argc, &Double_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_2 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_3 from the argument vector, file or structure.");
 
    Get_Flag = ug_get_arg_double_param (argv, "Double_Param_3", UG_Param_File_Name,
                                        argc, &Double_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_3 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Char_Param_1 from the argument vector, file or structure.");

    Get_Flag = ug_get_arg_char_param (argv, "Char_Param_1", Char_Param,
                                      UG_Param_File_Name, argc, UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Char_Param_1    = %s", Char_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Check if argument --version is in the argument vector or file.");

    Get_Flag = ug_get_arg_char_param (argv, "-version", Char_Param, 
                                      UG_Param_File_Name, argc, UG_Param_Struct_Ptr);

    if (Get_Flag == 1)
      ug_message ("-version        = found");
    else
      ug_message ("-version        = not found");

  ug_message (" ");
  ug_message ("Get value of Int_Param_Vector_1 from the argument vector, file or structure.");

    Int_Param_Vector = NULL;

    Get_Flag = ug_get_arg_int_param_vector (argv, "Int_Param_Vector_1",
                                            UG_Param_File_Name, argc, 
                                            &Int_Param_Vector_Entries,
                                            &Int_Param_Vector,
                                            UG_Param_Struct_Ptr);

    if (Get_Flag >= 1)
    {
      for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
      {
        sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
        ug_message (Text);
      }

      if (Get_Flag == 2)
        ug_free (Int_Param_Vector);
    }

    else
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of Int_Param_Vector_2 from the argument vector, file or structure.");

    Int_Param_Vector = NULL;

    Get_Flag = ug_get_arg_int_param_vector (argv, "Int_Param_Vector_2",
                                            UG_Param_File_Name, argc, 
                                            &Int_Param_Vector_Entries,
                                            &Int_Param_Vector,
                                            UG_Param_Struct_Ptr);

    if (Get_Flag >= 1)
    {
      for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
      {
        sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
        ug_message (Text);
      }

      if (Get_Flag == 2)
        ug_free (Int_Param_Vector);
    }

    else
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  ug_message ("Get value of Double_Param_Vector_2 from the argument vector, file or structure.");

    Double_Param_Vector = NULL;

    Get_Flag = ug_get_arg_double_param_vector (argv, "Double_Param_Vector_1",
                                               UG_Param_File_Name, argc,
                                               &Double_Param_Vector_Entries,
                                               &Double_Param_Vector,
                                               UG_Param_Struct_Ptr);

    if (Get_Flag >= 1)
    {
      for (Index = 0; Index < Double_Param_Vector_Entries; ++Index)
      {
        sprintf (Text, "Double Vector Value [%i] = %g", (int) Index, Double_Param_Vector[Index]);
        ug_message (Text);
      }

      if (Get_Flag == 2)
        ug_free (Double_Param_Vector);
    }

    else
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get value of parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  ug_message (" ");
  sprintf (Text, "Scan argument vector, read parameter file %s and re-set parameters.", UG_Param_File_Name);
  ug_message (Text);

    Error_Flag = ug_set_params_from_arg_or_file (argv, UG_Param_File_Name, argc, Message_Flag, UG_Param_Struct_Ptr);

    if (Error_Flag > 0 && Error_Flag != 412)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (Error_Flag);
    }

  strcpy (Case_Name, "save");
  strcpy (Suffix_Name, "ug");

  strcpy (New_File_Name, Case_Name);
  strcat (New_File_Name, ".");
  strcat (New_File_Name, Suffix_Name);
  strcat (New_File_Name, ".par");

  ug_message (" ");
  sprintf (Text, "Save parameter file in %s.", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_save_param_file (Case_Name, Suffix_Name,
                                     UG_Param_File_Name, Error_Message);

    if (Error_Flag > 0)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "File %s should be identical to %s", New_File_Name, UG_Param_File_Name);
  ug_message (Text);

  strcpy (System_Command, cat_Command);
  strcat (System_Command, New_File_Name);

    Error_Flag = ug_system (System_Command);
 
    if (Error_Flag != 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("Unable to execute system command");
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "Remove file %s", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_remove_file (New_File_Name);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("unable to remove file");
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Get value of Int_Param_2.");
 
    Get_Flag = ug_get_int_param ("Int_Param_2", &Int_Param,  UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Int_Param_2 = %i", (int) Int_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of Double_Param_2.");
 
    Get_Flag = ug_get_double_param ("Double_Param_2", &Double_Param, UG_Param_Struct_Ptr);
 
    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Double_Param_2 = %g", Double_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of character string Char_Param_1.");

    Get_Flag = ug_get_char_param ("Char_Param_1", Char_Param,  UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      ug_error_message ("Unable to get value of parameter.");
      exit (1);
    }

  sprintf (Text, "Char_Param_1    = %s", Char_Param);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get value of int parameter vector Int_Param_Vector_1");

    Get_Flag = ug_get_int_param_vector ("Int_Param_Vector_1",
                                        &Int_Param_Vector_Entries,
                                        &Int_Param_Vector,
                                        UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
    ug_message (Text);
  }

  ug_message (" ");
  ug_message ("Get value of int parameter vector Int_Param_Vector_2");

    Get_Flag = ug_get_int_param_vector ("Int_Param_Vector_2",
                                        &Int_Param_Vector_Entries,
                                        &Int_Param_Vector,
                                        UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Int_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Integer Vector Value [%i] = %i", (int) Index, (int) Int_Param_Vector[Index]);
    ug_message (Text);
  }

  ug_message (" ");
  ug_message ("Get value of double parameter vector Double_Param_Vector_1");

    Get_Flag = ug_get_double_param_vector ("Double_Param_Vector_1",
                                           &Double_Param_Vector_Entries,
                                           &Double_Param_Vector,
                                           UG_Param_Struct_Ptr);

    if (Get_Flag != 1)
    {
      ug_free_param (UG_Param_Struct_Ptr);
      if (Get_Flag <= 0)
        ug_error_message ("Unable to get value of parameter.");
      else
        ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (1);
    }

  for (Index = 0; Index < Double_Param_Vector_Entries; ++Index)
  {
    sprintf (Text, "Double Vector Value [%i] = %g", (int) Index, Double_Param_Vector[Index]);
    ug_message (Text);
  }

  if (! File_Status_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn on file status monitor");

      ug_file_status_monitor ("on");
  }

  ug_message (" ");
  ug_message ("Write UG parameter file.");
  ug_message ("Only those values which differ from default along with all");
  ug_message ("double and int parameter vectors that have been set are written");

    Error_Flag = ug_write_param_file (UG_Param_File_Name, Error_Message, UG_Param_Struct_Ptr);

    if (Error_Flag > 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "Parameter file %s written", UG_Param_File_Name);
  ug_message (Text);

  strcpy (System_Command, cat_Command);
  strcat (System_Command, UG_Param_File_Name);

    Error_Flag = ug_system (System_Command);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("Unable to execute system command");
      exit (Error_Flag);
    }

  strcpy (New_File_Name, UG_Param_File_Name);
  strcat (New_File_Name, ".bak");

  ug_message (" ");
  ug_message ("Create a backup file");

    Error_Flag = ug_backup_file (UG_Param_File_Name);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("unable to create backup file");
      exit (Error_Flag);
    }
 
  ug_message (" ");
  sprintf (Text, "File %s should be identical to %s", New_File_Name, UG_Param_File_Name);
  ug_message (Text);

  strcpy (System_Command, cat_Command);
  strcat (System_Command, New_File_Name);

    Error_Flag = ug_system (System_Command);
 
    if (Error_Flag != 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("Unable to execute system command");
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "Remove file %s", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_remove_file (New_File_Name);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("unable to remove file");
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "Compress file %s using bzip2", UG_Param_File_Name);
  ug_message (Text);

    Background_Flag = 0;

    strcpy (File_Compression_Suffix, ".bz2");

    Error_Flag = ug_compress_file (Background_Flag, File_Compression_Suffix, UG_Param_File_Name);

    if (Error_Flag < 0)
      ug_error_message ("unable to compress file");

  strcpy (New_File_Name, UG_Param_File_Name);
  strcat (New_File_Name, ".bz2");

  ug_message (" ");
  sprintf (Text, "Uncompress file %s using bzip2", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_uncompress_file (New_File_Name);

    if (Error_Flag < 0)
      ug_error_message ("unable to uncompress file");

  ug_message (" ");
  sprintf (Text, "Compress file %s using gzip", UG_Param_File_Name);
  ug_message (Text);

    Background_Flag = 0;

    strcpy (File_Compression_Suffix, ".gz");

    Error_Flag = ug_compress_file (Background_Flag, File_Compression_Suffix, UG_Param_File_Name);

    if (Error_Flag < 0)
      ug_error_message ("unable to compress file");

  strcpy (New_File_Name, UG_Param_File_Name);
  strcat (New_File_Name, ".gz");

  ug_message (" ");
  sprintf (Text, "Uncompress file %s using gzip", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_uncompress_file (New_File_Name);

    if (Error_Flag < 0)
      ug_error_message ("unable to uncompress file");

  ug_message (" ");
  sprintf (Text, "Compress file %s using compress", UG_Param_File_Name);
  ug_message (Text);

    Background_Flag = 0;

    strcpy (File_Compression_Suffix, ".Z");

    Error_Flag = ug_compress_file (Background_Flag, File_Compression_Suffix, UG_Param_File_Name);

    if (Error_Flag < 0)
      ug_error_message ("unable to compress file");

  strcpy (New_File_Name, UG_Param_File_Name);
  strcat (New_File_Name, ".Z");

  ug_message (" ");
  sprintf (Text, "Uncompress file %s using uncompress", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_uncompress_file (New_File_Name);

    if (Error_Flag < 0)
      ug_error_message ("unable to uncompress file");

  strcpy (New_File_Name, "copy.");
  strcat (New_File_Name, UG_Param_File_Name);

  ug_message (" ");
  sprintf (Text, "Copy file %s to %s", UG_Param_File_Name, New_File_Name);
  ug_message (Text);

    Error_Flag = ug_copy_file (UG_Param_File_Name, New_File_Name);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("unable to copy file");
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "File %s should be identical to %s", New_File_Name, UG_Param_File_Name);
  ug_message (Text);

  strcpy (System_Command, cat_Command);
  strcat (System_Command, New_File_Name);

    Error_Flag = ug_system (System_Command);
 
    if (Error_Flag != 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("Unable to execute system command");
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "Remove file %s", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_remove_file (New_File_Name);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("unable to remove file");
      exit (Error_Flag);
    }

  strcpy (New_File_Name, "copy.");
  strcat (New_File_Name, UG_Param_File_Name);

  ug_message (" ");
  sprintf (Text, "Rename file %s to %s", UG_Param_File_Name, New_File_Name);
  ug_message (Text);

    Error_Flag = ug_rename_file (UG_Param_File_Name, New_File_Name);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("unable to rename file");
      exit (Error_Flag);
    }

  ug_message (" ");
  sprintf (Text, "Remove file %s", New_File_Name);
  ug_message (Text);

    Error_Flag = ug_remove_file (New_File_Name);

    if (Error_Flag < 0)
    {
      ug_free_param (UG_Param_Struct_Ptr); 
      ug_error_message ("unable to remove file");
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Free UG parameter structure arrays.");

    ug_free_param (UG_Param_Struct_Ptr);

  if (! File_Status_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn off file status monitor");

      ug_file_status_monitor ("off");
  }

  ug_message (" ");
  ug_message ("Create named TMP files in named directory.");

    strcpy (TMP_File_Dir, "UG_junk_directory");

    ug_message (" ");
    sprintf (Text, "Set TMP file directory name to %s", TMP_File_Dir);
    ug_message (Text);

    Error_Flag = ug_set_tmp_file_dir (TMP_File_Dir);

    if (Error_Flag)
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);

    strcpy (File_Name1, "TMP_File_1");

    ug_message (" ");
    sprintf (Text, "Creating TMP file %s", File_Name1);
    ug_message (Text);

    File1 = ug_fopen (File_Name1, "tmp");

    strcpy (File_Name2, "TMP_File_2");

    sprintf (Text, "Creating TMP file %s", File_Name2);
    ug_message (Text);

    File2 = ug_fopen (File_Name2, "tmp");

    strcpy (Text, UG_SHELL_COMMAND_LS);
    strcat (Text, " ");
    strcat (Text, TMP_File_Dir);

    ug_message (" ");
    ug_message (Text);
    ug_message (" ");
    Error_Flag = ug_system (Text);

    ug_message (" ");

    v1 = 71.234;
    v2 = 75.678;
    v3 = 79.012;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fwrite (&v1, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fwrite (&v2, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fwrite (&v3, sizeof (double), 1, File1);

    sprintf (Text, "Wrote %i entries %g %g %g in TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name1);
    ug_message (Text);

    v1 = 3371.234;
    v2 = 4475.678;
    v3 = 7779.012;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fwrite (&v1, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fwrite (&v2, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fwrite (&v3, sizeof (double), 1, File2);

    sprintf (Text, "Wrote %i entries %g %g %g in TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name2);
    ug_message (Text);

    ug_rewind (File1);
    ug_rewind (File2);

    ug_message (" ");

    v1 = 0.0;
    v2 = 0.0;
    v3 = 0.0;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fread (&v1, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fread (&v2, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fread (&v3, sizeof (double), 1, File1);

    sprintf (Text, "Read %i entries %g %g %g from TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name1);
    ug_message (Text);

    v1 = 0.0;
    v2 = 0.0;
    v3 = 0.0;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fread (&v1, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fread (&v2, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fread (&v3, sizeof (double), 1, File2);

    sprintf (Text, "Read %i entries %g %g %g from TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name2);
    ug_message (Text);

    ug_fclose (File1);
    ug_fclose (File2);

  ug_message (" ");
  ug_message ("Removing all TMP files and directories created (if any).");

    ug_free_tmp_files ();

  ug_message (" ");
  ug_message ("Create named TMP files in system temp directory.");

    ug_message (" ");
    ug_message ("Clearing TMP file directory name.");

    Error_Flag = ug_set_tmp_file_dir ("NULL");

    if (Error_Flag)
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);

    strcpy (File_Name1, "TMP_File_1+");

    ug_message (" ");
    sprintf (Text, "Creating TMP file %s", File_Name1);
    ug_message (Text);

    File1 = ug_fopen (File_Name1, "tmp");

    strcpy (File_Name2, "TMP_File_2+");

    sprintf (Text, "Creating TMP file %s", File_Name2);
    ug_message (Text);

    File2 = ug_fopen (File_Name2, "tmp");

    v1 = 1.234;
    v2 = 5.678;
    v3 = 9.012;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fwrite (&v1, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fwrite (&v2, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fwrite (&v3, sizeof (double), 1, File1);

    ug_message (" ");
    sprintf (Text, "Wrote %i entries %g %g %g in TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name1);
    ug_message (Text);

    v1 = 331.234;
    v2 = 445.678;
    v3 = 779.012;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fwrite (&v1, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fwrite (&v2, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fwrite (&v3, sizeof (double), 1, File2);

    sprintf (Text, "Wrote %i entries %g %g %g in TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name2);
    ug_message (Text);

    ug_rewind (File1);
    ug_rewind (File2);

    v1 = 0.0;
    v2 = 0.0;
    v3 = 0.0;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fread (&v1, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fread (&v2, sizeof (double), 1, File1);
    Error_Flag = Error_Flag + ug_fread (&v3, sizeof (double), 1, File1);

    ug_message (" ");
    sprintf (Text, "Read %i entries %g %g %g from TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name1);
    ug_message (Text);

    v1 = 0.0;
    v2 = 0.0;
    v3 = 0.0;

    Error_Flag = 0;
    Error_Flag = Error_Flag + ug_fread (&v1, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fread (&v2, sizeof (double), 1, File2);
    Error_Flag = Error_Flag + ug_fread (&v3, sizeof (double), 1, File2);

    sprintf (Text, "Read %i entries %g %g %g from TMP file %s",
             (int) Error_Flag, v1, v2, v3, File_Name2);
    ug_message (Text);

    ug_fclose (File1);
    ug_fclose (File2);

  ug_message (" ");
  ug_message ("Removing all TMP files and directories created (if any).");

    ug_free_tmp_files ();

  ug_message (" ");
  ug_message ("Get base file name, extension and directory path from a file name with path.");

    for (Index = 1; Index <= 4; ++Index)
    {
           if (Index == 1) strcpy (File_Name1, ug_get_cwd ());
      else if (Index == 2) { strcpy (File_Name1, fname);
                             strcat (File_Name1, ext); }
      else if (Index == 3) { strcpy (File_Name1, ".");
                             strcat (File_Name1, UG_PATH_SEP);
                             strcat (File_Name1, "_JUNK_FILE.txt"); }
      else if (Index == 4) strcpy (File_Name1, argv[0]);

      ug_splitpath (File_Name1, drive, dname, fname, ext);

      ug_message (" ");
      sprintf (Text, "Full file name = %s", File_Name1);
      ug_message (Text);
      sprintf (Text, "Base file name from splitpath = %s", fname);
      ug_message (Text);
      sprintf (Text, "File name suffix from splitpath = %s", ext);
      ug_message (Text);
      sprintf (Text, "Drive name from splitpath = %s", drive);
      ug_message (Text);
      sprintf (Text, "Directory name from splitpath = %s", dname);
      ug_message (Text);
    }

  if (! Memory_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn on memory monitor");

      ug_memory_monitor ("on");
  }

  ug_message (" ");
  ug_message ("Malloc a test array with 100 entries.");

    Number_of_Array_Elements = 100;

    Error_Flag = 0;

    Test_Array = (INT_1D *) ug_malloc (&Error_Flag,
                                       Number_of_Array_Elements
                                     * sizeof (INT_1D));
 
    if (Error_Flag > 0)
    {
      ug_error_message ("Unable to malloc test array.");
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Set array to vary between -49 and 50.");

    Value = 50;

    for (Index = 0; Index < Number_of_Array_Elements; ++Index)
    {
      Test_Array[Index] = Value;

      --Value;
    }

  ug_message (" ");
  ug_message ("Get maximum and minimum value of array.");

    Max_Int_Value = ug_max_int (0, Number_of_Array_Elements-1, Test_Array);
    Min_Int_Value = ug_min_int (0, Number_of_Array_Elements-1, Test_Array);

  sprintf (Text, "Maximum value = %i", (int) Max_Int_Value);
  ug_message (Text);
  sprintf (Text, "Minimum value = %i", (int) Min_Int_Value);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Re-allocate test array with 1000 entries.");

    New_Number_of_Array_Elements = 1000;

    Error_Flag = 0;

    Test_Array = (INT_1D *) ug_realloc (&Error_Flag, Test_Array,
                                        New_Number_of_Array_Elements
                                      * sizeof (INT_1D));

    if (Error_Flag > 0)
    {
      ug_error_message ("Unable to realloc test array.");
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Set array to vary between -949 and 50.");

    for (Index = Number_of_Array_Elements;
         Index < New_Number_of_Array_Elements; ++Index)
    {
      Test_Array[Index] = Value;

      --Value;
    }

  ug_message (" ");
  ug_message ("Get maximum and minimum value of array.");

    Max_Int_Value = ug_max_int (0, New_Number_of_Array_Elements-1, Test_Array);
    Min_Int_Value = ug_min_int (0, New_Number_of_Array_Elements-1, Test_Array);

  sprintf (Text, "Maximum value = %i", (int) Max_Int_Value);
  ug_message (Text);
  sprintf (Text, "Minimum value = %i", (int) Min_Int_Value);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Limit array values to vary between -10 and 10.");

    ug_lim_max_int (0, New_Number_of_Array_Elements-1, 10, Test_Array);
    ug_lim_min_int (0, New_Number_of_Array_Elements-1, -10, Test_Array);

  ug_message (" ");
  ug_message ("Get maximum and minimum value of array.");

    Max_Int_Value = ug_max_int (0, New_Number_of_Array_Elements-1, Test_Array);
    Min_Int_Value = ug_min_int (0, New_Number_of_Array_Elements-1, Test_Array);

  sprintf (Text, "Maximum value = %i", (int) Max_Int_Value);
  ug_message (Text);
  sprintf (Text, "Minimum value = %i", (int) Min_Int_Value);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Free test array.");

    ug_free (Test_Array);

  ug_message (" ");
  ug_message ("Malloc and initialize a test array with 10 entries.");

    Number_of_Array_Elements = 10;

    Error_Flag = 0;

    Test_Array = (INT_1D *) ug_calloc (&Error_Flag,
                                       Number_of_Array_Elements,
                                       sizeof (INT_1D));
 
    if (Error_Flag > 0)
    {
      ug_error_message ("Unable to malloc and initialize test array.");
      exit (Error_Flag);
    }

  ug_message (" ");
  ug_message ("Array values should all be 0.");

    for (Index = 0; Index < Number_of_Array_Elements; ++Index)
    {
      sprintf (Text, "Array Value [%i] = %i", (int) Index, (int) Test_Array[Index]);
      ug_message (Text);
    }

  ug_message (" ");
  ug_message ("Free test array.");

    ug_free (Test_Array);

  if (! Memory_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn off memory monitor");

      ug_memory_monitor ("off");
  }

  ug_message (" ");
  ug_message ("Copy integer array");

    Start_Index = 1;
    End_Index = 21;

    Error_Flag = 0;

    Int_Array1 = (INT_1D *) ug_malloc (&Error_Flag,
                                       (End_Index+1) * sizeof (INT_1D));

    Int_Array2 = (INT_1D *) ug_malloc (&Error_Flag,
                                       (End_Index+1) * sizeof (INT_1D));
                            
 
    if (Error_Flag > 0)
    {
      ug_free (Int_Array1);
      ug_free (Int_Array2);
      ug_error_message ("Unable to malloc test array.");
      exit (Error_Flag);
    }

    for (Index = Start_Index; Index <= End_Index; ++Index)
    {
      Int_Array1[Index] = Index;
    }

    ug_copy_int (Start_Index, End_Index, Int_Array1, Int_Array2);

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    sprintf (Text, "Integer Array Value [%i] = %i (original) %i (copy)",
             (int) Index, (int) Int_Array1[Index], (int) Int_Array2[Index]);
    ug_message (Text);
  }

  ug_free (Int_Array1);
  ug_free (Int_Array2);

  ug_message (" ");
  ug_message ("Copy double array");

    Start_Index = 1;
    End_Index = 21;

    Error_Flag = 0;

    Double_Array1 = (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                       (End_Index+1) * sizeof (DOUBLE_1D));

    Double_Array2 = (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                       (End_Index+1) * sizeof (DOUBLE_1D));
                            
 
    if (Error_Flag > 0)
    {
      ug_free (Double_Array1);
      ug_free (Double_Array2);
      ug_error_message ("Unable to malloc test array.");
      exit (Error_Flag);
    }

    for (Index = Start_Index; Index <= End_Index; ++Index)
    {
      Double_Array1[Index] = ((double) Index) / 3.7;
    }

    ug_copy_double (Start_Index, End_Index, Double_Array1, Double_Array2);

  for (Index = Start_Index; Index <= End_Index; ++Index)
  {
    sprintf (Text, "Double Array Value [%i] = %g (original) %g (copy)",
             (int) Index, Double_Array1[Index], Double_Array2[Index]);
    ug_message (Text);
  }

  ug_message (" ");
  ug_message ("Get maximum and minimum value of array.");

    Max_double_Value = ug_max_double (Start_Index, End_Index, Double_Array1);
    Min_double_Value = ug_min_double (Start_Index, End_Index, Double_Array1);

  sprintf (Text, "Maximum value = %g", Max_double_Value);
  ug_message (Text);
  sprintf (Text, "Minimum value = %g", Min_double_Value);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Limit maximum and minimum values of array.");

    Max_double_Value = 0.9 * Max_double_Value;
    Min_double_Value = 1.1 * Min_double_Value;

    sprintf (Text, "Maximum limit = %g", Max_double_Value);
    ug_message (Text);
    sprintf (Text, "Minimum limit = %g", Min_double_Value);
    ug_message (Text);

    ug_lim_max_double (Start_Index, End_Index, Max_double_Value, Double_Array1);
    ug_lim_min_double (Start_Index, End_Index, Min_double_Value, Double_Array1);

  ug_message (" ");
  ug_message ("Get maximum and minimum value of array.");

    Max_double_Value = ug_max_double (Start_Index, End_Index, Double_Array1);
    Min_double_Value = ug_min_double (Start_Index, End_Index, Double_Array1);

  sprintf (Text, "Maximum value = %g", Max_double_Value);
  ug_message (Text);
  sprintf (Text, "Minimum value = %g", Min_double_Value);
  ug_message (Text);

  ug_free (Double_Array1);
  ug_free (Double_Array2);

  ug_message (" ");
  ug_message ("Get nearest integer to 3.500001.");

    double_Value = 3.500001;

    Value = NINT (double_Value);

  sprintf (Text, "Nearest integer = %i", (int) Value);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Get nearest integer to 3.499999.");

    double_Value = 3.499999;

    Value = NINT (double_Value);

  sprintf (Text, "Nearest integer = %i", (int) Value);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Determine round-off error.");

    ug_round_off_error (&Round_Off_Error);

  sprintf (Text, "Round-off error = %g", Round_Off_Error);
  ug_message (Text);

  ug_message (" ");
  ug_message ("Write total CPU time since start.");

    ug_cpu_message ("UG TEST ROUTINE: ");

  ug_message (" ");
  ug_message ("Write a standard error output message.");

    ug_error_message ("THIS IS THE STANDARD ERROR OUTPUT MESSAGE.");

  Error_Flag = ug_check_renumber ();

    Known_Error = 0;

    if (Error_Flag > 0)
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);

  Error_Flag = ug_check_sort ();

    Known_Error = 0;

    if (Error_Flag > 0)
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);

  ug_message (" ");
  ug_message ("Create another named TMP file directory.");

    strcpy (TMP_File_Dir, "xxx_UG_junk_directory");

    ug_message (" ");
    sprintf (Text, "Set TMP file directory name to %s", TMP_File_Dir);
    ug_message (Text);

    Error_Flag = ug_set_tmp_file_dir (TMP_File_Dir);

    if (Error_Flag)
      ug_lib_error_message (Error_Message, Error_Flag, &Known_Error);

    strcpy (Text, UG_SHELL_COMMAND_LS);
    strcat (Text, " ");
    strcat (Text, TMP_File_Dir);

    ug_message (" ");
    ug_message (Text);
    ug_message (" ");
    Error_Flag = ug_system (Text);

  if (File_Status_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn off file status monitor");

      ug_file_status_monitor ("off");
  }

  if (Memory_Monitor)
  {
    ug_message (" ");
    ug_message ("Turn off memory monitor");

      ug_memory_monitor ("off");
  }

  ug_message (" ");
  ug_message ("Register a dummy external routine for standard output messages.");

    ug_register_message (dummy_ext_ug_message_routine);

  ug_message (" ");
  ug_message ("This is a message sent through the registered routine.");

  ug_message (" ");
  ug_message ("De-register external standard output message routine.");

    ug_register_message (NULL);

  ug_message (" ");
  ug_message ("Register a dummy external routine for standard error output messages.");

    ug_register_error_message (dummy_ext_ug_error_message_routine);

  ug_error_message (" ");
  ug_error_message ("This is a message sent through the registered routine.");

  ug_message (" ");
  ug_message ("De-register external standard error output message routine.");

    ug_register_error_message (NULL);

  ug_message (" ");
  ug_message ("Access memory that is out of range.");
  ug_message ("The signal handler should trap this error and report that a");
  ug_message ("system SIGSEGV error has been reported due to an illegal memory access.");
  ug_message ("");
  ug_message ("NOTE: The previously created TMP file directory should be removed");
  ug_message ("automatically by the signal handler cleanup process.");
  ug_message ("");

    Index = 1234567890;
    Value = 123;

    sprintf (Text, "Attempting to set  Int_Array1[%i] = %i", (int) Index, (int) Value);
    ug_message (Text);
    ug_message ("");
    ug_message ("A system error message and comment from the external signal cleanup routine");
    ug_message ("should follow and then the program should terminate.");
    ug_message ("");

    Int_Array1[Index] = Value;

    ug_message ("+++ THIS MESSAGE AND NEXT SHOULD NOT APPEAR IF THE SIGNAL HANDLER WAS SET +++");
    ug_message ("");
    sprintf (Text, "Value actually set Int_Array1[%i] = %i", (int) Index, (int) Int_Array1[Index]);
    ug_message (Text);
    ug_message ("");

  exit (0);

}

void dummy_ext_signal_cleanup_routine (void)
{
  ug_error_message ("");
  ug_error_message ("~~~ Inside dummy external signal cleanup routine. ~~~");
  ug_error_message ("~~~ This routine was registered at the start of the program. ~~~");
  return;
}

void dummy_ext_ug_message_routine (FILE *UG_Output_File, INT_ UG_Standard_Output_Flag, char *text)
{
  printf ("dummy_ext_ug_message_routine : %s\n", text);

  fflush (stdout);

  return;
}

void dummy_ext_ug_error_message_routine (FILE *UG_Output_File, char *text)
{
  fprintf (stderr, "dummy_ext_ug_error_message_routine : %s\n", text);

  fflush (stderr);

  return;
}

INT_ ug_check_renumber (void)
{
  CHAR_133 Text;

  INT_1D *int_values = NULL;
  INT_1D *new_int_values = NULL;
  INT_1D *map = NULL;

  INT_ i, j, number_of_new_int_values, max_int_value, min_int_value;
  INT_ ierr = 0;
  INT_ n = 13;

  ug_message ("");
  ug_message ("Testing renumbering routine ug_renumber_int_values");
  ug_message ("using ascending order");

  int_values = (INT_1D *) ug_malloc (&ierr, (n+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (int_values);
    return (100499);
  }

  int_values[1] = 3;
  int_values[2] = -1;
  int_values[3] = 5;
  int_values[4] = 5;
  int_values[5] = 3;
  int_values[6] = 3;
  int_values[7] = 5;
  int_values[8] = 0;
  int_values[9] = 0;
  int_values[10] = -1;
  int_values[11] = 5;
  int_values[12] = 0;
  int_values[13] = 3;

  number_of_new_int_values = 0;

  ierr = ug_renumber_int_values (n, &number_of_new_int_values,
                                 &max_int_value, &min_int_value,
                                 int_values, new_int_values, &map);

  if (ierr > 0)
  {
    ug_free (int_values);
    ug_free (map);
    return (ierr);
  }

  ug_message ("");
  sprintf (Text, "number of new int values = %i", (int) number_of_new_int_values);
  ug_message (Text);

  ug_message ("");
  ug_message (" old value  new value");

  for (j = 0; j <= max_int_value-min_int_value; j++)
  {
    if (map[j])
    {
      sprintf (Text, "%10i %10i", (int) (j+min_int_value), (int) map[j]);
      ug_message (Text);
    }
  }

  ug_message ("");
  ug_message ("     index old value  new value");

  for (i = 1; i <= n; i++)
  {
    j = int_values[i] - min_int_value;

    sprintf (Text, "%10i %10i %10i", (int) i, (int) int_values[i], (int) map[j]);
    ug_message (Text);
  }

  ug_free (map);

  ug_message ("");
  ug_message ("Testing renumbering routine ug_renumber_int_values");
  ug_message ("using first encountered order");

  number_of_new_int_values = -1;

  ierr = ug_renumber_int_values (n, &number_of_new_int_values,
                                 &max_int_value, &min_int_value,
                                 int_values, new_int_values, &map);

  if (ierr > 0)
  {
    ug_free (int_values);
    ug_free (map);
    return (ierr);
  }

  ug_message ("");
  sprintf (Text, "number of new int values = %i", (int) number_of_new_int_values);
  ug_message (Text);

  ug_message ("");
  ug_message (" old value  new value");

  for (j = 0; j <= max_int_value-min_int_value; j++)
  {
    if (map[j])
    {
      sprintf (Text, "%10i %10i", (int) (j+min_int_value), (int) map[j]);
      ug_message (Text);
    }
  }

  ug_message ("");
  ug_message ("     index old value  new value");

  for (i = 1; i <= n; i++)
  {
    j = int_values[i] - min_int_value;

    sprintf (Text, "%10i %10i %10i", (int) i, (int) int_values[i], (int) map[j]);
    ug_message (Text);
  }

  ug_free (int_values);
  ug_free (map);

  return (0);
}

INT_ ug_check_sort (void)
{
  CHAR_133 Text;

  INT_1D *random_list = NULL;
  INT_1D *list = NULL;
  INT_1D *int_values = NULL;

  INT_ i, i1, i2, iv, j, m, rmax;
  INT_ ierr = 0;
  INT_ lmax = 1000000000;
  INT_ n = 1000;

  DOUBLE_1D *double_values = NULL;

  double dv, w;
  double dc100 = 100.0;

  ug_message ("");
  ug_message ("Testing sorting routines ug_sort_int_list and ug_sort_double_list");

  random_list = (INT_1D *) ug_malloc (&ierr, n * sizeof (INT_1D));
  list = (INT_1D *) ug_malloc (&ierr, n * sizeof (INT_1D));
  int_values = (INT_1D *) ug_malloc (&ierr, n * sizeof (INT_1D));
  double_values = (DOUBLE_1D *) ug_malloc (&ierr, n * sizeof (DOUBLE_1D));

  if (ierr > 0)
  {
    ug_free (random_list);
    ug_free (list);
    ug_free (int_values);
    ug_free (double_values);
    return (100499);
  }

  ug_random_list (n, lmax, random_list-1);

  rmax = ug_max_int (0, n-1, random_list);

  w = dc100 / ((double) rmax);

  for (m = 0; m <= 1; ++m)
  {
    for (i = 0; i < n; ++i)
    {
      int_values[i] = random_list[i];
    }

    ierr = ug_sort_int_list (m, n, int_values-1, list-1);

    if (ierr > 0)
    {
      ug_free (random_list);
      ug_free (list);
      ug_free (int_values);
      ug_free (double_values);
      return (ierr);
    }

    i1 = 0;
    i2 = MIN (n, 10);

    ug_message ("");

    for (i = i1; i < i2; ++i)
    {
      j = (m == 0) ? list[i]-1: i;

      iv = random_list[i];

      sprintf (Text, "%10i %10i %10i %10i", (int) (i+1), (int) iv, (int) list[i], (int) int_values[j]);
      ug_message (Text);
    }

    if (n > 10)
    {
      i1 = n-10;
      i2 = n;

      ug_message ("");

      for (i = i1; i < i2; ++i)
      {
        j = (m == 0) ? list[i]-1: i;

        iv = random_list[i];

        sprintf (Text, "%10i %10i %10i %10i", (int) (i+1), (int) iv, (int) list[i], (int) int_values[j]);
        ug_message (Text);
      }
    }

    for (i = 0; i < n; ++i)
    {
      double_values[i] = w * ((double) random_list[i]);
    }

    ierr = ug_sort_double_list (m, n, double_values-1, list-1);

    if (ierr > 0)
    {
      ug_free (random_list);
      ug_free (list);
      ug_free (int_values);
      ug_free (double_values);
      return (ierr);
    }

    i1 = 0;
    i2 = MIN (n, 10);

    ug_message ("");

    for (i = i1; i < i2; ++i)
    {
      j = (m == 0) ? list[i]-1: i;

      dv = w * ((double) random_list[i]);

      sprintf (Text, "%10i %14.6g %10i %14.6g",
               (int) (i+1), dv, (int) list[i], double_values[j]);
      ug_message (Text);
    }

    if (n > 10)
    {
      i1 = n-10;
      i2 = n;

      ug_message ("");

      for (i = i1; i < i2; ++i)
      {
        j = (m == 0) ? list[i]-1: i;

        dv = w * ((double) random_list[i]);

        sprintf (Text, "%10i %14.6g %10i %14.6g",
                 (int) (i+1), dv, (int) list[i], double_values[j]);
        ug_message (Text);
      }
    }
  }

  ug_free (random_list);
  ug_free (list);
  ug_free (int_values);
  ug_free (double_values);

  return (0);
}

INT_ ug_initialize_test_param
 (char Error_Message[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set the name, flag, default value, minimum allowable value, maximum allowable
 * value, and description for all UG test parameters.
 *
 * This routine is used only for testing and illustration of how UG parameters
 * can be set.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug.c,v 1.93 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

/*
 * -----------------------------------------------------------------------------
 * Declare variables.
 * -----------------------------------------------------------------------------
 */

  CHAR_UG_MAX *Def_Char_Param;

  CHAR_11 *Char_Param_Flag, *Double_Param_Flag, *Int_Param_Flag;

  CHAR_25 *Char_Param_Name, *Double_Param_Name, *Int_Param_Name;
  CHAR_UG_MAX *Char_Param_Alt_Name, *Double_Param_Alt_Name, *Int_Param_Alt_Name;

  CHAR_41 *Char_Param_Label, *Double_Param_Label, *Int_Param_Label;

  INT_1D *Char_Param_Type, *Double_Param_Vector_Entries, *Int_Param_Vector_Entries,
         *Def_Int_Param, *Max_Int_Param, *Min_Int_Param;

  DOUBLE_1D *Def_Double_Param, *Max_Double_Param, *Min_Double_Param;

  INT_ IV[10];
  double DV[10];

  INT_ Error_Flag, Index, Max_Char_Params, Max_Double_Params, Max_Int_Params,
       Max_Param_Labels, NDV, NIV,
       Number_of_Char_Params, Number_of_Char_Params_i,
       Number_of_Double_Params, Number_of_Double_Params_i,
       Number_of_Int_Params, Number_of_Int_Params_i;

/*
 * -----------------------------------------------------------------------------
 * Set local variables from structure.
 * -----------------------------------------------------------------------------
 */

  Max_Int_Params = UG_Param_Struct_Ptr->Max_Int_Params;
  Int_Param_Flag = UG_Param_Struct_Ptr->Int_Param_Flag;
  Int_Param_Label = UG_Param_Struct_Ptr->Int_Param_Label;
  Int_Param_Name = UG_Param_Struct_Ptr->Int_Param_Name;
  Int_Param_Alt_Name = UG_Param_Struct_Ptr->Int_Param_Alt_Name;
  Int_Param_Vector_Entries = UG_Param_Struct_Ptr->Int_Param_Vector_Entries;
  Def_Int_Param = UG_Param_Struct_Ptr->Def_Int_Param;
  Max_Int_Param = UG_Param_Struct_Ptr->Max_Int_Param;
  Min_Int_Param = UG_Param_Struct_Ptr->Min_Int_Param;

  Max_Double_Params = UG_Param_Struct_Ptr->Max_Double_Params;
  Double_Param_Flag = UG_Param_Struct_Ptr->Double_Param_Flag;
  Double_Param_Label = UG_Param_Struct_Ptr->Double_Param_Label;
  Double_Param_Name = UG_Param_Struct_Ptr->Double_Param_Name;
  Double_Param_Alt_Name = UG_Param_Struct_Ptr->Double_Param_Alt_Name;
  Double_Param_Vector_Entries = UG_Param_Struct_Ptr->Double_Param_Vector_Entries;
  Def_Double_Param = UG_Param_Struct_Ptr->Def_Double_Param;
  Max_Double_Param = UG_Param_Struct_Ptr->Max_Double_Param;
  Min_Double_Param = UG_Param_Struct_Ptr->Min_Double_Param;

  Max_Char_Params = UG_Param_Struct_Ptr->Max_Char_Params;
  Char_Param_Flag = UG_Param_Struct_Ptr->Char_Param_Flag;
  Char_Param_Label = UG_Param_Struct_Ptr->Char_Param_Label;
  Char_Param_Name = UG_Param_Struct_Ptr->Char_Param_Name;
  Char_Param_Alt_Name = UG_Param_Struct_Ptr->Char_Param_Alt_Name;
  Def_Char_Param = UG_Param_Struct_Ptr->Def_Char_Param;
  Char_Param_Type = UG_Param_Struct_Ptr->Char_Param_Type;

  Max_Param_Labels = UG_Param_Struct_Ptr->Max_Param_Labels;

  Number_of_Int_Params_i = UG_Param_Struct_Ptr->Number_of_Int_Params;
  Number_of_Double_Params_i = UG_Param_Struct_Ptr->Number_of_Double_Params;
  Number_of_Char_Params_i = UG_Param_Struct_Ptr->Number_of_Char_Params;

/*
 * -----------------------------------------------------------------------------
 * Initialize "new" int parameters.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */
 
  Index = Number_of_Int_Params_i-1;

  ++Index;

  #include "ug_initialize_param_realloc_int.h"

  strcpy (Int_Param_Name[Index], "Int_Param_1");
  strcpy (Int_Param_Alt_Name[Index], "i1,int1,intp1");
  strcpy (Int_Param_Flag[Index], "yes");
  strcpy (Int_Param_Label[  Index*Max_Param_Labels], "Integer parameter #1.");
  strcpy (Int_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Int_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Int_Param_Vector_Entries[Index] = -1;
  Def_Int_Param[Index] = 1;
  Min_Int_Param[Index] = 0;
  Max_Int_Param[Index] = 9;

  ++Index;

  #include "ug_initialize_param_realloc_int.h"

  strcpy (Int_Param_Name[Index], "Int_Param_2");
  strcpy (Int_Param_Alt_Name[Index], "i2,int2,intp2");
  strcpy (Int_Param_Flag[Index], "yes");
  strcpy (Int_Param_Label[  Index*Max_Param_Labels], "Integer parameter #2.");
  strcpy (Int_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Int_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Int_Param_Vector_Entries[Index] = -1;
  Def_Int_Param[Index] = 2;
  Min_Int_Param[Index] = 0;
  Max_Int_Param[Index] = 9;

  ++Index;

  #include "ug_initialize_param_realloc_int.h"

  strcpy (Int_Param_Name[Index], "Int_Param_3");
  strcpy (Int_Param_Flag[Index], "yes");
  strcpy (Int_Param_Label[  Index*Max_Param_Labels], "Integer parameter #3.");
  strcpy (Int_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Int_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Int_Param_Vector_Entries[Index] = -1;
  Def_Int_Param[Index] = 3;
  Min_Int_Param[Index] = 0;
  Max_Int_Param[Index] = 9;

  ++Index;

  #include "ug_initialize_param_realloc_int.h"

  strcpy (Int_Param_Name[Index], "Int_Param_4");
  strcpy (Int_Param_Alt_Name[Index], "i4,int4,intp4");
  strcpy (Int_Param_Flag[Index], "no");
  strcpy (Int_Param_Label[  Index*Max_Param_Labels], "Integer parameter #4.");
  strcpy (Int_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as not user");
  strcpy (Int_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Int_Param_Vector_Entries[Index] = -1;
  Def_Int_Param[Index] = 4;
  Min_Int_Param[Index] = 0;
  Max_Int_Param[Index] = 9;
 
/*
 * -----------------------------------------------------------------------------
 * Initialize "new" int parameter vectors.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */

  ++Index;

  #include "ug_initialize_param_realloc_int.h"

  strcpy (Int_Param_Name[Index], "Int_Param_Vector_1");
  strcpy (Int_Param_Alt_Name[Index], "iv1,intv1,intpv1");
  strcpy (Int_Param_Flag[Index], "yes");
  strcpy (Int_Param_Label[  Index*Max_Param_Labels], "Integer parameter vector #1.");
  strcpy (Int_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Int_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Int_Param_Vector_Entries[Index] = 0;
  Def_Int_Param[Index] = 0;
  Min_Int_Param[Index] = 0;
  Max_Int_Param[Index] = 0;

  ++Index;

  #include "ug_initialize_param_realloc_int.h"

  strcpy (Int_Param_Name[Index], "Int_Param_Vector_2");
  strcpy (Int_Param_Alt_Name[Index], "iv2,intv2,intpv2");
  strcpy (Int_Param_Flag[Index], "no");
  strcpy (Int_Param_Label[  Index*Max_Param_Labels], "Integer parameter vector #2.");
  strcpy (Int_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as not user");
  strcpy (Int_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Int_Param_Vector_Entries[Index] = 0;
  Def_Int_Param[Index] = 0;
  Min_Int_Param[Index] = 0;
  Max_Int_Param[Index] = 0;
 
/*
 * -----------------------------------------------------------------------------
 * Set number of int parameters in structure.
 * -----------------------------------------------------------------------------
 */

  ++Index;

  Number_of_Int_Params = Index;

  UG_Param_Struct_Ptr->Number_of_Int_Params = Number_of_Int_Params;
 
/*
 * -----------------------------------------------------------------------------
 * Initialize "new" double parameters.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */

  Index = Number_of_Double_Params_i-1;

  ++Index;

  #include "ug_initialize_param_realloc_double.h"

  strcpy (Double_Param_Name[Index], "Double_Param_1");
  strcpy (Double_Param_Alt_Name[Index], "d1,dbl1,dblp1");
  strcpy (Double_Param_Flag[Index], "yes");
  strcpy (Double_Param_Label[  Index*Max_Param_Labels], "Double parameter #1.");
  strcpy (Double_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Double_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Double_Param_Vector_Entries[Index] = -1;
  Def_Double_Param[Index] = 1.1;
  Min_Double_Param[Index] = 0.0;
  Max_Double_Param[Index] = 9.9;

  ++Index;

  #include "ug_initialize_param_realloc_double.h"

  strcpy (Double_Param_Name[Index], "Double_Param_2");
  strcpy (Double_Param_Alt_Name[Index], "d2,dbl2,dblp2");
  strcpy (Double_Param_Flag[Index], "yes");
  strcpy (Double_Param_Label[  Index*Max_Param_Labels], "Double parameter #2.");
  strcpy (Double_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Double_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Double_Param_Vector_Entries[Index] = -1;
  Def_Double_Param[Index] = 1.2;
  Min_Double_Param[Index] = 0.0;
  Max_Double_Param[Index] = 9.9;

  ++Index;

  #include "ug_initialize_param_realloc_double.h"

  strcpy (Double_Param_Name[Index], "Double_Param_3");
  strcpy (Double_Param_Alt_Name[Index], "d3,dbl3,dblp3");
  strcpy (Double_Param_Flag[Index], "yes");
  strcpy (Double_Param_Label[  Index*Max_Param_Labels], "Double parameter #3.");
  strcpy (Double_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Double_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Double_Param_Vector_Entries[Index] = -1;
  Def_Double_Param[Index] = 1.3;
  Min_Double_Param[Index] = 0.0;
  Max_Double_Param[Index] = 9.9;

  ++Index;

  #include "ug_initialize_param_realloc_double.h"

  strcpy (Double_Param_Name[Index], "Double_Param_4");
  strcpy (Double_Param_Alt_Name[Index], "d4,dbl4,dblp4");
  strcpy (Double_Param_Flag[Index], "no");
  strcpy (Double_Param_Label[  Index*Max_Param_Labels], "Double parameter #4.");
  strcpy (Double_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as not user");
  strcpy (Double_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Double_Param_Vector_Entries[Index] = -1;
  Def_Double_Param[Index] = 1.4;
  Min_Double_Param[Index] = 0.0;
  Max_Double_Param[Index] = 9.9;
 
/*
 * -----------------------------------------------------------------------------
 * Initialize "new" double parameter vectors.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */

  ++Index;

  #include "ug_initialize_param_realloc_double.h"

  strcpy (Double_Param_Name[Index], "Double_Param_Vector_1");
  strcpy (Double_Param_Alt_Name[Index], "dv1,dblv1,dblpv1");
  strcpy (Double_Param_Flag[Index], "no");
  strcpy (Double_Param_Label[  Index*Max_Param_Labels], "Double parameter vector #1.");
  strcpy (Double_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as not user");
  strcpy (Double_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  Double_Param_Vector_Entries[Index] = 0;
  Def_Double_Param[Index] = 0.0;
  Min_Double_Param[Index] = 0.0;
  Max_Double_Param[Index] = 0.0;
 
/*
 * -----------------------------------------------------------------------------
 * Set number of double parameters in structure.
 * -----------------------------------------------------------------------------
 */

  ++Index;

  Number_of_Double_Params = Index;

  UG_Param_Struct_Ptr->Number_of_Double_Params = Number_of_Double_Params;
 
/*
 * -----------------------------------------------------------------------------
 * Initialize "new" char parameters of type character string.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */
 
  Index = Number_of_Char_Params_i-1;

  ++Index;

  #include "ug_initialize_param_realloc_char.h"

  strcpy (Char_Param_Name[Index], "Char_Param_1");
  strcpy (Char_Param_Alt_Name[Index], "c1,char1,charp1");
  strcpy (Char_Param_Flag[Index], "yes");
  strcpy (Char_Param_Label[  Index*Max_Param_Labels], "Charater string parameter #1.");
  strcpy (Char_Param_Label[1+Index*Max_Param_Labels], "A variable that is labeled as user");
  strcpy (Char_Param_Label[2+Index*Max_Param_Labels], "adjustable.");
  strcpy (Def_Char_Param[Index], "_character_string_");
 
/*
 * -----------------------------------------------------------------------------
 * Initialize "new" opt parameters as type-2 character strings.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */
 
  ++Index;

  #include "ug_initialize_param_realloc_char.h"

  strcpy (Char_Param_Name[Index], "-version");
  strcpy (Char_Param_Alt_Name[Index], "");
  strcpy (Char_Param_Flag[Index], "yes");
  strcpy (Char_Param_Label[  Index*Max_Param_Labels], "Version Flag.");
  strcpy (Char_Param_Label[1+Index*Max_Param_Labels], "A type-2 option flag that is labeled as");
  strcpy (Char_Param_Label[2+Index*Max_Param_Labels], "user adjustable.");
  strcpy (Def_Char_Param[Index], "");
  Char_Param_Type[Index] = 2;
 
/*
 * -----------------------------------------------------------------------------
 * Initialize "new" opt parameters as type-2 character strings.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */
 
  ++Index;

  #include "ug_initialize_param_realloc_char.h"

  strcpy (Char_Param_Name[Index], "Opt1");
  strcpy (Char_Param_Alt_Name[Index], "o1,opt1");
  strcpy (Char_Param_Flag[Index], "yes");
  strcpy (Char_Param_Label[  Index*Max_Param_Labels], "Option Flag #1.");
  strcpy (Char_Param_Label[1+Index*Max_Param_Labels], "A type-2 option flag that is labeled as");
  strcpy (Char_Param_Label[2+Index*Max_Param_Labels], "user adjustable.");
  strcpy (Def_Char_Param[Index], "i1=3 d1=7.00003441");
  Char_Param_Type[Index] = 2;
 
/*
 * -----------------------------------------------------------------------------
 * Initialize "new" opt value parameters as type-3 character strings.
 * If any have already been set then the "new" ones are added at the end.
 * -----------------------------------------------------------------------------
 */
 
  ++Index;

  #include "ug_initialize_param_realloc_char.h"

  strcpy (Char_Param_Name[Index], "Opt2");
  strcpy (Char_Param_Alt_Name[Index], "o2,opt2");
  strcpy (Char_Param_Flag[Index], "yes");
  strcpy (Char_Param_Label[  Index*Max_Param_Labels], "Option Flag #2.");
  strcpy (Char_Param_Label[1+Index*Max_Param_Labels], "A type-3 option flag that is labeled as");
  strcpy (Char_Param_Label[2+Index*Max_Param_Labels], "user adjustable.");
  strcpy (Def_Char_Param[Index], "i2=7 d3=0.09963 iv2=");
  Char_Param_Type[Index] = 3;

/*
 * -----------------------------------------------------------------------------
 * Set number of char parameters in structure.
 * -----------------------------------------------------------------------------
 */

  ++Index;

  Number_of_Char_Params = Index;

  UG_Param_Struct_Ptr->Number_of_Char_Params = Number_of_Char_Params;
 
/*
 * -----------------------------------------------------------------------------
 * Set all "new" parameter values to the default value set in arrays
 * Def_Int_Param, Def_Char_Param, Def_Double_Param, and
 * Def_Char_Param.
 * -----------------------------------------------------------------------------
 */

  ug_set_param_def (Number_of_Char_Params_i, Number_of_Double_Params_i,
                    Number_of_Int_Params_i, UG_Param_Struct_Ptr);
 
/*
 * -----------------------------------------------------------------------------
 * Set all "new" parameter vector values.
 * -----------------------------------------------------------------------------
 */

  NIV = 6;

  IV[0] = 98;
  IV[1] = 54;
  IV[2] = 32;
  IV[3] = 110;
  IV[4] = 2;
  IV[5] = 443;

  Error_Flag = ug_set_int_param_vector ("Int_Param_Vector_1", 
                                        NIV, IV, UG_Param_Struct_Ptr);

  if (Error_Flag != 1)
    return (Error_Flag);

  NIV = 3;

  IV[0] = 72;
  IV[1] = 8;
  IV[2] = 900325;

  Error_Flag = ug_set_int_param_vector ("Int_Param_Vector_2", 
                                        NIV, IV, UG_Param_Struct_Ptr);

  if (Error_Flag != 1)
    return (Error_Flag);

  NDV = 3;

  DV[0] = 3.5;
  DV[1] = 932.00003;
  DV[2] = 12314.983;

  Error_Flag = ug_set_double_param_vector ("Double_Param_Vector_1", 
                                           NDV, DV, UG_Param_Struct_Ptr);

  if (Error_Flag != 1)
    return (Error_Flag);

  return (0);

}
