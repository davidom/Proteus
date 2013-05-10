#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/entity_iterator.hxx>
#include <geometry/geometry.hxx>
#include <geometry/topology.hxx>
#include <geometry/topology_constructor.hxx>

#include <algorithm>
#include <iostream>
#include <typeinfo>

template
<
  typename node_list,
  typename face_list
>
void
Proteus::node_to_face_topo_ctr::
construct(Proteus::topology <> & t, const node_list &nl, const face_list &fl)
{
  //resize the map
  t.resize(nl.size());

  //fill in the map
  for(int i=0; i<fl.size(); ++i) {
	for(int j=0; j<fl[i].size(); ++j) {
	  t.push(fl[i][j],i);
	}
  }
}
