#include "UG_LIB.h"

void ug_version
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
strncpy (Version_Date, "03/16/13 @ 02:16PM", 40);
strncpy (Version_Number, "3.35.21", 40);

  strcpy (&(Compile_Date[40]), "\0");
  strcpy (&(Compile_OS[40]), "\0");
  strcpy (&(Version_Date[40]), "\0");
  strcpy (&(Version_Number[40]), "\0");

  return;

}
