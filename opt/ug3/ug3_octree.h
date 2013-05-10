typedef struct _UG3_OCTREE UG3_OCTREE;
typedef struct _UG3_OCTREE_POINT UG3_OCTREE_POINT;
typedef struct _UG3_OCTREE_POINTLIST UG3_OCTREE_POINTLIST;

struct _UG3_OCTREE_POINT
{
  INT_ id;
  INT_ global_id;
};

struct _UG3_OCTREE_POINTLIST
{
  UG3_OCTREE_POINT pt;
  UG3_OCTREE_POINTLIST *next;
};

struct _UG3_OCTREE
{
  double xmin, xmax, ymin, ymax, zmin, zmax;
  UG3_OCTREE_POINTLIST *pointlist;
  UG3_OCTREE *octant[8];
};

UG3_OCTREE *ug3_octree_allocatetree
 (double xmin, double xmax,
  double ymin, double ymax, 
  double zmin, double zmax,
  INT_ level, INT_ maxlevel);

void ug3_octree_destroytree (UG3_OCTREE *octree_ptr);

UG3_OCTREE_POINTLIST *ug3_octree_allocatepointlist 
 (INT_ global_id,
  UG3_OCTREE_POINTLIST *pointlist_array);

void ug3_octree_destroypointlist (UG3_OCTREE_POINTLIST *pointlist_ptr);

INT_ ug3_octree_findpoint
 (UG3_OCTREE_POINTLIST *pointlist_ptr,
  double tol,
  DOUBLE_3D p,
  DOUBLE_3D *OctreeCoordinates);

INT_ ug3_octree_addpoint
 (UG3_OCTREE *octree_ptr,
  INT_ global_id,
  DOUBLE_3D p,
  UG3_OCTREE_POINTLIST *pointlist_array);

INT_ ug3_octree_findclosestpoint
 (UG3_OCTREE *octree_ptr,
  double tol,
  DOUBLE_3D p,
  DOUBLE_3D *OctreeCoordinates);
