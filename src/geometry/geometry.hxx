#ifndef __PROTEUS_GEOMETRY_CLASS__
#define __PROTEUS_GEOMETRY_CLASS__

#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/geometry_namespace.hxx>

namespace Proteus
{

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
