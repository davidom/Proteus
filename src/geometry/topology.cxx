#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/geometry.hxx>
#include <geometry/topology.hxx>

#include <algorithm>
#include <iostream>

void
Proteus::node_to_face_topo_ctr::construct()
const
{
  // Push counter variable for allocation later
  auto topo_b = t_.container_.begin();
  auto topo_e = t_.container_.end();
  for(auto itr=topo_b; itr!=topo_e; ++itr) {
	std::cout <<"pushing on entry: "<<std::distance(itr,topo_b)<<std::endl;
	if(itr->empty())
      itr->emplace_back(0);
  }
}
