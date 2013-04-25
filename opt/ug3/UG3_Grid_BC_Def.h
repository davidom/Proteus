#ifndef __UG3_GRID_BC_DEF_H__

#define __UG3_GRID_BC_DEF_H__

#define STD_NULL_UG3_GBC		0	// standard surface
#define STD_UG3_GBC			1	// standard surface
#define BL_INT_UG3_GBC			2	// surface that intersects BL
#define TRANSP_SRC_UG3_GBC		3	// embedded/transparent surface
						// converted to source nodes
#define TRANSP_BL_INT_UG3_GBC		4	// embedded/transparent surface
						// that intersects BL
#define TRANSP_UG3_GBC			5	// embedded/transparent surface
#define TRANSP_INTRNL_UG3_GBC		6	// embedded/transparent surface
						// converted to internal faces
#define FIXED_BL_INT_UG3_GBC		7	// fixed surface with BL region
						// that intersects volume BL
#define TMP_TRANSP_UG3_GBC		105	// temporary surface
						// duplicate of an
						// embedded/transparent surface
#define TMP_TRANSP_INTRNL_UG3_GBC	106	// temporary surface
						// duplicate of an
						// embedded/transparent surface
						// converted to internal faces
#define SOLID_TRANSP_UG3_GBC		205	// embedded/transparent surface
						// temporarily solid
#define SOLID_TRANSP_INTRNL_UG3_GBC	206	// embedded/transparent surface
						// converted to internal faces
						// temporarily solid
#define SOLID_TMP_TRANSP_UG3_GBC	305	// temporary surface
						// duplicate of an
						// embedded/transparent surface
						// temporarily solid
#define SOLID_TMP_TRANSP_INTRNL_UG3_GBC	306	// temporary surface
						// duplicate of an
						// embedded/transparent surface
						// converted to internal faces
						// temporarily solid
#define PART_STD_UG3_GBC		998	// temporary surface
						// partition surface with
						// adjacent fully resolved
						// volume elements
#define PART_UG3_GBC			999	// temporary surface
						// partition surface with
						// adjacent unresolved
						// volume elements and
						// frozen nearby region
#endif
