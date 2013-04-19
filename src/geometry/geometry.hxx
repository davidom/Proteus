#ifndef __PROTEUS_GEOMETRY_CLASS__
#define __PROTEUS_GEOMETRY_CLASS__

#include <geometry/entity.hxx>
#include <utility/pimpl_h.hxx>
#include <array>
#include <vector>

namespace Proteus
{

  typedef entity<std::array<double,2>> node2d;
  typedef entity<std::array<double,3>> node3d;
  typedef entity<std::vector<double>> nodeNd;

  typedef entity<std::array<size_t,2>> edge;

  typedef entity<std::array<size_t,3>> tria;
  typedef entity<std::array<size_t,4>> quad;
  typedef entity<std::vector<size_t>> face;

  typedef entity<std::array<size_t,4>> tet;
  typedef entity<std::vector<size_t>> cell;

  class geometry
  {
	private:
	  class geometry_impl;
	  pimpl<geometry_impl> m_;

	public:
	  geometry();
	  ~geometry();

  };
}

#endif
