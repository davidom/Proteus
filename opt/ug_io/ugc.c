#include "UG_IO_LIB.h"

int main
(INT_ argc,
  char *argv[])

{

/*
 * -----------------------------------------------------------------------------
 * UGC : UG_IO File Conversion Routine
 * -----------------------------------------------------------------------------
 */

  extern INT_ UG_Standard_Output_Flag;

  CHAR_UG_MAX Input_File_Name;
  CHAR_UG_MAX Output_File_Name;

  INT_ File_Name_Flag, Geom_Flag, Index, Interactive_Flag, Mode_Flag, Quad_Flag,
       Reorder_Surf_ID_Flag, Task_Flag, Version_Flag, Vol_ID_Flag;

  if (argc > 1)
  {
    if (strcmp (argv[1], "--build") == 0 || strcmp (argv[1], "--version") == 0)
    {
      if (strcmp (argv[1], "--build") == 0)
        UG_Standard_Output_Flag = 0;

      ug_version_info ("ug_io", ug_io_version);
      ug_version_info ("ug_cpp", ug_cpp_version);
      ug_version_info ("ug3", ug3_version);
      ug_version_info ("ug", ug_version);
#if defined (_USE_CGNS_LIB_)
        ug_version_info ("cgns", cgns_version);
#endif
#if defined (_USE_MESH_LIB_)
        ug_version_info ("lm5", lm5_version);
#endif

      UG_Standard_Output_Flag = 1;

      ug_version_info ("Build_Number", NULL);

      exit (0);
    }
  }

  strcpy (Input_File_Name, "");
  strcpy (Output_File_Name, "");

  File_Name_Flag = 0;
  Geom_Flag = 0;
  Interactive_Flag = 0;
  Mode_Flag = 0;
  Quad_Flag = 1;
  Reorder_Surf_ID_Flag = 0;
  Task_Flag = 0;
  Version_Flag = 0;
  Vol_ID_Flag = 0;

  for (Index = 1; Index < argc; ++Index)
  {
    if (strcmp(argv[Index], "list_summary_doc") == 0)
      Task_Flag = 1;

    else if (strcmp(argv[Index], "list_full_doc") == 0)
      Task_Flag = 2;

    else if (strcmp(argv[Index], "--1") == 0)
      Version_Flag = -1;

    else if (strcmp(argv[Index], "-2") == 0 || strcmp(argv[Index], "-2d") == 0)
      Mode_Flag = 2;

    else if (strcmp(argv[Index], "-b") == 0)
      Task_Flag = 4;

    else if (strcmp(argv[Index], "-br") == 0)
      Task_Flag = 5;

    else if (strcmp(argv[Index], "-idr") == 0)
      Reorder_Surf_ID_Flag = 1;

    else if (strcmp(argv[Index], "-idr2") == 0)
      Reorder_Surf_ID_Flag = 2;

    else if (strcmp(argv[Index], "-geom") == 0)
      Geom_Flag = 1;

    else if (strcmp(argv[Index], "-no_quad") == 0)
      Quad_Flag = 0;

    else if (strcmp(argv[Index], "-vol_id") == 0)
      Vol_ID_Flag = 1;

    else if (strcmp(argv[Index], "-i") == 0)
      Interactive_Flag = 1;

    else if (File_Name_Flag == 0)
    {
      File_Name_Flag = 1;

      strcpy (Input_File_Name, argv[Index]);
    }

    else
    {
      File_Name_Flag = 2;

      strcpy (Output_File_Name, argv[Index]);
    }
  }

  if (File_Name_Flag == 2)
  {
    if (Mode_Flag == 0)
      Task_Flag = MAX (Task_Flag, 3);
    else
      Task_Flag = 3;

    if (Interactive_Flag == 1)
      Task_Flag = - Task_Flag;
  }

  if (Task_Flag == 0 && argc == 1)
  {
    printf ("	[--1]\n");
    printf ("	[-2,-2d]\n");
    printf ("	[-b,-br]\n");
    printf ("	[-idr]\n");
    printf ("	[-idr2]\n");
    printf ("	[-i]\n");
    printf ("	[-geom]\n");
    printf ("	[-no_quad]\n");
    printf ("	[-vol_id]\n");
    printf ("	input_file\n");
    printf ("	output_file\n");
  }

  else if (Task_Flag == 0)
    printf ("*** both input and output file names must be specified ***\n");

  else if (Task_Flag == 1)
  {
    printf ("--1			: use previous version output file type definition\n");
    printf ("-2,-2d			: convert a 2D planar grid file\n");
    printf ("-b			: read in only boundary surface grid\n");
    printf ("-br			: read in only boundary surface grid and reorder by ID\n");
    printf ("-idr			: reorder input surface face IDs consequtively\n");
    printf ("-idr2			: reorder output file surface face IDs consequtively\n");
    printf ("-i			: enable interactive allocation mode\n");
    printf ("-geom			: output information for a geometry definition file\n");
    printf ("-no_quad		: convert quads to trias\n");
    printf ("-vol_id			: allocate, read, set, and write volume ID\n");
    printf ("input_file		: input file name\n");
    printf ("output_file		: output file name\n");
    printf ("\n");
    printf ("UG_IO File Names\n");
    printf ("----------------\n");
  }

  else if (Task_Flag == 2)
  {
    printf ("\n");
    printf ("Program Controls\n");
    printf ("================\n");
    printf ("\n");
    printf ("--1\n");
    printf ("\n");
    printf (" Use previous version output file type definition.\n");
    printf ("\n");
    printf ("-2,-2d\n");
    printf ("\n");
    printf (" Convert a 2D planar grid file.\n");
    printf ("\n");
    printf ("-b\n");
    printf ("\n");
    printf (" Read in only the boundary surface grid even if the input grid file contains a\n");
    printf (" volume grid.\n");
    printf ("\n");
    printf ("-br\n");
    printf ("\n");
    printf (" Read in only the boundary surface grid even if the input grid file contains a\n");
    printf (" volume grid, re-order the surface grid such that it is sorted into groups by\n");
    printf (" surface ID, and duplicate all nodes that are shared by faces with different\n");
    printf (" surface IDs.\n");
    printf ("\n");
    printf ("-idr\n");
    printf ("\n");
    printf (" Reorder input surface face IDs consequtively 1,2,3...\n");
    printf (" N/A with -2,-2d, or -geom.\n");
    printf ("\n");
    printf ("\n");
    printf ("-idr2\n");
    printf ("\n");
    printf (" Reorder output file surface face IDs consequtively 1,2,3...\n");
    printf (" N/A with -2,-2d, or -geom.\n");
    printf ("\n");
    printf ("-i\n");
    printf ("\n");
    printf (" Enable interactive allocation/de-allocation of output file function variables.\n");
    printf (" Only used if the output file is a function type.\n");
    printf ("\n");
    printf ("-geom\n");
    printf ("\n");
    printf (" Output information for a geometry definition file.\n");
    printf ("\n");
    printf ("-no_quad\n");
    printf ("\n");
    printf (" Convert quads to trias. Only applicable for 3D surface grid files.\n");
    printf ("\n");
    printf ("-vol_id\n");
    printf ("\n");
    printf (" Allocate, read, set, and write volume ID.\n");
    printf ("\n");
    printf ("input_file\n");
    printf ("\n");
    printf (" Specify input grid, node, flow, or function file to convert.\n");
    printf ("\n");
    printf ("output_file\n");
    printf ("\n");
    printf (" Specify output grid, node, flow, or function file.\n");
    printf (" If the input file is a grid file then the output file must be also.\n");
    printf (" If the input file is a node file then the output file must be also.\n");
    printf (" If the input file is a flow file then the output file must be a flow or\n");
    printf (" function type.\n");
    printf (" If the input file is a function file then the output file must be also.\n");
    printf ("\n");
    printf ("UG_IO File Names\n");
    printf ("================\n");
  }

  if (Task_Flag != 0)
  {
    if (Mode_Flag == 0)
    {
      if (Geom_Flag == 1 && (Task_Flag == 1 || Task_Flag == 2)) Task_Flag = -Task_Flag;
      if (Vol_ID_Flag == 1 && Task_Flag == -3) Task_Flag = -13;
      if (Vol_ID_Flag == 1 && Task_Flag == 3) Task_Flag = 13;

#if defined (_USE_CGNS_LIB_)
      ug_io_cgns_register_read_grid (cgns_ug_io_read_grid);
      ug_io_cgns_register_write_grid (cgns_ug_io_write_grid);
#endif

#if defined (_USE_MESH_LIB_)
      ug_io_mesh_register_read_grid (mesh_ug_io_read_grid);
      ug_io_mesh_register_write_grid (mesh_ug_io_write_grid);
#endif

      ug_io_convert (Input_File_Name, Output_File_Name,
                     Quad_Flag, Reorder_Surf_ID_Flag, Task_Flag, Version_Flag);
    }

    else if (Mode_Flag == 2)
    {
#if defined (_USE_MESH_LIB_)
      ug_io_mesh_register_read_2d_grid (mesh_ug_io_read_2d_grid);
      ug_io_mesh_register_write_2d_grid (mesh_ug_io_write_2d_grid);
#endif

      ug_io_convert_2d (Input_File_Name, Output_File_Name, Task_Flag);
    }
  }
 
  exit (0);
 
}

