#ifndef __PROTEUS_TOPOLOGY_CLASS__
#define __PROTEUS_TOPOLOGY_CLASS__

#include <vector>

namespace Proteus
{
  class topology_constructor
  {
	private:

	public:
	  virtual void construct() const = 0; 
  };

  template
  <
	typename from_list,
	typename to_list,
	typename constructor_class,
	typename container=std::vector<std::vector<std::size_t>>
  >
  class topology
  {
	friend constructor_class;

	private:
	  container container_;
	  topology() = delete;

	public:
	  topology(from_list fl, to_list tl) : fl_(fl), tl_(tl) { container_.resize(fl.size()); };
	  auto operator[](std::size_t n) const -> decltype(container_[n]) { return container_[n]; }
	  const from_list & fl_;
	  const to_list & tl_;
  };

  class node_to_face_topo_ctr : public topology_constructor
  {
	private:
	  Proteus::topology<Proteus::Geometry::node_list, Proteus::Geometry::face_list, Proteus::node_to_face_topo_ctr> & t_;

	public:
	  node_to_face_topo_ctr(Proteus::topology<Proteus::Geometry::node_list, Proteus::Geometry::face_list, Proteus::node_to_face_topo_ctr> t) : t_(t) {}
	  void construct() const;
  };
}

#endif
