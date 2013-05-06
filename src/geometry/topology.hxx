#ifndef __PROTEUS_TOPOLOGY_CLASS__
#define __PROTEUS_TOPOLOGY_CLASS__

#include <vector>
#include <iostream>

namespace Proteus
{
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
      const from_list & fl_;
      const to_list & tl_;

      topology(const from_list & fl, const to_list & tl) : fl_(fl), tl_(tl) {}
      
      auto operator[](std::size_t n) const -> decltype(container_[n]) { return container_[n]; }

  };

  class node_to_face_topo_ctr
  {
    private:

	public:
      void construct(
        Proteus::topology
        <
          Proteus::Geometry::node_list,
          Proteus::Geometry::face_list,
          Proteus::node_to_face_topo_ctr
        > & t
      );
	  node_to_face_topo_ctr() = default;
  };
}

#endif
