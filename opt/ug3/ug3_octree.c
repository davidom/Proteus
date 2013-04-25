#include "UG3_LIB.h"

static INT_ UG3_OCTREE_VERTEX_ID = 0;

UG3_OCTREE *ug3_octree_allocatetree 
 (double xmin, double xmax,
  double ymin, double ymax, 
  double zmin, double zmax,
  INT_ level, INT_ maxlevel)
{
  INT_ i, j, Error_Flag;
  double dc1d2, xmn, xmx, ymn, ymx, zmn, zmx, xmid, ymid, zmid;
  UG3_OCTREE *octree_ptr = NULL;
  UG3_OCTREE *octant_ptr = NULL;

  dc1d2 = 0.5;

  if (level == 0) UG3_OCTREE_VERTEX_ID = 0;

  Error_Flag = 0;
  octree_ptr = (UG3_OCTREE *) ug_malloc (&Error_Flag, 1 * sizeof (UG3_OCTREE));

  if (Error_Flag > 0)
  {
    ug_message ("Unable to allocate octree");
    return NULL;
  }
   
  octree_ptr->xmin = xmin;
  octree_ptr->xmax = xmax;
  octree_ptr->ymin = ymin;
  octree_ptr->ymax = ymax;
  octree_ptr->zmin = zmin;
  octree_ptr->zmax = zmax;
  octree_ptr->pointlist = NULL;

  xmid = dc1d2 * (xmin + xmax);
  ymid = dc1d2 * (ymin + ymax);
  zmid = dc1d2 * (zmin + zmax);

  if (level < maxlevel)
  {
    for (i = 0; i < 8; i++)
    {
      switch(i)
      {
        case 0:
          xmn = xmin;
          ymn = ymin;
          zmn = zmin;
          xmx = xmid;
          ymx = ymid;
          zmx = zmid;
          break;
        case 1:
          xmn = xmid;
          ymn = ymin;
          zmn = zmin;
          xmx = xmax;
          ymx = ymid;
          zmx = zmid;
          break;
        case 2:
          xmn = xmin;
          ymn = ymid;
          zmn = zmin;
          xmx = xmid;
          ymx = ymax;
          zmx = zmid;
          break;
        case 3:
          xmn = xmin;
          ymn = ymin;
          zmn = zmid;
          xmx = xmid;
          ymx = ymid;
          zmx = zmax;
          break;
        case 4:
          xmn = xmin;
          ymn = ymid;
          zmn = zmid;
          xmx = xmid;
          ymx = ymax;
          zmx = zmax;
          break;
        case 5:
          xmn = xmid;
          ymn = ymin;
          zmn = zmid;
          xmx = xmax;
          ymx = ymid;
          zmx = zmax;
          break;
        case 6:
          xmn = xmid;
          ymn = ymid;
          zmn = zmin;
          xmx = xmax;
          ymx = ymax;
          zmx = zmid;
          break;
        case 7:
          xmn = xmid;
          ymn = ymid;
          zmn = zmid;
          xmx = xmax;
          ymx = ymax;
          zmx = zmax;
          break;
      }

      octant_ptr = ug3_octree_allocatetree (xmn, xmx, ymn, ymx, zmn, zmx,
					    level+1, maxlevel);
      if (octant_ptr == NULL)
      {
	ug_error_message ("Unable to allocate octree.");
	for (j = 0; j < i; j++)
	{
	  ug3_octree_destroytree (octree_ptr->octant[j]);
	}
	return NULL;
      }
      else
      {
	octree_ptr->octant[i] = octant_ptr;
      }
    }
  }
  else
  {
    for (i = 0; i < 8; i++)
    {
      octree_ptr->octant[i] = NULL;
    }
  }

  return octree_ptr;
}

void ug3_octree_destroytree (UG3_OCTREE *octree_ptr)
{
  INT_ i;
  UG3_OCTREE *octant_ptr = NULL;

  for (i = 0; i < 8; i++)
  {
    octant_ptr = octree_ptr->octant[i];
    if (octant_ptr != NULL)
    {
      ug3_octree_destroytree (octant_ptr);
    }
  }

  ug_free (octree_ptr);
  octree_ptr = NULL;

  return;
}

UG3_OCTREE_POINTLIST *ug3_octree_allocatepointlist (INT_ global_id,
                                                    UG3_OCTREE_POINTLIST *pointlist_array)
{
  UG3_OCTREE_POINTLIST *pointlist_ptr = NULL;

  if (pointlist_array == NULL)
  {
    ug_error_message ("NULL pointlist pointer");
    return NULL;
  }

  pointlist_ptr = pointlist_array;

  pointlist_ptr->pt.id = ++UG3_OCTREE_VERTEX_ID;
  pointlist_ptr->pt.global_id = global_id;
  pointlist_ptr->next = NULL;

  return pointlist_ptr;
}

void ug3_octree_destroypointlist (UG3_OCTREE_POINTLIST *pointlist_ptr)
{
  UG3_OCTREE_POINTLIST *current_ptr = pointlist_ptr;
  UG3_OCTREE_POINTLIST *next_ptr = NULL;

  while (current_ptr != NULL)
  {
    next_ptr = current_ptr->next;
    ug_free (current_ptr);

    current_ptr = next_ptr;
  }

  return;
}

INT_ ug3_octree_findpoint
 (UG3_OCTREE_POINTLIST *pointlist_ptr,
  double tol,
  DOUBLE_3D p,
  DOUBLE_3D *OctreeCoordinates)
{
  int inode;
  double dx, dy, dz, dist;
  UG3_OCTREE_POINTLIST *current_ptr = pointlist_ptr;
  UG3_OCTREE_POINT pt;

  while (current_ptr != NULL)
  {
    pt = current_ptr->pt;

    inode = pt.global_id;

    dx = p[0] - OctreeCoordinates[inode][0];
    dy = p[1] - OctreeCoordinates[inode][1];
    dz = p[2] - OctreeCoordinates[inode][2];

    dist = sqrt (dx*dx + dy*dy + dz*dz);
    if (dist <= tol)
    {
      return (pt.id);
    }

    current_ptr = current_ptr->next;
  }

  return (-1);
}

INT_ ug3_octree_addpoint
 (UG3_OCTREE *octree_ptr,
  INT_ global_id,
  DOUBLE_3D p,
  UG3_OCTREE_POINTLIST *pointlist_array)
{
  INT_ i, id;
  double xmax, xmin, ymax, ymin, zmax, zmin;
  UG3_OCTREE *octant_ptr = NULL;
  UG3_OCTREE_POINTLIST *pointlist_ptr = NULL;

  xmax = octree_ptr->xmax;
  xmin = octree_ptr->xmin;
  ymax = octree_ptr->ymax;
  ymin = octree_ptr->ymin;
  zmax = octree_ptr->zmax;
  zmin = octree_ptr->zmin;

  if (p[0] >= xmin && p[0] <= xmax &&
      p[1] >= ymin && p[1] <= ymax &&
      p[2] >= zmin && p[2] <= zmax)
  {
    if (octree_ptr->octant[0] != NULL)
    {
      for (i = 0; i < 8; i++)
      {
	octant_ptr = octree_ptr->octant[i];
	if (octant_ptr != NULL)
	{
	  id = ug3_octree_addpoint (octant_ptr, global_id, p, pointlist_array);
	  if (id > 0) return id;
	}
      }
    }
    else
    {
      pointlist_ptr = ug3_octree_allocatepointlist (global_id, pointlist_array);
      
      if (pointlist_ptr == NULL)
      {
	ug_message ("Unable to allocate octree pointlist.");
	return (-1);
      }

      pointlist_ptr->next = octree_ptr->pointlist;
      octree_ptr->pointlist = pointlist_ptr;
      return (pointlist_ptr->pt.id);
    }
  }

  return (0);
}

INT_ ug3_octree_findclosestpoint
 (UG3_OCTREE *octree_ptr,
  double tol,
  DOUBLE_3D p,
  DOUBLE_3D *OctreeCoordinates)
{
  INT_ i, found;
  double eps, xmax, xmin, ymax, ymin, zmax, zmin;
  UG3_OCTREE *octant_ptr = NULL;
  UG3_OCTREE_POINTLIST *pointlist_ptr = NULL;

  found = 0;
  eps = MAX (tol, 1.e-10);

  xmax = octree_ptr->xmax;
  xmin = octree_ptr->xmin;
  ymax = octree_ptr->ymax;
  ymin = octree_ptr->ymin;
  zmax = octree_ptr->zmax;
  zmin = octree_ptr->zmin;

  if ((p[0] >= (xmin - eps)) && (p[0] <= (xmax + eps)) &&
      (p[1] >= (ymin - eps)) && (p[1] <= (ymax + eps)) &&
      (p[2] >= (zmin - eps)) && (p[2] <= (zmax + eps)))
  {
    for (i = 0; i < 8; i++) 
    {
      octant_ptr = octree_ptr->octant[i];
      if (octant_ptr != NULL)
      {
        found = ug3_octree_findclosestpoint (octant_ptr, tol, p, OctreeCoordinates);
        if (found > 0) break;
      }
    }

    pointlist_ptr = octree_ptr->pointlist;
    if (pointlist_ptr != NULL)
    {
      found = ug3_octree_findpoint (pointlist_ptr, tol, p, OctreeCoordinates);
    }
  }

  return found;
}
