#ifndef __PROTEUS_GEOMETRY_CLASS__
#define __PROTEUS_GEOMETRY_CLASS__

#include <geometry/entity.hxx>
#include <array>
#include <vector>

namespace Proteus
{
  /** \brief Geometry Namespace
   *
   * This namespace holds some common typdefs for used in creating geometry
  */
  namespace Geometry
  {
	typedef entity<std::array<double,2>> node2d;
	typedef entity<std::array<double,3>> node3d;
	typedef entity<std::vector<double>> node;

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

  /** \brief Entity Factory
   *
   * This class is a factory-type definition for creating defined types of
   *  entities. The four functions define the four dimension of entities and
   *  offer a unified interface.
  */
  template
  <
	typename node_type = Geometry::node,
	typename edge_type = Geometry::edge,
	typename face_type = Geometry::face,
	typename cell_type = Geometry::cell
  >
  class geometry
  {
	private:

	public:
	  geometry() = default;
	  ~geometry() = default;

	  template<typename ...Args> 
	  node_type create_node(Args&& ...args) const { return node_type( std::forward<Args>(args)... ); }
	  template<typename ...Args> 
	  edge_type create_edge(Args&& ...args) const { return edge_type( std::forward<Args>(args)... ); }
	  template<typename ...Args> 
	  face_type create_face(Args&& ...args) const { return face_type( std::forward<Args>(args)... ); }
	  template<typename ...Args> 
	  cell_type create_cell(Args&& ...args) const { return cell_type( std::forward<Args>(args)... ); }
  };
}

#endif
