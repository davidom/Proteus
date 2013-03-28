#include "edge.hxx"
#include "edge_list.hxx"
#include "edge_list_impl.hxx"

namespace Proteus
{

  /**
    * \brief Node List Class Implementation
	*/

  // Add Node to Storage Container
  const edge & edge_list::impl::add_edge
    (const std::initializer_list<size_t> && l)
  {
	store_.emplace_back(std::unique_ptr<edge>(new edge(std::move(l))));
	return (*(store_.back()));
  }

  const edge & edge_list::impl::add_edge
    (const std::initializer_list<size_t> & l)
  {
	store_.emplace_back(std::unique_ptr<edge>(new edge(l)));
	return (*(store_.back()));
  }
}
