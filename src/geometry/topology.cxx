#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/entity_iterator.hxx>
#include <geometry/geometry.hxx>
#include <geometry/topology.hxx>

#include <algorithm>
#include <iostream>
#include <typeinfo>

void
Proteus::node_to_face_topo_ctr::construct(
  Proteus::topology
  <
	Proteus::Geometry::node_list,
	Proteus::Geometry::face_list,
	Proteus::node_to_face_topo_ctr
  > &
  t
)
{

  // Resize the topology
  t.container_.resize(t.fl_.size());

  // Push counter variable for allocation later
  for(auto & itr : t.container_) {
	if(!itr.empty())
	  itr.clear();
	itr.push_back(0);
  }
  
  Proteus::all_range<decltype(t.tl_)> al(t.tl_);
  Proteus::entity_iterator<decltype(t.tl_), Proteus::all_range<decltype(t.tl_)>> ei(t.tl_, al);

  // Increment counter for each entity sharing that node
  for(auto & eit : ei) {
	for(decltype(eit.size()) i = 0; i< eit.size(); ++i) {
	  t.container_[eit[i]][0] += 1;
	}
  }

  // Reserve the space for all of the push_back
  for(auto & itr : t.container_) {
	itr.reserve(itr[0]);
	itr.pop_back();
  }

  // Construct map
  auto itr = ei.begin();
  while(itr != ei.end()) {
	for(decltype(itr->size()) i = 0; i<itr->size(); ++i) {
	  t.container_[itr[i]].push_back(itr.index());
	}

	++itr;
  }

  for(auto & itr : t.container_) {
	for(auto & iitr : itr) {
	  std::cout <<iitr<<" ";
	}
	std::cout <<std::endl;
  }
}
