#ifndef __UG3_GRID_BC_MACRO_H__

#define __UG3_GRID_BC_MACRO_H__

#ifndef CHK_STD_BL_UG3_GBC
#define CHK_STD_BL_UG3_GBC(bc) ((-bc == STD_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_BL_UG3_GBC
#define CHK_BL_UG3_GBC(bc) ((-bc == STD_UG3_GBC || -bc == TRANSP_UG3_GBC || -bc == TRANSP_INTRNL_UG3_GBC || -bc == TMP_TRANSP_UG3_GBC || -bc == TMP_TRANSP_INTRNL_UG3_GBC || -bc == SOLID_TRANSP_UG3_GBC || -bc == SOLID_TRANSP_INTRNL_UG3_GBC || -bc == SOLID_TMP_TRANSP_UG3_GBC || -bc == SOLID_TMP_TRANSP_INTRNL_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_BL_INT_UG3_GBC
#define CHK_BL_INT_UG3_GBC(bc) ((bc == TRANSP_BL_INT_UG3_GBC || bc == BL_INT_UG3_GBC || -bc == BL_INT_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_FIXED_BL_INT_UG3_GBC
#define CHK_FIXED_BL_INT_UG3_GBC(bc) ((bc == FIXED_BL_INT_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_PART_UG3_GBC
#define CHK_PART_UG3_GBC(bc) ((bc == PART_UG3_GBC || bc == PART_STD_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_STD_UG3_GBC
#define CHK_STD_UG3_GBC(bc) ((bc == STD_UG3_GBC || bc == STD_NULL_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_TMP_UG3_GBC
#define CHK_TMP_UG3_GBC(bc) ((abs(bc) == TMP_TRANSP_UG3_GBC || abs(bc) == TMP_TRANSP_INTRNL_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_TRANSP_UG3_GBC
#define CHK_TRANSP_UG3_GBC(bc) ((bc == TRANSP_SRC_UG3_GBC || bc == TRANSP_BL_INT_UG3_GBC || abs(bc) == TRANSP_UG3_GBC || abs(bc) == TRANSP_INTRNL_UG3_GBC || abs(bc) == TMP_TRANSP_UG3_GBC || abs(bc) == TMP_TRANSP_INTRNL_UG3_GBC) ? 1 : 0)
#endif

#ifndef CHK_VALID_UG3_GBC
#define CHK_VALID_UG3_GBC(bc) ((CHK_BL_UG3_GBC (bc) == 1 || CHK_BL_INT_UG3_GBC (bc) == 1 || CHK_FIXED_BL_INT_UG3_GBC (bc) == 1 || CHK_PART_UG3_GBC (bc) == 1 || CHK_STD_UG3_GBC (bc) == 1 || CHK_TRANSP_UG3_GBC (bc) == 1) ? 1 : 0)
#endif

#endif
