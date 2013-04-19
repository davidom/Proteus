#ifndef __PROTEUS_GEOMETRY_CLASS__
#define __PROTEUS_GEOMETRY_CLASS__

#include <geometry/entity.hxx>
#include <array>
#include <vector>

namespace Proteus
{
  namespace Geometry
  {
	typedef entity<std::array<double,2>> node2d;
	typedef entity<std::array<double,3>> node3d;
	typedef entity<std::vector<double>> nodeNd;

	typedef entity<std::array<size_t,2>> edge;
	typedef entity<std::vector<size_t>> curve;

	typedef entity<std::array<size_t,3>> tria;
	typedef entity<std::array<size_t,4>> quad;
	typedef entity<std::vector<size_t>> face;

	typedef entity<std::array<size_t,4>> tet;
	typedef entity<std::array<size_t,5>> pent5;
	typedef entity<std::array<size_t,6>> pent6;
	typedef entity<std::array<size_t,8>> hex;
	typedef entity<std::vector<size_t>> cell;
  }

  template
  <
	typename node_type = Geometry::nodeNd,
	typename edge_type = Geometry::edge,
	typename face_type = Geometry::face,
	typename cell_type = Geometry::cell
  >
  class geometry
  {
	private:

	public:
	  geometry() {};
	  ~geometry() = default;

	  template<typename ...Args> 
	  node_type create_node(Args&& ...args) { return node_type( std::forward<Args>(args)... ); }
	  template<typename ...Args> 
	  edge_type create_edge(Args&& ...args) { return edge_type( std::forward<Args>(args)... ); }
	  template<typename ...Args> 
	  face_type create_face(Args&& ...args) { return face_type( std::forward<Args>(args)... ); }
	  template<typename ...Args> 
	  cell_type create_cell(Args&& ...args) { return cell_type( std::forward<Args>(args)... ); }
  };
}

#endif
