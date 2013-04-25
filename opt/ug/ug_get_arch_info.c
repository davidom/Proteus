#include "UG_LIB.h"

INT_ ug_get_arch_info
 (char arch_mode32_suffix[],
  char arch_mode64_suffix[],
  char base_arch_type[],
  char base_sys_arch_type[],
  char def_arch_mode_suffix[],
  char def_arch_type[])

/*
 * Get system architecture information for naming UG architecture dependent 
 * directories.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_arch_info.c,v 1.5 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

{
  CHAR_UG_MAX machine_type, os_release, os_type;

  ug_get_sys_info (machine_type, os_release, os_type);

  if (strstr (os_type, "CYGWIN") || strstr (os_type, "MINGW"))
  {
    strcpy (arch_mode32_suffix, "");
    strcpy (arch_mode64_suffix, "64");
    strcpy (base_arch_type, "CYGWIN");
    strcpy (base_sys_arch_type, "CYGWIN");
    strcpy (def_arch_mode_suffix, "");
  }

  else if (strstr (os_type, "Linux"))
  {
    if (strstr (machine_type, "i386"))
    {
      strcpy (arch_mode32_suffix, "");
      strcpy (arch_mode64_suffix, "-64");
      strcpy (base_arch_type, "Linux-x86");
      strcpy (base_sys_arch_type, "Linux");
      strcpy (def_arch_mode_suffix, "");
    }
    else if (strstr (machine_type, "x86_64"))
    {
      strcpy (arch_mode32_suffix, "");
      strcpy (arch_mode64_suffix, "-64");
      strcpy (base_arch_type, "Linux-x86");
      strcpy (base_sys_arch_type, "Linux");
      strcpy (def_arch_mode_suffix, "-64");
    }
    else if (strstr (machine_type, "ia64"))
    {
      strcpy (arch_mode32_suffix, "");
      strcpy (arch_mode64_suffix, "");
      strcpy (base_arch_type, "Linux-ia64");
      strcpy (base_sys_arch_type, "Linux");
      strcpy (def_arch_mode_suffix, "");
    }
    else
      return (416);
  }

  else if (strstr (os_type, "Darwin"))
  {
    if (strstr (machine_type, "i386"))
    {
      strcpy (arch_mode32_suffix, "");
      strcpy (arch_mode64_suffix, "-64");
      strcpy (base_arch_type, "MacOSX-x86");
      strcpy (base_sys_arch_type, "MacOSX");
      strcpy (def_arch_mode_suffix, "");
    }
    else if (strstr (machine_type, "x86_64"))
    {
      strcpy (arch_mode32_suffix, "");
      strcpy (arch_mode64_suffix, "-64");
      strcpy (base_arch_type, "MacOSX-x86");
      strcpy (base_sys_arch_type, "MacOSX");
      strcpy (def_arch_mode_suffix, "-64");
    }
    else
      return (416);
  }

  else if (strstr (os_type, "IRIX") || strstr (os_type, "IRIX64"))
  {
      strcpy (arch_mode32_suffix, "");
      strcpy (arch_mode64_suffix, "-64");
      strcpy (base_arch_type, "IRIX");
      strcpy (base_sys_arch_type, "IRIX");
      strcpy (def_arch_mode_suffix, "");
  }

  else if (strstr (os_type, "SunOS"))
  {
      strcpy (arch_mode32_suffix, "");
      strcpy (arch_mode64_suffix, "-64");
      strcpy (base_arch_type, "SunOS");
      strcpy (base_sys_arch_type, "SunOS");
      strcpy (def_arch_mode_suffix, "");
  }

  else if (strstr (os_type, "WIN"))
  {
    strcpy (arch_mode32_suffix, "32");
    strcpy (arch_mode64_suffix, "64");
    strcpy (base_arch_type, "WIN");
    strcpy (base_sys_arch_type, "WIN");
    strcpy (def_arch_mode_suffix, "32");
  }

  else
    return (416);

  strcpy (def_arch_type, base_arch_type);
  strcat (def_arch_type, def_arch_mode_suffix);

  return (0);

}
