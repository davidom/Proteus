#include "UG_CPP_LIB.h"

void ug_cpp_version
 (char Compile_Date[],
  char Compile_OS[],
  char Version_Date[],
  char Version_Number[])

{

/*
 * Put compile date, compile OS, version date, and version number in text
 * string.
 */

strncpy (Compile_Date, "", 40);
strncpy (Compile_OS, "", 40);
strncpy (Version_Date, "12/19/12 @ 05:34PM", 40);
strncpy (Version_Number, "1.0.7", 40);

  strcpy (&(Compile_Date[40]), "\0");
  strcpy (&(Compile_OS[40]), "\0");
  strcpy (&(Version_Date[40]), "\0");
  strcpy (&(Version_Number[40]), "\0");

  return;

}
