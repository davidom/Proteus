#define STL_SQUARE(x)              ((x) * (x))
#define STL_DISTANCE(a,b)          (sqrt(STL_SQUARE((a)[0] - (b)[0]) + \
				         STL_SQUARE((a)[1] - (b)[1]) + \
				         STL_SQUARE((a)[2] - (b)[2])))

typedef struct _STL_Triang STL_Triang;

struct _STL_Triang
{
  INT_ id;                    
  DOUBLE_3D *v[3];
  DOUBLE_3D normal;		
};

double ug_io_get_stl_tolerance
 (void);

void ug_io_set_stl_tolerance
 (double);

INT_ ug_io_read_stl
 (FILE *,
  char[],
  INT_,
  INT_,
  INT_,
  INT_ *,
  INT_ *,
  INT_1D *,
  INT_3D *,
  DOUBLE_3D *);
